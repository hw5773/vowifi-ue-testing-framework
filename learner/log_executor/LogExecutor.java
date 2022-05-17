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

import static java.lang.Thread.sleep;

public class LogExecutor {
  VoWiFiUEConfig config;

  public Socket ueSocket, epdgSocket, imsSocket;

  public BufferedWriter ueOut, epdgOut, imsOut;
  public BufferedReader ueIn, epdgIn, imsIn;

  int rebootCount = 0;
  boolean sqnSynchronized = false;

  private static final String[] OS_LINUX_RUNTIME = { "/bin/bash", "-l", "-c" };

  private static final List<String> expectedResults = Arrays.asList(
    "ike_sa_init_request",
    "ike_sa_init_response",
    "null_action",
    "DONE");

  private static final int COOLING_TIME = 5*1000; // 5 seconds
  private static final String DEFAULT_CONF_FILE = "vowifi-ue.properties";

  public LogExecutor(VoWiFiUEConfig config) throws Exception {
    this.config = config;

    initUEConnection();
    initEPDGConnection();
    initIMSConnection();
  }

  public static void main(String[] args) {
    List<List<String>> queries = new ArrayList<>();

    if (args.length <= 1) {
      System.out.println("Invalid command line arguments");
      System.out.println("< -f / -q > <log file / query> -i <vowifi-ue.properties>");
      return;
    }

    if (args[0].contains("-f")) {
      System.out.println("file");

      if(args.length > 0) {
        String file_name = args[1];
        try (BufferedReader br = new BufferedReader(new FileReader(file_name))) {
          String line;
          
          while ((line = br.readLine()) != null) {
            if (line.contains("INFO")) {
              line = line.split("/")[0].split("\\[")[1].replaceAll("\\|", " ");
              System.out.println(line);
              List<String> splitLine = Arrays.asList(line.split("\\s+"));
              for (int i=0; i<splitLine.size(); i++){
                System.out.println(splitLine.get(i));
              }
              queries.add(splitLine);
            }
          }
        } catch (Exception e) {
          e.printStackTrace();
        }
      }

      System.out.println(queries.size());

      VoWiFiUEConfig vowifiUEConfig = null;
      LogExecutor logExecutor = null;

      if(args.length >= 3  && args[2].contains("-i")){
        String vowifiUEPropertiesFilename = args[3];

        try {
          vowifiUEConfig = new VoWiFiUEConfig(vowifiUEPropertiesFilename);
        } catch (IOException e) {
          e.printStackTrace();
          return;
        }
      }
      else{
        try {
          vowifiUEConfig = new VoWiFiUEConfig(DEFAULT_CONF_FILE);
        } catch (IOException e) {
          e.printStackTrace();
          return;
        }
      }

      try {
        logExecutor = new LogExecutor(vowifiUEConfig);
      } catch (Exception e) {
        e.printStackTrace();
        return;
      }

      Boolean timeoutOccured = false;
      Integer queryNum = 1;

      for (List<String> query: queries) {
        String msg = "Starting Query # " + Integer.toString(queryNum);
        System.out.println("Starting Query # " + Integer.toString(queryNum));

        Boolean exceptionOccured = executeQuery(logExecutor, query);

        while (exceptionOccured) {
          exceptionOccured = executeQuery(logExecutor, query);
        }

        msg = "Finished Query # " + Integer.toString(queryNum);
        System.out.println("Finished Query # " + Integer.toString(queryNum));

        queryNum ++;
      }
    } else if (args[0].contains("-q")) {
      System.out.println("query");
      System.out.println(args[1]);
      String line = args[1];
      System.out.println("query: " + line);
      List<String> splitLine = Arrays.asList(line.split("\\s+"));

      for(String word: splitLine) {
        System.out.println(word);
      }

      VoWiFiUEConfig vowifiUEConfig = null;
      LogExecutor logExecutor = null;

      try {
        vowifiUEConfig = new VoWiFiUEConfig("vowifi-ue.properties");
      } catch (IOException e) {
        e.printStackTrace();
        return;
      }

      try {
        logExecutor = new LogExecutor(vowifiUEConfig);
      } catch (Exception e) {
        e.printStackTrace();
        return;
      }

      Boolean timeoutOccured = false;

      logExecutor.pre();
      for (String command: splitLine) {
        if (command.contains("ε"))
          continue;

        if (timeoutOccured) {
          System.out.println("RESULT: NULL ACTION");
          continue;
        }

        String result = logExecutor.step(command);

        if (result.matches("timeout")){
          timeoutOccured = true;
          System.out.println("RESULT: NULL ACTION(TIMEOUT)");
          continue;
        }

      System.out.println("RESULT: " + result);
      }
    } else {
      System.out.println("Invalid command line arguments");
    }
    try {
      sleep(1000000);
    } catch (Exception e) {

    }

    killEPDG();
    try {
      sleep(2000);
    } catch (InterruptedException e) {
      e.printStackTrace();
    }
    killIMS();
  }

