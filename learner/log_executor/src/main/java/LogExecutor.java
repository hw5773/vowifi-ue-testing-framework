import java.io.*;
import java.net.Socket;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;
import java.sql.Time;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.TimeUnit;
import java.util.Iterator;
import java.util.Stack;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.commons.cli.*;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import static java.lang.Thread.sleep;

public class LogExecutor {
  private static VoWiFiUEConfig config = null;
  private static QueryReplyLogger qrLogger = null;

  final static Log logger = LogFactory.getLog(LogExecutor.class);

  public Socket ueSocket, epdgSocket, imsSocket;

  public BufferedWriter ueOut, epdgOut, imsOut;
  public BufferedReader ueIn, epdgIn, imsIn;

  static JSONParser jsonParser;

  int rebootCount = 0;
  int enableVoWiFiCount = 0;
  int initEPDGCount = 0;
  int initIMSCount = 0;

  private static final String[] OS_LINUX_RUNTIME = { "/bin/bash", "-l", "-c" };

  private static final int COOLING_TIME = 1*1000;
  private static final int TESTCASE_SLEEP_TIME = 3*1000;
  private static final int DEFAULT_SOCKET_TIMEOUT_VALUE = 10*1000; 
  private static final int EPDG_SOCKET_TIMEOUT_VALUE = 20*1000; 
  private static final int IMS_SOCKET_TIMEOUT_VALUE = 20*1000; 
  private static final int HELLO_MESSAGE_TIMEOUT_VALUE = 5*1000;
  private static final int UE_REBOOT_SLEEP_TIME = 45*1000;
  private static final String DEFAULT_CONF_FILE = "vowifi-ue.properties";
  private static final int DEFAULT_NUMBER_OF_TRIALS = 3;

  public LogExecutor(VoWiFiUEConfig config, QueryReplyLogger qrLogger) throws Exception {
    this.config = config;
    this.qrLogger = qrLogger;

    initUEConnection();
    initEPDGConnection();
    initIMSConnection();
  }

  public static void main(String[] args) throws Exception {
    List<Testcases> testcases = new ArrayList<>();
    LogExecutor logExecutor = null;
    VoWiFiUEConfig vowifiUEConfig = null;
    QueryReplyLogger vowifiQRLogger = null;

    Options options = new Options();

    Option argFile = new Option("f", "file", true, "File that contains testcases");
    options.addOption(argFile);

    Option argConfig = new Option("c", "config", true, "Configuration file");
    options.addOption(argConfig);

    CommandLineParser parser = new DefaultParser();
    HelpFormatter formatter = new HelpFormatter();
    CommandLine cmd = null;

    try {
      cmd = parser.parse(options, args);
    } catch (org.apache.commons.cli.ParseException e) {
      logger.error(e.getMessage());
      formatter.printHelp("Options", options);

      System.exit(1);
    }

    String testcaseFilePath = cmd.getOptionValue("file");
    String configFilePath = cmd.getOptionValue("config", DEFAULT_CONF_FILE);

    if (testcaseFilePath == null) {
      logger.error("Query File should be inserted");
      formatter.printHelp("Options", options);

      System.exit(1);
    }

    File ctest = new File(configFilePath);
    if (!ctest.exists()) {
      logger.error("Configuration File (" + configFilePath + ") does not exist");
      formatter.printHelp("Options", options);

      System.exit(1);
    }

    if (ctest.isDirectory()) {
      logger.error(configFilePath + " must not be a directory");
      formatter.printHelp("Options", options);

      System.exit(1);
    }

    try {
      vowifiUEConfig = new VoWiFiUEConfig(configFilePath);
    } catch (Exception e) {
      logger.error("Error happened while processing the configuration file");
      e.printStackTrace();
    }

    try {
      String outputDir = vowifiUEConfig.getOutputDir();
      vowifiQRLogger = new QueryReplyLogger(outputDir, logger);
    } catch (Exception e) {
      logger.error("Error happened while initializing QueryReplyLogger");
      e.printStackTrace();
    }

    try {
      logExecutor = new LogExecutor(vowifiUEConfig, vowifiQRLogger);
    } catch (Exception e) {
      logger.error("Error happend while initializing LogExecutor");
      e.printStackTrace();
    }

    jsonParser = new JSONParser();

    logger.info("Testcase File Mode (File Path: " + testcaseFilePath + ")");
    try (FileReader reader = new FileReader(testcaseFilePath)) {
      JSONObject jsonObject = (JSONObject) jsonParser.parse(reader);
      JSONArray testcaseArray = (JSONArray) jsonObject.get("testcases");
      logger.info("testcases: " + testcaseArray);
      Iterator i = testcaseArray.iterator();

      while (i.hasNext()) {
        JSONObject tc = (JSONObject) i.next();
        logger.info("Next Message: " + tc);
        Testcases tcs = new Testcases(tc, logger);
        testcases.add(tcs);
      }
    } catch (Exception e) {
      logger.error("Error happened while processing the testcase file");
      e.printStackTrace();
    }

    logger.info("# of Testcases: " + testcases.size());

    Boolean timeoutOccured = false;
    int testcaseNum = 1;
    List<QueryReplyPair> pairs;
    Iterator iter;

    for (Testcases testcase: testcases) {
      logger.info("Starting Testcase #" + testcaseNum);

      pairs = executeTestcase(logExecutor, testcase);

      while (pairs == null) {
        pairs = executeTestcase(logExecutor, testcase);
      }

      logger.info("Finished Testcase #" + testcaseNum);
      testcaseNum ++;

      qrLogger.addLog(testcase, pairs);
      sleep(TESTCASE_SLEEP_TIME);
    }

    qrLogger.storeLog();
  }

