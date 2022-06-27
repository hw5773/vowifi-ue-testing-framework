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

  final static Log logger = LogFactory.getLog(LogExecutor.class);

  public Socket ueSocket, epdgSocket, imsSocket;

  public BufferedWriter ueOut, epdgOut, imsOut;
  public BufferedReader ueIn, epdgIn, imsIn;

  static JSONParser jsonParser;

  int rebootCount = 0;
  int enableVoWiFiCount = 0;
  int resetEPDGCount = 0;
  int resetIMSCount = 0;

  private static final String[] OS_LINUX_RUNTIME = { "/bin/bash", "-l", "-c" };

  private static final int COOLING_TIME = 5*1000;
  private static final int DEFAULT_SOCKET_TIMEOUT_VALUE = 30*1000; 
  private static final int EPDG_SOCKET_TIMEOUT_VALUE = 180*1000; 
  private static final int HELLO_MESSAGE_TIMEOUT_VALUE = 5*1000;
  private static final int UE_REBOOT_SLEEP_TIME = 45*1000;
  private static final String DEFAULT_CONF_FILE = "vowifi-ue.properties";
  private static final int DEFAULT_NUMBER_OF_TRIALS = 3;

  public LogExecutor(VoWiFiUEConfig config) throws Exception {
    this.config = config;

    initUEConnection();
    initEPDGConnection();
    initIMSConnection();
  }

  public static void main(String[] args) throws Exception {
    List<QueryString> queries = new ArrayList<>();
    LogExecutor logExecutor = null;
    VoWiFiUEConfig vowifiUEConfig = null;

    Options options = new Options();

    Option argFile = new Option("f", "file", true, "File that contains queries");
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

    String queryFilePath = cmd.getOptionValue("file");
    String configFilePath = cmd.getOptionValue("config", DEFAULT_CONF_FILE);

    if (queryFilePath == null) {
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
      logExecutor = new LogExecutor(vowifiUEConfig);
    } catch (Exception e) {
      logger.error("Error happend while initializing LogExecutor");
      e.printStackTrace();
    }

    jsonParser = new JSONParser();

    logger.info("Query File Mode (File Path: " + queryFilePath + ")");
    try (FileReader reader = new FileReader(queryFilePath)) {
      JSONObject jsonObject = (JSONObject) jsonParser.parse(reader);
      JSONArray queryArray = (JSONArray) jsonObject.get("queries");
      logger.info("queries: " + queryArray);
      Iterator i = queryArray.iterator();

      while (i.hasNext()) {
        JSONObject q = (JSONObject) i.next();
        logger.info("Next Message: " + q);
        QueryString qs = new QueryString(q, logger);
        queries.add(qs);
      }
    } catch (Exception e) {
      logger.error("Error happened while processing the query file");
      e.printStackTrace();
    }

    logger.info("# of Querie Strings: " + queries.size());

    Boolean timeoutOccured = false;
    int queryNum = 1;

    for (QueryString query: queries) {
      logger.info("Starting Query #" + queryNum);

      Boolean exceptionOccured = executeQuery(logExecutor, query);

      while (exceptionOccured) {
        exceptionOccured = executeQuery(logExecutor, query);
      }

      logger.info("Finished Query #" + queryNum);

      queryNum ++;
    }
    
    try {
      //sleep(1000000);
      sleep(3000);
    } catch (Exception e) {

    }

    //killEPDG();
    try {
      sleep(3000);
    } catch (InterruptedException e) {
      e.printStackTrace();
    }
    //killIMS();
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
    initEPDGConnection();

    logger.debug("FINISH: restartIMS()");
  }

  public void sendMSGToEPDG(Query query) {
    logger.debug("START: sendMSGToEPDG()");

    /*
    String result = new String();
    try {
      msg = "----------------- " + msg + "------------------\n";
      epdgOut.write(msg);
      epdgOut.flush();
      result = epdgIn.readLine();
      System.out.println("Received for the sent message = " + result);
    } catch(SocketException e) {
      e.printStackTrace();
    } catch(IOException e) {
      e.printStackTrace();
    } catch(Exception e) {
      e.printStackTrace();
    }
    */

    logger.debug("FINISH: sendMSGToEPDG()");
  }

  public void initUEConnection() {
    logger.debug("START: initUEConnection()");

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
        sleep(2*1000);
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
    /*
    try {
      // Initialize test service
      System.out.println("Connecting to IMS (S-CSCF)...");
      imsSocket = new Socket(config.ims_controller_ip_address, config.ims_port);
      imsSocket.setTcpNoDelay(true);
      imsOut = new BufferedWriter(new OutputStreamWriter(imsSocket.getOutputStream()));
      imsIn = new BufferedReader(new InputStreamReader(imsSocket.getInputStream()));
      System.out.println("The connection with IMS (S-CSCF) is established.");

      String result = new String();
      try {
        sleep(2*1000);
        imsOut.write("Hello\n");
        imsOut.flush();
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
        logger.info("PASSED: Testing the connection between the statelearner and the ePDG");
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
    */
    logger.debug("FINISH: initIMSConnection()");
  }

	public String getExpectedResult(String symbol, String result) {
    logger.debug("START: getExpectedResult()");
		String final_result = "null_action";

    /*
		if (symbol.contains("ike_sa_init_request") 
        || symbol.contains("iks_sa_init_response")) {
		  if (ikeSaInitExpectedResults.contains(result)) {
				final_result = result;
			}
		}
    */

    logger.debug("FINISH: getExpectedResult()");
		return final_result;
	}

  public static Boolean executeQuery(LogExecutor logExecutor, QueryString query) {
    logger.debug("START: executeQuery()");
    logExecutor.pre();

    boolean timeoutOccured = false;
    boolean exceptionOccured = false;
    long startTime, endTime, duration;
    double insec;

    while (query.hasNextMessage()) {
      Query message = query.getNextMessage();
      Reply reply;
      String name = message.getName();

      if (name.contains("ε"))
        continue;

      logger.info("COMMAND: " + name);

      if (timeoutOccured) {
        logger.info("RESULT: NULL ACTION (TIMEOUT)");
        continue;
      }

      startTime = System.currentTimeMillis();
      reply = logExecutor.step(message);
      endTime = System.currentTimeMillis();
      duration = (endTime - startTime);
      insec = duration/1000.0;
      logger.info("Elapsed Time in prefix: " + duration + " ms (" + insec + " s)");

      if (reply.getName().matches("EXCEPTION")) {
        logger.info("Exception occured, restarting query");
        exceptionOccured = true;
      }

      if (reply.getName().matches("timeout")) {
        timeoutOccured = true;
        logger.info("RESULT: NULL ACTION (TIMEOUT)");
        continue;
      }

      logger.info("RESULT: " + reply.getName());
    }

    logger.debug("FINISH: executeQuery()");
    return exceptionOccured;
  }

  public boolean resetEPDG() {
    logger.debug("START: resetEPDG()");
    String result = "";

    /*
    logger.info("Sending symbol: RESET to ePDG");
    try {
      sleep(COOLING_TIME);
      epdgOut.write("RESET " + resetEPDGCount + "\n");
      epdgOut.flush();
      result = epdgIn.readLine();
      System.out.println("ACK for resetEPDG(): " + result);
      resetEPDGCount++;
    } catch(SocketException e) {
      e.printStackTrace();
    } catch(IOException e) {
      e.printStackTrace();
    } catch(Exception e) {
      e.printStackTrace();
    }
    */

    logger.debug("FINISH: resetEPDG()");
    return true;
  }

  public boolean resetIMS() {
    logger.debug("START: resetIMS()");
    String result = "";

    /*
    System.out.println("Sending symbol: RESET to IMS Server");
    try {
      sleep(1*1000);
      imsOut.write("RESET " + resetIMSCount + "\n");
      imsOut.flush();
      result = imsIn.readLine();
      logger.info("ACK for resetIMS(): " + result);
      resetIMSCount++;
    } catch(SocketException e) {
      e.printStackTrace();
    } catch(IOException e) {
      e.printStackTrace();
    } catch(Exception e) {
      e.printStackTrace();
    }
    */
    logger.debug("FINISH: resetIMS()");
    return true;
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

  public void sendEnableVoWiFi() {
    String result;
    int retry = 3;
    boolean enabled = false;
    boolean rebooted = false;

    try {
      sleep(COOLING_TIME);

			ueSocket.setSoTimeout(DEFAULT_SOCKET_TIMEOUT_VALUE);
      while (enabled == false && retry > 0) {
        logger.info("Sending symbol: reset to UE Controller to enable VoWiFi");
        enableVoWiFiCount++;
        ueOut.write("reset\n");
        ueOut.flush();

        result = ueIn.readLine();
        logger.info("UE Controller's ACK for reset: " + result);
        if (result.contains("ACK")) {
          enabled = true;
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

  private Reply processResult(Query query) {
    String result = null;
    String rstr;
    String print;
    String spi, ispi, rspi;
    byte rcvd[];
    Reply reply = null;
    Stack<Integer> stack = new Stack<Integer>();
    int idx, type, len, depth;

    depth = 0;
    try {
		  epdgSocket.setSoTimeout(EPDG_SOCKET_TIMEOUT_VALUE);
      do {
        print = "";
        for (int i=0; i<depth; i++)
        {
          print += "  ";
        }
      	result = epdgIn.readLine();
        len = result.length();
        idx = 0;
        rcvd = result.getBytes();
        type = (char) rcvd[idx++];

        logger.debug("Message type: " + type);
        switch (type) {
          case 1:
            if (reply == null) {
              logger.error("The attribute should be within the block");
            } else {
              reply = reply.addSubmessage(ReplyType.ATTRIBUTE);
            }
            break;

          case 2:
            if (reply == null) {
              reply = new Reply(query, ReplyType.MESSAGE, logger);
            } else {
              reply = reply.addSubmessage(ReplyType.PAYLOAD);
            }
            depth++;
            stack.push(1);
            break;

          case 3:
            if (reply.hasParent()) {
              reply = reply.getParent();
            }
            depth--;
            stack.pop();
            break;
        }

        len -= 1;
        ispi = result.substring(idx, idx + 16);
        idx += 16; len -= 16;
        spi = reply.getIspi();
        if (spi == null) {
          reply.setIspi(ispi);
        } else {
          if (!spi.equals(ispi)) {
            logger.error("Initiator's SPIs are different");
          }
        }

        rspi = result.substring(idx, idx + 16);
        idx += 16; len -= 16;
        spi = reply.getRspi();
        if (spi == null) {
          reply.setRspi(rspi);
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

          reply.setName(arr[0]);

          if (arr.length > 1) {
            if (arr.length != 3) {
              logger.error("The array length should be 3");
            }
            else {
              reply.setValueType(arr[1]);
              reply.setValue(arr[2]);
            }
          }
          logger.info(print);
        }
      } while (stack.size() != 0);
  	  epdgSocket.setSoTimeout(DEFAULT_SOCKET_TIMEOUT_VALUE);
		} catch (SocketTimeoutException e) {
			logger.info("Timeout occured for " + query.getName());
			handleTimeout();
			reply = new Reply(query, ReplyType.MESSAGE, logger);
      reply.setName("timeout");
    } catch (Exception e) {
			e.printStackTrace();
			logger.info("Attempting to restart device, and reset ePDG and IMS Server. Also restarting query.");
			handleEPDGIMSFailure();
			reply = new Reply(query, ReplyType.MESSAGE, logger);
      reply.setName("null_action");
		}

    return reply;
  }

	public Reply step(Query query) {
    logger.debug("START: step()");
    Reply reply;
    String qname = query.getName();
    String rname;
    
		try {
			sleep(50); //50 milliseconds
		} catch (Exception e) {
			e.printStackTrace();
		}

		if(qname.startsWith("enable_vowifi")) {
      logger.info("sendEnableVoWiFi()");
  		sendEnableVoWiFi();
    } else if(qname.startsWith("ike_sa_init_response")) {
      logger.info("sendEnableVoWiFi()");
  		sendMSGToEPDG(query);
    }
    reply = processResult(query);
    rname = reply.getName();

	  logger.info("##### " + qname + " -> " + rname + " #####");
    
    logger.debug("FINISH: step()");
		return reply;
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
          /*
					do { 
            trial++;
            ret = resetUE();
          } while (ret == false && trial < DEFAULT_NUMBER_OF_TRIALS);
          */

          if (trial == DEFAULT_NUMBER_OF_TRIALS) {
            logger.error("Resetting UE failed");
            System.exit(1);
          }

          trial = 0;
          do {
            trial++;
            ret = resetEPDG();
          } while (ret == false && trial < DEFAULT_NUMBER_OF_TRIALS);

          if (trial == DEFAULT_NUMBER_OF_TRIALS) {
            logger.error("Resetting ePDG failed");
            System.exit(1);
          }

          trial = 0;
          do {
            trial++;
            ret = resetIMS();
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

  public void handleTimeout(){
    logger.debug("START: handleTimeout()");
    
    String result = new String("");
    if (isEPDGAlive() == false || isIMSAlive() == false) {
      handleEPDGIMSFailure();
      return;
    }
    try {
      ueOut.write("ue_reboot\n");
      ueOut.flush();
      logger.info("Sleeping while UE reboots");
      sleep(UE_REBOOT_SLEEP_TIME);
      result = ueIn.readLine();
      logger.info("Result for reboot: " + result);
    } catch (IOException e) {
      e.printStackTrace();
    } catch (InterruptedException e) {
      e.printStackTrace();
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
      rebootUE();
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

    String imsStartCmd = config.getIMSStartCmd();
    logger.info("start IMS Server by " + imsStartCmd);
    runProcess(imsStartCmd);

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