  public void restartEPDG() {
    System.out.println("START: restartEPDG()");
    /*
    try {
      epdgOut.close();
      epdgIn.close();
      epdgSocket.close();

      sleep(1000);

      killEPDG();
      startEPDG();

      epdgSocket = new Socket(config.epdg_controller_ip_address, config.epdg_port);
      epdgSocket.setTcpNoDelay(true);
      epdgOut = new BufferedWriter(new OutputStreamWriter(epdgSocket.getOutputStream()));
      epdgIn = new BufferedReader(new InputStreamReader(mme_socket.getInputStream()));

      sqnSynchronized = false;
    } catch (UnknownHostException e){
      e.printStackTrace();
    } catch (SocketException e){
      e.printStackTrace();
    } catch (Exception e){
      e.printStackTrace();
    }
    */
    System.out.println("FINISH: restartEPDG()");
  }

  public void restartIMS() {
    System.out.println("START: restartIMS()");

    System.out.println("FINISH: restartIMS()");
  }

  public void sendMSGToEPDG(String msg) {
    System.out.println("START: sendMSGToEPDG()");
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
    System.out.println("FINISH: sendMSGToEPDG()");
  }

  public void initUEConnection() {
    System.out.println("START: initUEConnection()");
    /*
    try {
      System.out.println("Connecting to UE...");
      System.out.println("UE controller IP Address: " + config.ue_controller_ip_address);
      ueSocket = new Socket(config.ue_controller_ip_address, config.ue_port);
      ueSocket.setTcpNoDelay(true);
      System.out.println("Connected to UE");

      System.out.println("Initializing Buffers for UE...");
      ueOut = new BufferedWriter(new OutputStreamWriter(ueSocket.getOutputStream()));
      ueIn = new BufferedReader(new InputStreamReader(ueSocket.getInputStream()));
      System.out.println("Initialized Buffers for UE");
    } catch (UnknownHostException e) {
      e.printStackTrace();
    } catch (SocketException e) {
      e.printStackTrace();
    } catch (Exception e) {
      e.printStackTrace();
    }
    */
    System.out.println("FINISH: initUEConnection()");
  }

  public void initEPDGConnection() {
    System.out.println("START: initEPDGConnection()");
    /*
    try {
      // Initialize test service
      System.out.println("Connecting to ePDG...");
      epdgSocket = new Socket(config.epdg_controller_ip_address, config.epdg_port);
      epdgSocket.setTcpNoDelay(true);
      epdgOut = new BufferedWriter(new OutputStreamWriter(epdgSocket.getOutputStream()));
      epdgIn = new BufferedReader(new InputStreamReader(epdgSocket.getInputStream()));
      System.out.println("The TCP connection with ePDG is established.");

      String result = new String();
      try {
        sleep(2*1000);
        epdgOut.write("Hello\n");
        epdgOut.flush();
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
    */
    System.out.println("FINISH: initEPDGConnection()");
  }

  public void initIMSConnection() {
    System.out.println("START: initIMSConnection()");
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
    System.out.println("FINISH: initIMSConnection()");
  }

	public String getExpectedResult(String symbol, String result) {
    System.out.println("START: getExpectedResult()");
		String final_result = "null_action";

    /*
		if (symbol.contains("ike_sa_init_request") 
        || symbol.contains("iks_sa_init_response")) {
		  if (ikeSaInitExpectedResults.contains(result)) {
				final_result = result;
			}
		}
    */

    System.out.println("FINISH: getExpectedResult()");
		return final_result;
	}

  public static Boolean executeQuery(LogExecutor logExecutor, List<String> query) {
    System.out.println("START: executeQuery()");
    logExecutor.pre();

    boolean timeoutOccured = false;
    boolean exceptionOccured = false;

    /*
    for (String command: query) {
      if (command.contains("ε"))
        continue;
      System.out.println("COMMAND: " + command);

      if (timeoutOccured) {
        System.out.println("RESULT: NULL ACTION");
        continue;
      }

      command = command.replaceAll("\\s+","");
      long startTime = System.currentTimeMillis();
      String result = logExecutor.step(command);
      long endTime = System.currentTimeMillis();
      long duration = (endTime - startTime);
      double insec = duration/1000.0;
      System.out.println("time elapsed in prefix: " + duration + " " + insec);

      if (result.matches("EXCEPTION")) {
        System.out.println("Exception occured, restarting query");
        exceptionOccured = true;
      }

      if (result.matches("timeout")) {
        timeoutOccured = true;
        System.out.println("RESULT: NULL ACTION(TIMEOUT)");
        continue;
      }

      System.out.println("RESULT: " + result);
    }
    */

    System.out.println("FINISH: executeQuery()");
    return exceptionOccured;
  }