  public void restartEPDG() throws InterruptedException {
    logger.debug("START: restartEPDG()");
    logger.debug("Invoke killEPDG()");
    killEPDG();
    sleep(COOLING_TIME);
    logger.debug("Invoke startEPDG()");
    startEPDG();
    sleep(COOLING_TIME);
    logger.debug("Invoke initEPDGConnection()");
    initEPDGConnection();
    logger.debug("FINISH: restartEPDG()");
  }

  public void restartIMS() throws InterruptedException {
    logger.debug("START: restartIMS()");
    logger.debug("Invoke killIMS()");
    killIMS();
    sleep(COOLING_TIME);
    logger.debug("Invoke startIMS()");
    startIMS();
    sleep(COOLING_TIME);
    logger.debug("Invoke initIMSConnection()");
    initIMSConnection();

    logger.debug("FINISH: restartIMS()");
  }

  private void sendMSGToEPDG(Testcase testcase) {
    logger.debug("START: sendMSGToEPDG()");
    int depth;

    depth = 0;
    sendMessage(testcase, depth, "epdg");

    logger.debug("FINISH: sendMSGToEPDG()");
  }

  private void sendMSGToIMS(Testcase testcase) {
    logger.debug("START: sendMSGToEPDG()");
    int depth;

    depth = 0;
    sendMessage(testcase, depth, "ims");

    logger.debug("FINISH: sendMSGToEPDG()");
  }

