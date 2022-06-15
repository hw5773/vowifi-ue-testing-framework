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

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.commons.cli.*;

import static java.lang.Thread.sleep;

public class LogExecutor {
  private static VoWiFiUEConfig config = null;

  final static Log logger = LogFactory.getLog(LogExecutor.class);

  public Socket ueSocket, epdgSocket, imsSocket;

  public BufferedWriter ueOut, epdgOut, imsOut;
  public BufferedReader ueIn, epdgIn, imsIn;

  int rebootCount = 0;
  int enableVoWiFiCount = 0;
  int resetEPDGCount = 0;
  int resetIMSCount = 0;

  private static final String[] OS_LINUX_RUNTIME = { "/bin/bash", "-l", "-c" };

  private static final List<String> expectedResults = Arrays.asList(
    "ike_sa_init_request",
    "ike_sa_init_response",
    "null_action",
    "DONE");

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

  public static void main(String[] args) {
    List<List<String>> queries = new ArrayList<>();
    LogExecutor logExecutor = null;
    VoWiFiUEConfig vowifiUEConfig = null;

    Options options = new Options();

    Option argFile = new Option("f", "file", true, "File that contains queries");
    options.addOption(argFile);

    Option argQuery = new Option("q", "query", true, "Query to be executed by LogExecutor");
    options.addOption(argQuery);

    Option argConfig = new Option("c", "config", true, "Configuration file");
    options.addOption(argConfig);

    CommandLineParser parser = new DefaultParser();
    HelpFormatter formatter = new HelpFormatter();
    CommandLine cmd = null;

    try {
      cmd = parser.parse(options, args);
    } catch (ParseException e) {
      logger.error(e.getMessage());
      formatter.printHelp("Options", options);

      System.exit(1);
    }

    String queryFilePath = cmd.getOptionValue("file");
    String queryString = cmd.getOptionValue("query");
    String configFilePath = cmd.getOptionValue("config", DEFAULT_CONF_FILE);

    if (queryFilePath == null && queryString == null) {
      logger.error("Query File or Query should be inserted");
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

    if (queryFilePath != null) {
      logger.info("Query File Mode (File Path: " + queryFilePath + ")");
      try (BufferedReader br = new BufferedReader(new FileReader(queryFilePath))) {
        String line;
          
        while ((line = br.readLine()) != null) {
          if (line.contains("INFO")) {
            line = line.split("/")[0].split("\\[")[1].replaceAll("\\|", " ");
            logger.debug(line);
            List<String> splitLine = Arrays.asList(line.split("\\s+"));
            for (int i=0; i<splitLine.size(); i++){
              logger.debug(splitLine.get(i));
            }
            queries.add(splitLine);
          }
        }
      } catch (Exception e) {
        logger.error("Error happened while processing the query file");
        e.printStackTrace();
      }

      logger.info("# of Queries: " + queries.size());
    }
    else if (queryString != null) {
      logger.info("Query String Mode");
      logger.info("Query: " + queryString);

      try {
        List<String> splitLine = Arrays.asList(queryString.split("\\s+"));
        queries.add(splitLine);
      } catch (Exception e) {
        logger.error("Error happened while processing the query string");
        e.printStackTrace();
      }
    }

    Boolean timeoutOccured = false;
    Integer queryNum = 1;

    for (List<String> query: queries) {
      String msg = "Starting Query # " + Integer.toString(queryNum);
      logger.info("Starting Query # " + Integer.toString(queryNum));

      Boolean exceptionOccured = executeQuery(logExecutor, query);

      while (exceptionOccured) {
        exceptionOccured = executeQuery(logExecutor, query);
      }

      msg = "Finished Query # " + Integer.toString(queryNum);
      logger.info("Finished Query # " + Integer.toString(queryNum));

      queryNum ++;
    }

    try {
      //sleep(1000000);
      sleep(3000);
    } catch (Exception e) {

    }

    killEPDG();
    try {
      sleep(3000);
    } catch (InterruptedException e) {
      e.printStackTrace();
    }
    killIMS();
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

  public void sendMSGToEPDG(String msg) {
    logger.debug("START: sendMSGToEPDG()");

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
        System.out.println("PASSED: Testing the connection between the statelearner and the ePDG");
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
        System.out.println("PASSED: Testing the connection between the statelearner and the ePDG");
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

  public static Boolean executeQuery(LogExecutor logExecutor, List<String> query) {
    logger.debug("START: executeQuery()");
    logExecutor.pre();

    boolean timeoutOccured = false;
    boolean exceptionOccured = false;
    long startTime, endTime, duration;
    double insec;

    for (String command: query) {
      if (command.contains("ε"))
        continue;
      logger.info("COMMAND: " + command);

      if (timeoutOccured) {
        logger.info("RESULT: NULL ACTION (TIMEOUT)");
        continue;
      }

      command = command.replaceAll("\\s+","");
      startTime = System.currentTimeMillis();
      String result = logExecutor.step(command);
      endTime = System.currentTimeMillis();
      duration = (endTime - startTime);
      insec = duration/1000.0;
      logger.info("Elapsed Time in prefix: " + duration + " ms (" + insec + " s)");

      if (result.matches("EXCEPTION")) {
        logger.info("Exception occured, restarting query");
        exceptionOccured = true;
      }

      if (result.matches("timeout")) {
        timeoutOccured = true;
        logger.info("RESULT: NULL ACTION (TIMEOUT)");
        continue;
      }

      logger.info("RESULT: " + result);
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
      ueOut.write("RESET\n");
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
    
    if(result.contains("ACK\n")) {
      System.out.println("PASSED: Testing the connection between the statelearner and UE");
      logger.debug("FINISH: resetUE()");
      return true;
    } else {
      System.out.println("FAILED: Testing the connection between the statelearner and UE");
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
    
    if(result.contains("ACK\n")) {
      System.out.println("PASSED: Testing the connection between the statelearner and UE");
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
    int retry = 5;
    boolean enabled = false;

    try {
      sleep(COOLING_TIME);

      while (enabled == false && retry > 0) {
        logger.info("Sending symbol: enable_vowifi to UE Controller");
        enableVoWiFiCount++;
        ueOut.write("enable_vowifi\n");
        ueOut.flush();

        result = ueIn.readLine();
        logger.info("UE Controller's ACK for enable_vowifi: " + result);
        if (!result.contains("DONE")) {
          enabled = true;
        }
        retry--;
      }

      if (enabled == false)
      {
        logger.error("Failed to start the VoWiFi protocol");
        System.exit(1);
      }
    } catch (SocketException e) {
      e.printStackTrace();
    } catch (IOException e) {
      e.printStackTrace();
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

	public String step(String symbol) {
    logger.debug("START: step()");
    String result = "";
    
		try {
			sleep(50); //50 milliseconds
		} catch (Exception e) {
			e.printStackTrace();
		}

		try {
			if(symbol.startsWith("enable_vowifi")) {
				while (!result.contains("ike_sa_init_request")) {
					epdgSocket.setSoTimeout(EPDG_SOCKET_TIMEOUT_VALUE);
					sendEnableVoWiFi();
					result = epdgIn.readLine();
					if (result.compareTo("") != 0 && result.toCharArray()[0] == ' ') {
						result = new String(Arrays.copyOfRange(result.getBytes(), 1, result.getBytes().length));
					}
  				epdgSocket.setSoTimeout(DEFAULT_SOCKET_TIMEOUT_VALUE);
	  			logger.info(symbol + "->" + result);
		  		return result;
			  }
      }
		} catch (SocketTimeoutException e) {
			logger.info("Timeout occured for " + symbol);
			handleTimeout();
			return "timeout";
		} catch (Exception e) {
			e.printStackTrace();
			logger.info("Attempting to restart device, and reset ePDG and IMS Server. Also restarting query.");
			handleEPDGIMSFailure();
			return "null_action";
		}

		try {
			if (symbol.contains("reject")) {
				epdgSocket.setSoTimeout(DEFAULT_SOCKET_TIMEOUT_VALUE);
				epdgOut.write(symbol + "\n");
				epdgOut.flush();

				result = epdgIn.readLine();
				if(result.compareTo("") != 0 && result.toCharArray()[0] == ' ') {
					result = new String(Arrays.copyOfRange(result.getBytes(), 1, result.getBytes().length));
				}

				logger.info(symbol + "->" + result);
				return result;
			}
		} catch (SocketTimeoutException e){
			logger.info("Timeout occured for " + symbol);
			logger.info("Restarting UE and marking following command as null action");
			handleTimeout();
			return "timeout";
		} catch (Exception e) {
			e.printStackTrace();
			logger.info("Attempting to restart UE and reset ePDG and IMS. Also restarting query.");
			handleEPDGIMSFailure();
			return "null_action";
		}

		try {
			if (result.compareTo("") != 0 && result.toCharArray()[0] == ' ') {
				if (result.toLowerCase().startsWith("null_action")) {
					result = "null_action";
				}
				if (result.toLowerCase().startsWith("detach_request")) {
					result = "null_action";
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("Attempting to restart UE and reset ePDG and IMS. Also restarting query.");
			handleEPDGIMSFailure();

			return "null_action";
		}

		System.out.println("####" + symbol +"/"+result + "####");
    
    logger.debug("FINISH: step()");
		return result;
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
					do { 
            trial++;
            ret = resetUE();
          } while (ret == false && trial < DEFAULT_NUMBER_OF_TRIALS);

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
      System.out.println("PASSED: Testing the connection between the statelearner and ePDG");
      return true;
    } else {
      System.out.println("FAILED: Testing the connection between the statelearner and ePDG");
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
      System.out.println("PASSED: Testing the connection between the statelearner and the srsEPC");
      return true;
    } else {
      System.out.println("FAILED: Testing the connection between the statelearner and the srsEPC");
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