  public String resetEPDG() {
    System.out.println("START: resetEPDG()");
    String result = new String("");
    /*
    System.out.println("Sending symbol: RESET to MME controller");
    try {
      sleep(1*1000);
      ePDGOut.write("RESET " + resetEPDGCount + "\n");
      ePDGOut.flush();
      result = ePDGIn.readLine();
      System.out.println("ACK for RESET_MME: " + result);
      resetEPDGCount++;
    } catch(SocketException e) {
      e.printStackTrace();
    } catch(IOException e) {
      e.printStackTrace();
    } catch(Exception e) {
      e.printStackTrace();
    }
    */
    System.out.println("FINISH: resetEPDG()");
    return result;
  }

  public String resetUE() {
    System.out.println("START: resetUE()");
    String result = new String("");
    /*
    System.out.println("Sending symbol: RESET to UE controller");
    try {
      sleep(1 * 1000);
      ueOut.write("RESET\n");
      ueOut.flush();
      result = ueIn.readLine();
      System.out.println("ACK for RESET_UE: " + result);
    } catch(SocketException e) {
      e.printStackTrace();
    } catch(IOException e) {
      e.printStackTrace();
    } catch(Exception e) {
      e.printStackTrace();
    }
    */
    System.out.println("FINISH: resetUE()");
    return result;
  }

  public String rebootUE() {
    System.out.println("START: rebootUE()");
    String result = new String("");
    /*
    try {
      ueOut.write("ue_reboot\n");
      ueOut.flush();
      System.out.println("Waiting for the response from UE .... ");
      result = ueIn.readLine();
      System.out.println("UE's ACK for REBOOT: " + result);
    } catch (SocketException e) {
      e.printStackTrace();
    } catch(IOException e) {
      e.printStackTrace();
    } catch(Exception e) {
      e.printStackTrace();
    }
    */
    System.out.println("FINISH: rebootUE()");
    return result;
  }

  public String restartUEAdbServer() {
    System.out.println("START: restartUEAdbServer()");
    String result = new String("");
    /*
    try {
      //sleep(2000);
      ueOut.write("adb_server_restart\n");
      ueOut.flush();
      result = ueIn.readLine();
      System.out.println("Result for adb_server_restart: " + result);
    } catch (SocketException e) {
      e.printStackTrace();
    } catch(IOException e) {
      e.printStackTrace();
    } catch(Exception e) {
      e.printStackTrace();
    }
    */
    System.out.println("FINISH: restartUEAdbServer()");
    return result;
  }

	public String step(String symbol) {
    System.out.println("START: step()");
    String result = "";
    /*
		try {
			sleep(50); //50 milliseconds
		} catch (Exception e) {
			e.printStackTrace();
		}

		String attachResult = "";
		try {
			if(symbol.startsWith("enable_vowifi")) {
				while (!result.contains("ike_sa_init_request")) {
					ePDGSocket.setSoTimeout(180*1000);
					send_enable_s1();
					result = ePDGIn.readLine();
					if (result.compareTo("") != 0 && result.toCharArray()[0] == ' ') {
						result = new String(Arrays.copyOfRange(result.getBytes(), 1, result.getBytes().length));
					}
					result = getClosests(result);
  				ePDGSocket.setSoTimeout(5*1000);
	  			System.out.println(symbol + "->" + result);
		  		return result;
			  }
      }
		} catch (SocketTimeoutException e) {
			System.out.println("Timeout occured for " + symbol);
			handleTimeout();
			return "timeout";
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("Attempting to restart device and reset srsEPC. Also restarting query.");
			handle_enb_epc_failure();
			return "null_action";
		}

		try {
			if (symbol.contains("reject")) {
				ePDGSocket.setSoTimeout(5*1000);
				ePDGOut.write(symbol + "\n");
				ePDGOut.flush();

				result = ePDGIn.readLine();
				if(result.compareTo("") != 0 && result.toCharArray()[0] == ' ') {
					result = new String(Arrays.copyOfRange(result.getBytes(), 1, result.getBytes().length));
				}
				result = getClosests(result);

				System.out.println(symbol + "->" + result);
				return result;
			}
		} catch (SocketTimeoutException e){
			System.out.println("Timeout occured for " + symbol);
			System.out.println("Restarting UE and marking following command as null action");
			handleTimeout();
			return "timeout";
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("Attempting to restart UE and reset ePDG and IMS. Also restarting query.");
			handleEPDGFailure();
			handleIMSFailure();
			return "null_action";
		}

		try {
			if (result.compareTo("") != 0 && result.toCharArray()[0] == ' ') {
				result = new String(Arrays.copyOfRange(result.getBytes(), 1, result.getBytes().length));
				result = getClosests(result);
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
			handleEPDGFailure();
			handleIMSFailure();

			return "null_action";
		}

		System.out.println("####" + symbol +"/"+result + "####");
    */
    System.out.println("FINISH: step()");
		return result;
	}