  private void sendMessage(Testcase testcase, int depth, String receiver) {
    String name;
    String ispi, rspi;
    String msg;
    String print;
    Iterator iter;

    name = testcase.getName();
    ispi = testcase.getIspi();
    rspi = testcase.getRspi();
    logger.debug("Testcase: " + name + ", Type: " + testcase.getTestcaseType() + ", ISPI: " + ispi + ", RSPI: " + rspi);
    print = "";

    try {
      for (int i=0; i<depth; i++)
        print += "  ";

      if (testcase.getTestcaseType() != TestcaseType.ATTRIBUTE) {
        msg = "2";
      } else {
        msg = "1";
      }
      msg += ispi;
      msg += rspi;
      msg += name;

      if (testcase.hasValue()) {
        msg += ":";
        msg += testcase.getValueType().getValueTypeAsInteger();
        msg += ":";
        msg += testcase.getValue();
      }

      msg += "\n";
      if (receiver.startsWith("epdg")) {
        epdgOut.write(msg);
        epdgOut.flush();
      } else if (receiver.startsWith("ims")) {
        imsOut.write(msg);
        imsOut.flush();
      }

      print += name;
      if (testcase.hasValue()) {
        print += ": ";
        print += testcase.getValue();
      }
      logger.info(print);

      if (testcase.getHasSubTestcase()) {
        depth++;
        while (testcase.getHasNextSubTestcase()) {
          sendMessage(testcase.getNextSubTestcase(), depth, receiver);
        }
        depth--;
      }

      if (testcase.getTestcaseType() != TestcaseType.ATTRIBUTE) {
        msg = "3";
        print = "";
        for (int i=0; i<depth; i++) {
          print += "  ";
        }
        print += "end";
        logger.info(print);

        msg += ispi;
        msg += rspi;
        msg += "\n";
        if (receiver.startsWith("epdg")) {
          epdgOut.write(msg);
          epdgOut.flush();
        } else if (receiver.startsWith("ims")) {
          imsOut.write(msg);
          imsOut.flush();
        }
      }
    } catch (SocketException e) {
      e.printStackTrace();
    } catch (IOException e) {
      e.printStackTrace();
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  public void initUEConnection() {
    logger.debug("START: initUEConnection()");

    String result;
    String ueControllerIPAddress = config.getUEControllerIPAddress();
    int uePort = config.getUEControllerPort();
    
    try {
      logger.info("Connecting to UE...");
      logger.info("UE controller Address: " + ueControllerIPAddress + ":" + uePort);
      ueSocket = new Socket(ueControllerIPAddress, uePort);
      ueSocket.setTcpNoDelay(true);
      logger.info("Connected to UE");

      logger.debug("Initializing Buffers for UE...");
      ueOut = new BufferedWriter(new OutputStreamWriter(ueSocket.getOutputStream()));
      ueIn = new BufferedReader(new InputStreamReader(ueSocket.getInputStream()));
      logger.debug("Initialized Buffers for UE");

      ueOut.write("device\n");
      ueOut.flush();

      result = ueIn.readLine();
      logger.info("UE model: " + result);
      qrLogger.setUEModel(result);

    } catch (UnknownHostException e) {
      e.printStackTrace();
    } catch (SocketException e) {
      e.printStackTrace();
    } catch (Exception e) {
      e.printStackTrace();
    }
    
    logger.debug("FINISH: initUEConnection()");
  }

  public void initEPDGConnection() {
    logger.debug("START: initEPDGConnection()");

    String epdgControllerIPAddress = config.getEPDGControllerIPAddress();
    int epdgPort = config.getEPDGControllerPort();

    try {
      logger.info("Connecting to ePDG...");
      epdgSocket = new Socket(epdgControllerIPAddress , epdgPort);
      epdgSocket.setTcpNoDelay(true);
      epdgOut = new BufferedWriter(new OutputStreamWriter(epdgSocket.getOutputStream()));
      epdgIn = new BufferedReader(new InputStreamReader(epdgSocket.getInputStream()));
      System.out.println("The TCP connection with ePDG is established.");

      String result = new String();
      try {
        sleep(1*1000);
        epdgOut.write("Hello\n");
        epdgOut.flush();
        System.out.println("Sent = Hello");
        result = epdgIn.readLine();
        System.out.println("Received = " + result);
      } catch(SocketException e) {
        e.printStackTrace();
        startEPDG();
        initEPDGConnection();
      } catch(IOException e) {
        e.printStackTrace();
        startEPDG();
        initEPDGConnection();
      } catch(Exception e) {
        e.printStackTrace();
        startEPDG();
        initEPDGConnection();
      }

      if (result.startsWith("ACK")) {
        logger.info("PASSED: Testing the connection between the statelearner and the ePDG");
      }
    } catch (UnknownHostException e) {
      e.printStackTrace();
      startEPDG();
      initEPDGConnection();
    } catch (SocketException e) {
      e.printStackTrace();
      startEPDG();
      initEPDGConnection();
    } catch (Exception e) {
      e.printStackTrace();
      startEPDG();
      initEPDGConnection();
    }
    System.out.println("Initialize the connection with ePDG success");
    logger.debug("FINISH: initEPDGConnection()");
  }

  public void initIMSConnection() {
    logger.debug("START: initIMSConnection()");
    
    String imsControllerIPAddress = config.getIMSControllerIPAddress();
    int imsPort = config.getIMSControllerPort();
   
    try {
      // Initialize test service
      System.out.println("Connecting to IMS (S-CSCF)...");
      System.out.println("IP address: " + imsControllerIPAddress + " / Port: " + imsPort);
      imsSocket = new Socket(imsControllerIPAddress, imsPort);
      imsSocket.setTcpNoDelay(true);
      imsOut = new BufferedWriter(new OutputStreamWriter(imsSocket.getOutputStream()));
      imsIn = new BufferedReader(new InputStreamReader(imsSocket.getInputStream()));
      System.out.println("The connection with IMS (S-CSCF) is established.");

      String result = new String();
      try {
        sleep(2*1000);
        imsOut.write("Hello\n");
        imsOut.flush();
        System.out.println("Sent = Hello");
        result = imsIn.readLine();
        System.out.println("Received = " + result);
      } catch(SocketException e) {
        e.printStackTrace();
        startIMS();
        initIMSConnection();
      } catch(IOException e) {
        e.printStackTrace();
        startIMS();
        initIMSConnection();
      } catch(Exception e) {
        e.printStackTrace();
        startIMS();
        initIMSConnection();
      }

      if(result.startsWith("ACK")) {
        logger.info("PASSED: Testing the connection between the statelearner and IMS (S-CSCF)");
      }
    } catch (UnknownHostException e) {
      e.printStackTrace();
      startIMS();
      initIMSConnection();
    } catch (SocketException e) {
      e.printStackTrace();
      startIMS();
      initIMSConnection();
    } catch (Exception e) {
      e.printStackTrace();
      startIMS();
      initIMSConnection();
    }
    System.out.println("Initialize the connection with IMS (S-CSCF) success");
    
    logger.debug("FINISH: initIMSConnection()");
  }

  public static List<QueryReplyPair> executeTestcase(LogExecutor logExecutor, Testcases testcase) {
    logger.debug("START: executeTestcase()");

    boolean timeoutOccured = false;
    boolean exceptionOccured = false;
    long startTime, endTime, duration;
    double insec;
    String ispi = null;
    String rspi = null;
    List<QueryReplyPair> pairs;

    logExecutor.pre();
    pairs = new ArrayList<>();

    while (testcase.hasNextMessage()) {
      Testcase message = testcase.getNextMessage();
      QueryReplyPair pair;
      String name = message.getName();

      if (ispi != null) {
        message.setIspi(ispi);
      }

      if (rspi != null) {
        message.setRspi(rspi);
      }

      if (name.contains("ε"))
        continue;

      logger.info("COMMAND: " + name);

      if (timeoutOccured) {
        logger.info("RESULT: null_action (timeout)");
        continue;
      }

      startTime = System.currentTimeMillis();
      pair = logExecutor.step(message);
      endTime = System.currentTimeMillis();
      logger.info(">>>>> Query: " + pair.getQueryName() + " / Reply: " + pair.getReplyName() + " <<<<<");
      duration = (endTime - startTime);
      insec = duration/1000.0;
      logger.info("Elapsed Time in prefix: " + duration + " ms (" + insec + " s)");
      
      if (ispi == null) {
        ispi = pair.getIspi();
        logger.debug("ISPI is set to " + ispi);
      }

      if (rspi == null) {
        rspi = pair.getRspi();
        logger.debug("RSPI is set to " + rspi);
      }

      pairs.add(pair);

      if (pair.getReplyName().matches("exception")) {
        logger.info("Exception occured, restarting testcase");
        exceptionOccured = true;
        break;
      }

      if (pair.getReplyName().matches("timeout")) {
        timeoutOccured = true;
        logger.info("RESULT: null_action (timeout)");
        continue;
      }

      logger.info("RESULT: " + pair.getReplyName());
    }
    logExecutor.post();

    if (exceptionOccured)
      pairs = null;

    logger.debug("FINISH: executeTestcase()");
    return pairs;
  }

  public boolean initEPDG() {
    logger.debug("START: initEPDG()");
    String result = "";
    boolean ret = false;

    logger.info("Sending symbol: init to ePDG");
    try {
      sleep(COOLING_TIME);
      epdgOut.write("init\n");
      epdgOut.flush();
      initEPDGCount++;
      result = epdgIn.readLine();
      logger.info("ACK for initEPDG(): " + result);
    } catch(SocketException e) {
      e.printStackTrace();
    } catch(IOException e) {
      e.printStackTrace();
    } catch(Exception e) {
      e.printStackTrace();
    }

    if(result.contains("ACK")) {
      logger.info("PASSED: Initializing the ePDG is succeeded");
      ret = true;
    } else {
      logger.error("FAILED: Initializing the ePDG is failed");
      ret = false;
    }

    logger.debug("FINISH: initEPDG()");
    return ret;
  }

  public boolean finEPDG() {
    logger.debug("START: finEPDG()");
    String result = "";
    boolean ret = false;

    sendDisableWiFi();
    logger.info("Sending symbol: fin to ePDG");
    try {
      epdgOut.write("fin\n");
      epdgOut.flush();
      //result = epdgIn.readLine();
      //logger.info("ACK for finEPDG(): " + result);
    } catch(SocketException e) {
      e.printStackTrace();
    } catch(IOException e) {
      e.printStackTrace();
    } catch(Exception e) {
      e.printStackTrace();
    }

    /*
    if(result.contains("ACK")) {
      logger.info("PASSED: Finalizing the ePDG is succeeded");
      ret = true;
    } else {
      logger.error("FAILED: Finalizing the ePDG is failed");
      ret = false;
    }
    */

    logger.debug("FINISH: finEPDG()");
    return ret;
  }

  public boolean initIMS() {
    logger.debug("START: initIMS()");
    String result = "";

    /*
    System.out.println("Sending symbol: init to IMS Server");
    try {
      sleep(1*1000);
      imsOut.write("RESET " + initIMSCount + "\n");
      imsOut.flush();
      result = imsIn.readLine();
      logger.info("ACK for initIMS(): " + result);
      initIMSCount++;
    } catch(SocketException e) {
      e.printStackTrace();
    } catch(IOException e) {
      e.printStackTrace();
    } catch(Exception e) {
      e.printStackTrace();
    }
    */
    logger.debug("FINISH: initIMS()");
    return true;
  }

  public boolean finIMS() {
    logger.debug("START: finIMS()");
    String result = "";
    boolean ret = false;

    /*
    logger.info("Sending symbol: fin to IMS");
    try {
      imsOut.write("fin\n");
      imsOut.flush();
      result = epdgIn.readLine();
      logger.info("ACK for finIMS(): " + result);
    } catch(SocketException e) {
      e.printStackTrace();
    } catch(IOException e) {
      e.printStackTrace();
    } catch(Exception e) {
      e.printStackTrace();
    }

    if(result.contains("ACK")) {
      logger.info("PASSED: Finalizing the IMS is succeeded");
      ret = true;
    } else {
      logger.error("FAILED: Finalizing the IMS is failed");
      ret = false;
    }
    */

    logger.debug("FINISH: finIMS()");
    return ret;
  }

  public boolean resetUE() {
    logger.debug("START: resetUE()");
    String result = "";
    
    logger.info("Sending symbol: RESET to UE controller");
    try {
      sleep(COOLING_TIME);
      ueOut.write("reset\n");
      ueOut.flush();
      result = ueIn.readLine();
      logger.info("ACK for resetUE(): " + result);
    } catch(SocketException e) {
      e.printStackTrace();
    } catch(IOException e) {
      e.printStackTrace();
    } catch(Exception e) {
      e.printStackTrace();
    }
    
    if(result.contains("ACK")) {
      logger.info("PASSED: Testing the connection between the statelearner and UE");
      logger.debug("FINISH: resetUE()");
      return true;
    } else {
      logger.error("FAILED: Testing the connection between the statelearner and UE");
      logger.debug("FINISH: resetUE()");
      return false;
    }
  }

  public boolean rebootUE() {
    logger.debug("START: rebootUE()");
    String result = "";
    
    try {
      sleep(COOLING_TIME);
      ueOut.write("ue_reboot\n");
      ueOut.flush();
      logger.info("Waiting for the response from UE .... ");
      result = ueIn.readLine();
      logger.info("UE's ACK for REBOOT: " + result);
    } catch (SocketException e) {
      e.printStackTrace();
    } catch(IOException e) {
      e.printStackTrace();
    } catch(Exception e) {
      e.printStackTrace();
    }
    
    if(result.contains("ACK")) {
      logger.info("PASSED: Testing the connection between the statelearner and UE");
      logger.debug("FINISH: rebootUE()");
      return true;
    } else {
      System.out.println("FAILED: Testing the connection between the statelearner and UE");
      logger.debug("FINISH: rebootUE()");
      return false;
    }
  }

  public String restartUEAdbServer() {
    logger.debug("START: restartUEAdbServer()");
    String result = new String("");
    
    try {
      sleep(COOLING_TIME);
      ueOut.write("adb_server_restart\n");
      ueOut.flush();
      result = ueIn.readLine();
      logger.info("Result for adb_server_restart: " + result);
    } catch (SocketException e) {
      e.printStackTrace();
    } catch(IOException e) {
      e.printStackTrace();
    } catch(Exception e) {
      e.printStackTrace();
    }
    
    logger.debug("FINISH: restartUEAdbServer()");
    return result;
  }

  public boolean sendDisableWiFi() {
    boolean ret = false;
    String result;

    try {
			ueSocket.setSoTimeout(DEFAULT_SOCKET_TIMEOUT_VALUE);
      logger.info("Sending symbol: wifi_off to UE Controller to enable VoWiFi");
      ueOut.write("wifi_off\n");
      ueOut.flush();

      result = ueIn.readLine();
      logger.info("UE Controller's ACK for reset: " + result);
      if (result.contains("ACK")) {
        ret = true;
      }
    } catch (SocketException e) {
      ret = false;
      e.printStackTrace();
    } catch (IOException e) {
      ret = false;
      e.printStackTrace();
    } catch (Exception e) {
      ret = false;
      e.printStackTrace();
    }

    return ret;
  }

  public boolean sendEnableWiFi() {
    boolean ret = false;
    String result;

    try {
			ueSocket.setSoTimeout(DEFAULT_SOCKET_TIMEOUT_VALUE);
      logger.info("Sending symbol: wifi_on to UE Controller to enable VoWiFi");
      ueOut.write("wifi_on\n");
      ueOut.flush();

      result = ueIn.readLine();
      logger.info("UE Controller's ACK for reset: " + result);
      if (result.contains("ACK")) {
        ret = true;
      }
    } catch (SocketException e) {
      ret = false;
      e.printStackTrace();
    } catch (IOException e) {
      ret = false;
      e.printStackTrace();
    } catch (Exception e) {
      ret = false;
      e.printStackTrace();
    }

    return ret;
  }

  public void sendEnableVoWiFi() {
    String result;
    int retry = 3;
    boolean enabled = false;
    boolean rebooted = false;

    try {
      sleep(COOLING_TIME);

      while (enabled == false && retry > 0) {
        if (sendDisableWiFi()) {
          if (initEPDG()) {
            enabled = sendEnableWiFi();
          }
        }
        retry--;
      }

			ueSocket.setSoTimeout(UE_REBOOT_SLEEP_TIME);
      while (enabled == false && rebooted == false) {
        logger.info("Sending symbol: ue_reboot to UE Controller to enable VoWiFi");
        ueOut.write("ue_reboot\n");
        ueOut.flush();

        result = ueIn.readLine();
        logger.info("UE Controller's ACK for ue_reboot: " + result);
        if (result.contains("ACK")) {
          enabled = true;
        }
        rebooted = true;
      }

      if (enabled == false) {
        logger.error("Failed to start the VoWiFi protocol");
        System.exit(1);
      }

      ueSocket.setSoTimeout(DEFAULT_SOCKET_TIMEOUT_VALUE);
    } catch (SocketException e) {
      e.printStackTrace();
    } catch (IOException e) {
      e.printStackTrace();
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  private MessageLog processResult(Testcase testcase, String reporter) {
    String result = null;
    String rstr;
    String print;
    String spi, ispi, rspi;
    byte rcvd[];
    MessageLog mlog = null;
    Stack<Integer> stack = new Stack<Integer>();
    int idx, type, len, depth;
    BufferedReader sockIn = null;

    depth = 0;
    try {
      if (reporter.startsWith("epdg")) {
  		  epdgSocket.setSoTimeout(EPDG_SOCKET_TIMEOUT_VALUE);
        sockIn = epdgIn;
      } else if (reporter.startsWith("ims")) {
        imsSocket.setSoTimeout(IMS_SOCKET_TIMEOUT_VALUE);
        sockIn = imsIn;
      }
      do {
        print = "";
        for (int i=0; i<depth; i++)
        {
          print += "  ";
        }
      	result = sockIn.readLine();
        logger.debug("Result from " + reporter + ": " + result);
        len = result.length();
        idx = 0;
        rcvd = result.getBytes();
        type = (char) rcvd[idx++];

        switch (type) {
          case 1:
            if (mlog == null) {
              logger.error("The attribute should be within the block");
            } else {
              mlog = mlog.addSubmessage(MessageLogType.ATTRIBUTE);
            }
            break;

          case 2:
            if (mlog == null) {
              mlog = new MessageLog(testcase, MessageLogType.MESSAGE, logger);
            } else {
              mlog = mlog.addSubmessage(MessageLogType.PAYLOAD);
            }
            depth++;
            stack.push(1);
            break;

          case 3:
            if (mlog.hasParent()) {
              mlog = mlog.getParent();
            }
            depth--;
            stack.pop();
            break;
        }

        len -= 1;
        ispi = result.substring(idx, idx + 16);
        idx += 16; len -= 16;
        spi = mlog.getIspi();
        if (spi == null) {
          mlog.setIspi(ispi);
        } else {
          if (!spi.equals(ispi)) {
            logger.error("Initiator's SPIs are different");
          }
        }

        rspi = result.substring(idx, idx + 16);
        idx += 16; len -= 16;
        spi = mlog.getRspi();
        if (spi == null) {
          mlog.setRspi(rspi);
        } else {
          if (!spi.equals(rspi)) {
            logger.error("Responder's SPIs are different");
          }
        }

        if (len > 0)
        {
          String[] arr;
          rstr = result.substring(idx);
          print += rstr;
          arr = rstr.split(":", 0);

          mlog.setName(arr[0]);

          if (arr.length > 1) {
            if (arr.length != 3) {
              logger.error("The array length should be 3");
            }
            else {
              mlog.setValueType(arr[1]);
              mlog.setValue(arr[2]);
            }
          }
          logger.info(print);
        }
      } while (stack.size() != 0);
  	  epdgSocket.setSoTimeout(DEFAULT_SOCKET_TIMEOUT_VALUE);
		} catch (SocketTimeoutException e) {
			logger.info("Timeout occured for " + testcase.getName());
			handleTimeout();
			mlog = new MessageLog(testcase, MessageLogType.MESSAGE, logger);
      mlog.setName("timeout");
    } catch (Exception e) {
			e.printStackTrace();
			logger.info("Attempting to restart device, and reset ePDG and IMS Server. Also restarting testcase.");
			handleEPDGIMSFailure();
			mlog = new MessageLog(testcase, MessageLogType.MESSAGE, logger);
      mlog.setName("null_action");
		}

    return mlog;
  }

  public void pre() {
    logger.debug("START: pre()");
    
	  int flag = 0; 
    int trial = 0;
    boolean ret;
    boolean resetDone = false;
		String result = "";
		String resultForUE = "";
		String resultForEPDG = "";
		String resultForIMS = "";

		try {
			logger.info("---- Starting RESET ----");

			do {
				try {
          if (trial == DEFAULT_NUMBER_OF_TRIALS) {
            logger.error("Resetting UE failed");
            System.exit(1);
          }

          trial = 0;
          do {
            trial++;
            ret = initIMS();
          } while (ret == false && trial < DEFAULT_NUMBER_OF_TRIALS);

          if (trial == DEFAULT_NUMBER_OF_TRIALS) {
            logger.error("Resetting IMS failed");
            System.exit(1);
          }
            
          if (isEPDGAlive() && isIMSAlive())
            resetDone = true;
        } catch (Exception e) {
          e.printStackTrace();
        }
			}while(resetDone == false);

			logger.info("---- RESET DONE ----");
		} catch (Exception e) {
			e.printStackTrace();
			throw new RuntimeException(e);
		}

    logger.debug("FINISH: pre()");
	}

	public QueryReplyPair step(Testcase testcase) {
    logger.debug("START: step()");
    MessageLog query, reply;
    QueryReplyPair pair;
    String tname = testcase.getName();
    String receiver = testcase.getQueryReceiver();
    String sender = testcase.getReplySender();
    String qname, rname;
    boolean ret;
    logger.info("Testcase (" + tname + ")'s ISPI: " + testcase.getIspi() + ", RSPI: " + testcase.getRspi());
    
		try {
			sleep(50); //50 milliseconds
		} catch (Exception e) {
			e.printStackTrace();
		}

    if (receiver.startsWith("epdg")) {
      logger.info("sendMSGToEPDG()");
  		sendMSGToEPDG(testcase);
      logger.debug("Receiver 1: " + receiver);
      query = processResult(testcase, receiver);
    } else if (receiver.startsWith("ims")) {
      logger.info("sendMSGToIMS()");
      sendMSGToIMS(testcase);
      logger.debug("Receiver 2: " + receiver);
      query = processResult(testcase, receiver);
    } else {
      logger.info("sendEnableVoWiFi(): enable_vowifi");
  		sendEnableVoWiFi();
      query = new MessageLog(testcase, MessageLogType.MESSAGE, logger);
      query.setName("enable_vowifi");
    }
    logger.debug("Sender: " + sender);
    reply = processResult(testcase, sender);
    rname = reply.getName();

	  logger.info("##### " + query.getName() + " -> " + reply.getName() + " #####");
    pair = new QueryReplyPair(testcase, query, reply, logger);
    
    logger.debug("FINISH: step()");
		return pair;
	}

  public void post() {
    logger.debug("START: post()");

    finEPDG();
    //finIMS();

    logger.debug("FINISH: post()");
  }

  public void handleTimeout(){
    logger.debug("START: handleTimeout()");
    
    if (isEPDGAlive() == false || isIMSAlive() == false) {
      handleEPDGIMSFailure();
      return;
    }
    
    logger.debug("FINISH: handleTimeout()");
  }

  public void isAdbServerRestartRequired() {
    logger.debug("START: isAdbServerRestartRequired()");
    logger.debug("FINISH: isAdbServerRestartRequired()");
  }

  public void handleEPDGIMSFailure() {
    logger.debug("START: handleEPDGIMSFailure()");

    try {
      restartEPDG();
      restartIMS();
    } catch (Exception e) {
      e.printStackTrace();
    }
    logger.debug("FINISH: handleEPDGIMSFailure()");
  }

  public boolean isEPDGAlive() {
    logger.debug("START: isEPDGAlive()");

    String result;
    
    try {
      epdgSocket.setSoTimeout(HELLO_MESSAGE_TIMEOUT_VALUE);
      epdgOut.write("Hello\n");
      epdgOut.flush();
      logger.info("Sent the Hello message to ePDG");
      result = epdgIn.readLine();
      logger.info("Received the Hello message from ePDG in isEPDGAlive() = " + result);
      epdgSocket.setSoTimeout(DEFAULT_SOCKET_TIMEOUT_VALUE);
    } catch(SocketTimeoutException e) {
      logger.error("Timeout in Socket with ePDG");
      e.printStackTrace();
      return false;
    } catch (Exception e) {
      e.printStackTrace();
      return false;
    }

    if(result.contains("ACK")) {
      logger.info("PASSED: Testing the connection between the statelearner and ePDG");
      return true;
    } else {
      logger.error("FAILED: Testing the connection between the statelearner and ePDG");
      return false;
    }
  }

  public boolean isIMSAlive() {
    logger.debug("START: isIMSAlive()");

    /*
    String result;

    try {
      imsSocket.setSoTimeout(HELLO_MESSAGE_TIMEOUT_VALUE);
      imsOut.write("Hello\n");
      imsOut.flush();
      logger.info("Sent the Hello message to IMS Server");
      result = imsIn.readLine();
      System.out.println("Received the Hello messge from IMS Server in isIMSAlive() = " + result);
      imsSocket.setSoTimeout(DEFAULT_SOCKET_TIMEOUT_VALUE);
    } catch(SocketTimeoutException e) {
      logger.error("Timeout in Socket with IMS Server");
      e.printStackTrace();
      return false;
    } catch (Exception e) {
      e.printStackTrace();
      return false;
    }

    if(result.contains("ACK")) {
      logger.info("PASSED: Testing the connection between the statelearner and the srsEPC");
      return true;
    } else {
      logger.error("FAILED: Testing the connection between the statelearner and the srsEPC");
      return false;
    }
    */
    logger.debug("FINISH: isIMSAlive()");
    return true;
  }

  public static void startEPDG() {
    logger.debug("START: startEPDG()");

    String epdgStartCmd = config.getEPDGStartCmd();
    logger.info("start ePDG by " + epdgStartCmd);
    runProcess(epdgStartCmd);

    logger.debug("FINISH: startEPDG()");
  }

  public static void startIMS() {
    logger.debug("START: startIMS()");

    //String imsStartCmd = config.getIMSStartCmd();
    //logger.info("start IMS Server by " + imsStartCmd);
    //runProcess(imsStartCmd);

    logger.debug("FINISH: startIMS()");
  }

  public static void killEPDG() {
    logger.debug("START: killEPDG()");

    String epdgStopCmd = config.getEPDGStopCmd();
    logger.info("kill ePDG by " + epdgStopCmd);
    runProcess(epdgStopCmd);

    logger.debug("FINISH: killEPDG()");
  }

  public static void killIMS() {
    logger.debug("START: killIMS()");

    String imsStopCmd = config.getIMSStopCmd();
    logger.info("kill IMS Server by " + imsStopCmd);
    runProcess(imsStopCmd);

    logger.debug("FINISH: killIMS()");
  }

  public static void runProcess(String... command) {
    System.out.print("command to run: ");
    for (String s : command) {
      System.out.print(s);
    }
    System.out.print("\n");
    String[] allCommand = null;
    try {
      allCommand = concat(OS_LINUX_RUNTIME, command);

      ProcessBuilder pb = new ProcessBuilder(allCommand);
      pb.redirectErrorStream(true);
      Process p = pb.start();

      return;
    } catch (IOException e) {
      logger.error("ERROR: " + command + " is not running after invoking script");
      logger.error("Attempting again...");
      e.printStackTrace();
    } catch (Exception e) {
      e.printStackTrace();
      return;
    }
  }

  private static <T> T[] concat(T[] first, T[] second) {
    T[] result = Arrays.copyOf(first, first.length + second.length);
    System.arraycopy(second, 0, result, first.length, second.length);
    return result;
  }
}