  public void pre() {
    System.out.println("START: pre()");
    /*
	  int flag = 0;
		try {
			if (!config.combine_query) {

				String result = new String("");
				String resultForUE = new String("");
				String resultForEPDG = new String("");
				String resultForIMS = new String("");
				String attachResult = new String ("");
				boolean resetDone = false;

				timeoutCount = 0;
				rebootCount = 0;

				System.out.println("---- Starting RESET ----");

				do {
					try {
						resultForUE = resetUE();
						resultForEPDG = resetEPDG();
						resultForIMS = resetIMS();
						result = new String("");
						attachResult = new String("");

						ePDGSocket.setSoTimeout(180*1000);
						if (flag == 0){
							flag = 1;
						}
						sendEnableVoWiFi();

						result = ePDGIn.readLine();
						if (result == null || result.compareTo("")==0){
							continue;
						}
						result = new String(Arrays.copyOfRange(result.getBytes(), 1, result.getBytes().length));
						result = getClosests(result);

						if(result.contains("detach_request")){
							mme_socket.setSoTimeout(20*1000);
							attach_result = mme_in.readLine();
							if (attach_result.compareTo("") != 0 && attach_result.toCharArray()[0] == ' ') {
								attach_result = new String(Arrays.copyOfRange(attach_result.getBytes(), 1, attach_result.getBytes().length));
							}
							attach_result = getClosests(attach_result);
							//result = result + ","+ attach_result;
							result = attach_result;
							//System.out.println("Done by IK!");
							//mme_out.write("attach_reject\n");
							//mme_out.flush();
						}
						System.out.println("Response of ENABLE_S1: " + result);
						mme_socket.setSoTimeout(30*1000);
						int attach_request_guti_counter = 3;

						if (result.contains("attach_request_guti") 
                || result.contains("service_request") 
                || result.contains("tau_request")) {
							attach_request_guti_count++;
							flag = 1;

							if(attach_request_guti_count < attach_request_guti_counter) {
								System.out.println("Sending symbol: attach_reject to MME controller to delete the UE context in attach_request_guti");
								mme_out.write("attach_reject\n");
								mme_out.flush();
							}

							else if(attach_request_guti_count% attach_request_guti_counter == 0){
								handle_enb_epc_failure();
							}
							else if(attach_request_guti_count > attach_request_guti_counter){
								System.out.println("Sending symbol: auth_reject to MME controller to delete the UE context");
								mme_out.write("auth_reject\n");
								mme_out.flush();
								TimeUnit.SECONDS.sleep(2);
								reboot_ue();
							}
						}
						else if (result.startsWith("attach_request") 
                || result.startsWith("DONE")) {
							if (flag == 0){
								flag = 1;
								continue;
							}
							attach_request_guti_count = 0;

							if(sqn_synchronized == false){
								//send_enable_s1();
								//result = mme_in.readLine();
								//result = new String(Arrays.copyOfRange(result.getBytes(), 1, result.getBytes().length));
								//result = getClosests(result);
								System.out.println("Sending symbol: auth_request to MME controller");
								mme_out.write("auth_request_plain\n");
								mme_out.flush();

								result = mme_in.readLine();
								result = new String(Arrays.copyOfRange(result.getBytes(), 1, result.getBytes().length));
								result = getClosests(result);

								System.out.println("RESULT FROM AUTH REQUEST: " + result);

								if (result.contains("auth")) {
									System.out.println("Received " + result + ". Synched the SQN value");
									sqn_synchronized = true;
									reset_done = true;
									break;
								}
							}
							else if(sqn_synchronized == true){
								reset_done = true;
								break;
							}
						}
					} catch (SocketTimeoutException e) {
						enable_s1_timeout_count++;
						System.out.println("Timeout occured for enable_s1");
						System.out.println("Sleeping for a while...");
						sleep(10*1000);
						//System.out.println("Rebooting ADB Server");
						if(enable_s1_timeout_count ==3) {
							//System.out.println("Rebooting UE");
							handle_enb_epc_failure();
						}

					}
				}while(reset_done == false);

				result = reset_mme();
				if (result.contains("attach_request_guti")){
					System.out.println("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
					System.out.println("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
					System.out.println("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
					System.out.println("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
				}
				result = reset_ue();
				//sleep(2000);
				System.out.println("---- RESET DONE ----");

			}
		} catch (Exception e) {
			e.printStackTrace();
			throw new RuntimeException(e);
		}
    */
    System.out.println("FINISH: pre()");
	}

  public void handleTimeout(){
    System.out.println("START: handleTimeout()");
    /*
    String result = new String("");
    if (enb_alive() == false || mme_alive() == false) {
      handle_enb_epc_failure();
      return;
    }
    try {
      ueOut.write("ue_reboot\n"); // reboot the UE and turn cellular network ON with 4G LTE
      ueOut.flush();
      System.out.println("Sleeping while UE reboots");
      TimeUnit.SECONDS.sleep(45);
      result = ue_in.readLine();
      System.out.println("Result for reboot: " + result);
    } catch (IOException e) {
      e.printStackTrace();
    } catch (InterruptedException e) {
      e.printStackTrace();
    }
    */
    System.out.println("FINISH: handleTimeout()");
  }

  public void isAdbServerRestartRequired() {
    System.out.println("START: isAdbServerRestartRequired()");
    /*
    if(enable_s1_count >= 50){
      //ue_socket.setSoTimeout(30*1000);
      enable_s1_count = 0;
    }
    */
  }

  public void handleEPDGIMSFailure() {
    System.out.println("START: handleEPDGIMSFailure()");

    try {
      rebootUE();
      restartEPDG();
      restartIMS();
    } catch (Exception e) {
      e.printStackTrace();
    }
    System.out.println("FINISH: handleEPDGIMSFailure()");
  }

  public boolean isEPDGAlive() {
    System.out.println("START: isEPDGAlive()");
    /*
    try {
      enodeb_socket.setSoTimeout(5*1000);
      enodeb_out.write("Hello\n");
      enodeb_out.flush();
      result = enodeb_in.readLine();
      System.out.println("Received from Hello message in enb alive = " + result);
      enodeb_socket.setSoTimeout(30*1000);
    } catch(SocketTimeoutException e) {
      e.printStackTrace();
      return false;
    } catch (Exception e) {
      e.printStackTrace();
      return false;
    }

    if(result.contains("ACK")) {
      System.out.println("PASSED: Testing the connection between the statelearner and the srsENB");
      return true;
    } else {
      System.out.println("FAILED: Testing the connection between the statelearner and the srsENB");
      return false;
    }
    */
    System.out.println("FINISH: isEPDGAlive()");
    return true;
  }

  public boolean isIMSAlive() {
    System.out.println("START: isIMSAlive()");

    /*
    try {
      mme_socket.setSoTimeout(5*1000);
      mme_out.write("Hello\n");
      mme_out.flush();
      result = mme_in.readLine();
      System.out.println("Received from Hello message in mme alive = " + result);
      mme_socket.setSoTimeout(30*1000);

    } catch(SocketTimeoutException e) {
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
    System.out.println("FINISH: isIMSAlive()");
    return true;
  }

  public static void startEPDG() {
    System.out.println("START: startEPDG()");
    //runProcess(false, "/home/rafiul/my-computer/research/lte/code/LTEUE-State-Fuzzing/logExecutor/src/start_enb.sh");
    System.out.println("FINISH: startEPDG()");
  }

  public static void startIMS() {
    System.out.println("START: startIMS()");
    System.out.println("TODO: Needs to implement startIMS()");
    System.out.println("FINISH: startIMS()");
  }

  public static void killEPDG() {
    System.out.println("START: killEPDG()");
    //runProcess(false, "/home/rafiul/my-computer/research/lte/code/LTEUE-State-Fuzzing/logExecutor/src/kill_enb.sh");
    System.out.println("FINISH: killEPDG()");
  }

  public static void killIMS() {
    System.out.println("START: killIMS()");
    System.out.println("TODO: Needs to implement killIMS()");
    System.out.println("FINISH: killIMS()");
  }

  public static void runProcess(boolean isWin, String... command) {
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
      System.out.println("ERROR: " + command + " is not running after invoking script");
      System.out.println("Attempting again...");
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
