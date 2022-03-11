
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

public class Log_executor {
    LTEUEConfig config;

    public Socket mme_socket, enodeb_socket, ue_socket;

    public BufferedWriter mme_out, enodeb_out, ue_out;
    public BufferedReader mme_in, enodeb_in, ue_in;

    int reboot_count = 0;
    int enable_s1_count = 0;
    int attach_request_guti_count = 0;
    int enable_s1_timeout_count = 0;
    int reset_mme_count = 0;
    boolean sqn_synchronized = false;

    private static final String[] WIN_RUNTIME = { "cmd.exe", "/C" };
    private static final String[] OS_LINUX_RUNTIME = { "/bin/bash", "-l", "-c" };

    /*
    private static final String[] expectedResults = {"attach_request",
            "attach_request_guti",
            "detach_request",
            "auth_response_rejected",
            "auth_response",
            "security_mode_complete",
            "security_mode_reject",
            "emm_status",
            "attach_complete",
            "identity_response",
            "auth_failure_mac",
            "auth_failure_seq",
            "auth_failure_noneps",
            "tau_request",
            "service_request",
            "tau_complete",
            "ul_nas_transport",
            "null_action",
            "GUTI_reallocation_complete",
            "DONE"};
    */
    private static final List<String> expectedResults = Arrays.asList(
            "attach_request",
            "attach_request_guti",
            "detach_request",
            "auth_response_rejected",
            "auth_response",
            "security_mode_complete",
            "security_mode_reject",
            "emm_status",
            "attach_complete",
            "identity_response",
            "auth_failure_mac",
            "auth_failure_seq",
            "auth_failure_noneps",
            "tau_request",
            "service_request",
            "tau_complete",
            "ul_nas_transport",
            "null_action",
            "GUTI_reallocation_complete",
            "identity_response_protected",
            "auth_response_protected",
            "DONE");

   	//alphabet = enable_s1 identity_request_plain identity_request_protected auth_request_plain auth_request_protected security_mode_command attach_accept tau_accept GUTI_reallocation dl_nas_transport rrc_release_tau paging_tmsi
	private static final List<String> enables1_expectedResults = Arrays.asList("attach_request","attach_request_guti", "null_action");
	private static final List<String> identityrequestplain_expectedResults = Arrays.asList("identity_response", "null_action");
	private static final List<String> identityrequestprotected_expectedResults = Arrays.asList("identity_response_protected", "null_action");
	private static final List<String> authrequestplain_expectedResults = Arrays.asList("auth_response", "auth_failure_mac", "auth_failure_seq", "auth_failure_noneps", "null_action");
	private static final List<String> authrequestprotected_expectedResults = Arrays.asList("auth_response_protected", "auth_failure_mac", "auth_failure_seq", "auth_failure_noneps", "null_action");
	private static final List<String> secmodcmd_expectedResults = Arrays.asList("security_mode_complete", "security_mode_reject", "null_action");
	private static final List<String> attachaccept_expectedResults = Arrays.asList("attach_complete", "null_action");
	private static final List<String> rrcreleasetau_expectedResults = Arrays.asList("tau_request", "null_action");
	private static final List<String> tauaccept_expectedResults = Arrays.asList("tau_complete", "null_action");
	private static final List<String> gutireallocation_expectedResults = Arrays.asList("GUTI_reallocation_complete", "null_action");
	private static final List<String> dlnastransport_expectedResults = Arrays.asList("ul_nas_transport", "null_action");
	private static final List<String> pagingtmsi_expectedResults = Arrays.asList("service_request", "null_action");



    private static final int WAIT_BEFORE_ENABLE_S1 = 5*1000; // 5 seconds

    public Log_executor(LTEUEConfig config) throws Exception {
        this.config = config;

        //start_epc_enb();
        init_epc_enb_con();
        init_ue_con();
    }

    public static void main(String[] args){
        List<List<String>> queries = new ArrayList<>();

        if (args.length <= 1) {
            System.out.println("Invalid command line arguments");
            System.out.println("< -f / -q > <log file / query> -i <lteue.properties>");
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
                            //System.out.println(line);
                            List<String> split_line = Arrays.asList(line.split("\\s+"));
                            /*
                            for (int i=0; i<split_line.size(); i++){
                                System.out.println(split_line.get(i));
                            }
                            \*/
                            queries.add(split_line);
                        }
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }

            System.out.println(queries.size());

            LTEUEConfig lteueConfig = null;
            Log_executor log_executor = null;

            if(args.length >= 3  && args[2].contains("-i")){
                String lteue_properties_filename = args[3];

                try {
                    lteueConfig = new LTEUEConfig(lteue_properties_filename);
                } catch (IOException e) {
                    e.printStackTrace();
                    return;
                }
            }
            else{
                try {
                    lteueConfig = new LTEUEConfig("lteue.properties");
                } catch (IOException e) {
                    e.printStackTrace();
                    return;
                }

            }



            try {
                log_executor = new Log_executor(lteueConfig);
            } catch (Exception e) {
                e.printStackTrace();
                return;
            }

            Boolean time_out_occured = false;

            Integer querry_num = 1;

            for (List<String> query: queries){

                String msg = "Starting Query # " + Integer.toString(querry_num);
                //log_executor.send_msg_to_epc(msg);
                System.out.println("Starting Query # " + Integer.toString(querry_num));

                Boolean exception_occured = execute_query(log_executor, query);

                while (exception_occured) {
                    exception_occured = execute_query(log_executor, query);
                }

                msg = "Finished Query # " + Integer.toString(querry_num);
                //log_executor.send_msg_to_epc(msg);
                System.out.println("Finished Query # " + Integer.toString(querry_num));


                querry_num ++;
            }

        } else if (args[0].contains("-q")) {
            System.out.println("query");
            System.out.println(args[1]);
            String line = args[1];
            System.out.println("query: " + line);
            List<String> split_line = Arrays.asList(line.split("\\s+"));

            for(String word: split_line){
                System.out.println(word);
            }


            LTEUEConfig lteueConfig = null;
            Log_executor log_executor = null;

            try {
                lteueConfig = new LTEUEConfig("lteue.properties");
            } catch (IOException e) {
                e.printStackTrace();
                return;
            }

            try {
                log_executor = new Log_executor(lteueConfig);
            } catch (Exception e) {
                e.printStackTrace();
                return;
            }

            Boolean time_out_occured = false;

            log_executor.pre();
            for (String command: split_line) {
                if (command.contains("ε"))
                    continue;

                if (time_out_occured) {
                    System.out.println("RESULT: NULL ACTION");
                    continue;
                }

                String result = log_executor.step(command);

                if (result.matches("timeout")){
                    time_out_occured = true;
                    System.out.println("RESULT: NULL ACTION(TIMEOUT)");
                    continue;
                }

                System.out.println("RESULT: " + result);
            }

        } else {
            System.out.println("Invalid command line arguments");
        }
        try{
            sleep(1000000);
        }catch(Exception e){

        }

        kill_eNodeb();
        try {
            sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        kill_EPC();

    }


    public void start_epc_enb(){
        // kill and start the processes
        try {

            kill_eNodeb();
            sleep(2 * 1000);
            kill_EPC();
            sleep(2*1000);
            start_EPC();
            sleep(10 * 1000);
            start_eNodeB();
            sleep(30 * 1000);
        }catch (InterruptedException e){
            e.printStackTrace();
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    public void restart_epc_enb(){
        try {

            mme_out.close();
            mme_in.close();
            mme_socket.close();

            enodeb_out.close();
            enodeb_in.close();
            enodeb_socket.close();

            sleep(1000);


            start_epc_enb();

            mme_socket = new Socket(config.mme_controller_ip_address, config.mme_port);
            mme_socket.setTcpNoDelay(true);
            //mme_socket.setSoTimeout(30 * 1000); // timeout set to 60s. For different timer, we need to set different values at different places
            mme_out = new BufferedWriter(new OutputStreamWriter(mme_socket.getOutputStream()));
            mme_in = new BufferedReader(new InputStreamReader(mme_socket.getInputStream()));

            enodeb_socket = new Socket(config.enodeb_controller_ip_address, config.enodeb_port);
            enodeb_socket.setTcpNoDelay(true);
            //enodeb_socket.setSoTimeout(30 * 1000); // timeout set to 60s. For different timer, we need to set different values at different places
            enodeb_out = new BufferedWriter(new OutputStreamWriter(enodeb_socket.getOutputStream()));
            enodeb_in = new BufferedReader(new InputStreamReader(enodeb_socket.getInputStream()));

            sqn_synchronized = false;

        }catch (UnknownHostException e){
            e.printStackTrace();
        }catch (SocketException e){
            e.printStackTrace();
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    public void send_msg_to_epc(String msg){
        String result = new String();
        try {
            msg = "----------------- " + msg + "------------------\n";
            mme_out.write(msg);
            mme_out.flush();
            result = mme_in.readLine();
            System.out.println("Received for send_msg_to_epc = " + result);
        }catch(SocketException e){
            e.printStackTrace();
        }catch(IOException e){
            e.printStackTrace();
        }catch(Exception e){
            e.printStackTrace();
        }

    }

    public void init_epc_enb_con(){
        try {
            // Initialize test service
            System.out.println("Connecting to srsEPC...");
            mme_socket = new Socket(config.mme_controller_ip_address, config.mme_port);
            mme_socket.setTcpNoDelay(true);
            mme_out = new BufferedWriter(new OutputStreamWriter(mme_socket.getOutputStream()));
            mme_in = new BufferedReader(new InputStreamReader(mme_socket.getInputStream()));
            System.out.println("Connected with srsEPC.");

            System.out.println("Connecting to srsENB...");
            enodeb_socket = new Socket(config.enodeb_controller_ip_address, config.enodeb_port);
            enodeb_socket.setTcpNoDelay(true);
            enodeb_out = new BufferedWriter(new OutputStreamWriter(enodeb_socket.getOutputStream()));
            enodeb_in = new BufferedReader(new InputStreamReader(enodeb_socket.getInputStream()));
            System.out.println("Connected with srsENB.");

            String result = new String();
            try {
                sleep(2*1000);
                enodeb_out.write("Hello\n");
                enodeb_out.flush();
                result = enodeb_in.readLine();
                System.out.println("Received = " + result);
            }catch(SocketException e){
                e.printStackTrace();
                start_epc_enb();
                init_epc_enb_con();
            }catch(IOException e){
                e.printStackTrace();
                start_epc_enb();
                init_epc_enb_con();
            }catch(Exception e){
                e.printStackTrace();
                start_epc_enb();
                init_epc_enb_con();
            }
            if(result.startsWith("ACK")){
                System.out.println("PASSED: Testing the connection between the statelearner and the srsENB");
            }

        }catch (UnknownHostException e){
            e.printStackTrace();
            start_epc_enb();
            init_epc_enb_con();
        }catch (SocketException e){
            e.printStackTrace();
            start_epc_enb();
            init_epc_enb_con();
        }catch (Exception e){
            e.printStackTrace();
            start_epc_enb();
            init_epc_enb_con();
        }
        System.out.println("Connected to srsLTE");
    }

    public void init_ue_con(){
        try{
            System.out.println("Connecting to UE...");
            System.out.println("UE controller IP Address: " + config.ue_controller_ip_address);
            ue_socket = new Socket(config.ue_controller_ip_address, config.ue_port);
            ue_socket.setTcpNoDelay(true);
            //ue_socket.setSoTimeout(180*1000);
            System.out.println("Connected to UE");

            System.out.println("Initializing Buffers for UE...");
            ue_out = new BufferedWriter(new OutputStreamWriter(ue_socket.getOutputStream()));
            ue_in = new BufferedReader(new InputStreamReader(ue_socket.getInputStream()));
            System.out.println("Initialized Buffers for UE");

        }catch (UnknownHostException e){
            e.printStackTrace();
        }catch (SocketException e){
            e.printStackTrace();
        }catch (Exception e){
            e.printStackTrace();
        }
    }



    public int minimum(int a, int b, int c) {
        return Math.min(Math.min(a, b), c);
    }

    public int computeLevenshteinDistance(CharSequence lhs, CharSequence rhs) {
        int[][] distance = new int[lhs.length() + 1][rhs.length() + 1];

        for (int i = 0; i <= lhs.length(); i++)
            distance[i][0] = i;
        for (int j = 1; j <= rhs.length(); j++)
            distance[0][j] = j;

        for (int i = 1; i <= lhs.length(); i++)
            for (int j = 1; j <= rhs.length(); j++)
                distance[i][j] = minimum(
                        distance[i - 1][j] + 1,
                        distance[i][j - 1] + 1,
                        distance[i - 1][j - 1] + ((lhs.charAt(i - 1) == rhs.charAt(j - 1)) ? 0 : 1));

        return distance[lhs.length()][rhs.length()];
    }

    public String getClosests(String result) {
        System.out.println("Getting closest of " + result);
        /*
        if (Arrays.asList(expectedResults).contains(result)) {
            return result;
        }

        int minDistance = Integer.MAX_VALUE;
        String correctWord = null;

        for (String word: Arrays.asList(expectedResults)) {
            int distance = computeLevenshteinDistance(result, word);

            if (distance < minDistance) {
                correctWord = word;
                minDistance = distance;
            }
        }
        */
        if (expectedResults.contains(result)) {
            return result;
        }

        int minDistance = Integer.MAX_VALUE;
        String correctWord = null;


        for (String word : expectedResults) {
            int distance = computeLevenshteinDistance(result, word);

            if (distance < minDistance) {
                correctWord = word;
                minDistance = distance;
            }
        }
        return correctWord;
    }

	public String getExpectedResult(String symbol, String result) {

		String final_result = "null_action";

		if (symbol.contains("enable_s1")) {
			if (enables1_expectedResults.contains(result)) {
				final_result = result;
			}
		}
		else if (symbol.contains("identity_request_plain") || symbol.contains("identity_request_mac") || symbol.contains("identity_request_wrong_mac")||symbol.contains("identity_request_replay")) {
			if (identityrequestplain_expectedResults.contains(result)) {
				final_result = result;
			}
		}
		else if (symbol.contains("identity_request_protected")) {
			if (identityrequestprotected_expectedResults.contains(result)) {
				final_result = result;
			}
		}
		else if (symbol.contains("auth_request_plain") || symbol.contains("auth_request_replay")) {
			if (authrequestplain_expectedResults.contains(result)) {
				final_result = result;
			}
		}
		else if (symbol.contains("auth_request_protected")) {
			if (authrequestprotected_expectedResults.contains(result)) {
				final_result = result;
			}
		}
		else if (symbol.contains("security_mode_command") || symbol.contains("security_mode_command_replay")) {
			if (secmodcmd_expectedResults.contains(result)) {
				final_result = result;
			}
		}
		else if (symbol.contains("attach_accept")) {
			if (attachaccept_expectedResults.contains(result)) {
				final_result = result;
			}
		}
		else if (symbol.contains("dl_nas_transport") || symbol.contains("dl_nas_transport_plain")) {
			if (dlnastransport_expectedResults.contains(result)) {
				final_result = result;
			}
		}
		else if (symbol.contains("rrc_release_tau")) {
			if (rrcreleasetau_expectedResults.contains(result)) {
				final_result = result;
			}
		}
		else if (symbol.contains("tau_accept") || symbol.contains("tau_accept_plain")) {
			if (tauaccept_expectedResults.contains(result)) {
				final_result = result;
			}
		}
		else if (symbol.contains("GUTI_reallocation")||symbol.contains("GUTI_reallocation_plain")) {
			if (gutireallocation_expectedResults.contains(result)) {
				final_result = result;
			}
		}
		else if (symbol.contains("paging_tmsi")) {
			if (pagingtmsi_expectedResults.contains(result)) {
				final_result = result;
			}
		}
		return final_result;
	}


    public static Boolean execute_query(Log_executor log_executor, List<String> query) {
        log_executor.pre();
        boolean time_out_occured = false;

        boolean exception_occured = false;

        for (String command: query) {
            if (command.contains("ε"))
                continue;
            System.out.println("COMMAND: " + command);

            if (time_out_occured) {
                System.out.println("RESULT: NULL ACTION");
                continue;
            }

            command = command.replaceAll("\\s+","");
            long startTime = System.currentTimeMillis();
            String result = log_executor.step(command);
            long endTime = System.currentTimeMillis();
            long duration = (endTime - startTime);
            double insec = duration/1000.0;
            System.out.println("time elapsed in prefix: "+(duration)+" "+insec);
            if (result.matches("EXCEPTION")) {
                System.out.println("Exception occured, restarting query");
                exception_occured = true;
            }

            //TODO: Add Levenshtein Distance method here

            //result = getClosests(result, expectedResults);

            if (result.matches("timeout")){
                time_out_occured = true;
                System.out.println("RESULT: NULL ACTION(TIMEOUT)");
                continue;
            }

            System.out.println("RESULT: " + result);
        }

        return exception_occured;
    }

    public String reset_mme(){
        String result = new String("");
        System.out.println("Sending symbol: RESET to MME controller");
        try {
            sleep(1*1000);
            mme_out.write("RESET " + reset_mme_count + "\n");
            mme_out.flush();
            result = mme_in.readLine();
            System.out.println("ACK for RESET_MME: " + result);
            reset_mme_count++;
            //sleep(2*1000);
        }catch(SocketException e){
            e.printStackTrace();
        }catch(IOException e){
            e.printStackTrace();
        }/*catch(InterruptedException e){
            e.printStackTrace();
        }*/catch(Exception e){
            e.printStackTrace();
        }
        return result;
    }

    public String reset_ue(){
        String result = new String("");
        System.out.println("Sending symbol: RESET to UE controller");
        try {
            sleep(1 * 1000);
            ue_out.write("RESET\n");
            ue_out.flush();
            result = ue_in.readLine();
            System.out.println("ACK for RESET_UE: " + result);
        }catch(SocketException e){
            e.printStackTrace();
        }catch(IOException e){
            e.printStackTrace();
        }/*catch(InterruptedException e){
            e.printStackTrace();
        }*/catch(Exception e){
            e.printStackTrace();
        }
        return result;
    }


    public void send_sim_card_reset(){

        try {
            sleep(2000);
            System.out.println("Sending symbol: SIM_CARD_RESET to UE controller");
            enable_s1_count++;
            ue_out.write("sim_card_reset\n");
            ue_out.flush();
            String result = new String("");
            enable_s1_count++;
            result = ue_in.readLine();
            System.out.println("Result for sim_card_reset: " + result);
            if(!result.contains("DONE")){
                send_sim_card_reset();
            }


        } /*catch (InterruptedException e) {
            e.printStackTrace();
        }*/catch (SocketException e){
            e.printStackTrace();
        }catch(IOException e){
            e.printStackTrace();
        }catch(Exception e){
            e.printStackTrace();
        }
    }

    public void send_enable_s1(){
        try {
            sleep(1000);
            String result = new String("");
            //            do {
            //                mme_out.write("enable_s1\n");
            //                mme_out.flush();
            //                result = mme_in.readLine();
            //                System.out.println("MME's ACK for enable_s1: " + result);
            //            } while (!result.contains("DONE"));

            //sleep(1000);
            System.out.println("Sending symbol: enable_s1 to UE controller");
            enable_s1_count++;
            ue_out.write("enable_s1\n");
            ue_out.flush();

            enable_s1_count++;
            result = ue_in.readLine();
            System.out.println("UE controller's ACK for enable_s1: " + result);
            if(!result.contains("DONE")){
                send_enable_s1();
            }


        } catch (SocketException e){
            e.printStackTrace();
        }catch(IOException e){
            e.printStackTrace();
        }catch(Exception e){
            e.printStackTrace();
        }

    }


    public String reboot_ue(){
        System.out.println("Sending REBOOT_UE command to UE_CONTROLLER");
        String result = new String("");
        try {
            ue_out.write("ue_reboot\n"); // reboot the UE and turn cellular network ON with 4G LTE
            ue_out.flush();
            System.out.println("Waiting for the response from UE .... ");
            result = ue_in.readLine();
            System.out.println("UE's ACK for REBOOT: " + result);

        }catch (SocketException e){
            e.printStackTrace();
        }catch(IOException e){
            e.printStackTrace();
        }catch(Exception e){
            e.printStackTrace();
        }
        return result;
    }


    public String restart_ue_adb_server(){
        System.out.println("Sending adb restart-server command to UE_CONTROLLER");
        String result = new String("");
        try {
            //sleep(2000);
            ue_out.write("adb_server_restart\n");
            ue_out.flush();
            result = ue_in.readLine();
            System.out.println("Result for adb_server_restart: " + result);
        }catch (SocketException e){
            e.printStackTrace();
        }catch(IOException e){
            e.printStackTrace();
        }catch(Exception e){
            e.printStackTrace();
        }
        return result;

    }

	public String step(String symbol) {

		try {
			sleep(50); //50 milliseconds
		}catch (Exception e){
			e.printStackTrace();
		}

		String result = "";
		String attach_result = "";
		try{

			if(symbol.startsWith("enable_s1")) {
				//TODO: if there is any reject (e.g., auth_reject), while loop should be removed.
				while (!result.contains("attach_request")) {
					mme_socket.setSoTimeout(180*1000);
					send_enable_s1();
					result = mme_in.readLine();
					if (result.compareTo("") != 0 && result.toCharArray()[0] == ' ') {
						result = new String(Arrays.copyOfRange(result.getBytes(), 1, result.getBytes().length));
					}
					result = getClosests(result);
					if(result.contains("detach_request")){
						mme_socket.setSoTimeout(180*1000);
						attach_result = mme_in.readLine();
						if (attach_result.compareTo("") != 0 && attach_result.toCharArray()[0] == ' ') {
							attach_result = new String(Arrays.copyOfRange(attach_result.getBytes(), 1, attach_result.getBytes().length));
						}
						attach_result = getClosests(attach_result);
						//result = result + ","+ attach_result;
						result = attach_result;
					}
					if(result.contains("attach_request_guti")){
						result = "attach_request";
						System.out.println(symbol + "->IK " + result);
					}
				}
				mme_socket.setSoTimeout(5*1000);
				System.out.println(symbol + "->" + result);
				return result;
			}
		} catch (SocketTimeoutException e){
			//System.out.println("enable_s1 timeout occured.");
			System.out.println("Timeout occured for " + symbol);
			handle_timeout(); //TODO: if there is any reject (e.g., auth_reject), handle_timeout should not reboot the UE
			return "timeout";

		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("Attempting to restart device and reset srsEPC. Also restarting query.");
			handle_enb_epc_failure();
			return "null_action";
		}

		try {
			if (symbol.contains("reject")) {
				mme_socket.setSoTimeout(5*1000);
				mme_out.write(symbol + "\n");
				mme_out.flush();

				result = mme_in.readLine();
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
			handle_timeout();
			return "timeout";
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("Attempting to restart device and reset srsEPC. Also restarting query.");
			handle_enb_epc_failure();
			return "null_action";
		}

		try{
			if(! symbol.startsWith("enable_s1") && ! symbol.contains("reject")){
				if(symbol.startsWith("attach_accept")){
					mme_socket.setSoTimeout(4* 1000);
				}else if (symbol.startsWith("rrc_release")){
					TimeUnit.SECONDS.sleep(10);
					mme_socket.setSoTimeout(5 * 1000);
				} else if (symbol.startsWith("paging_tmsi")) {
					System.out.println("### RRC RELEASE ###");
					TimeUnit.SECONDS.sleep(10);
					String symbol_1 = "rrc_release";
					mme_out.write(symbol_1 + "\n");
					mme_out.flush();
					mme_socket.setSoTimeout(10 * 1000);
					//result = mme_in.readLine();
					//System.out.println("Response of rrc_release = " + result);
					sleep(2000);
				}else if(symbol.startsWith("tau_accept")){
					mme_socket.setSoTimeout(4* 1000);
				} else {
					mme_socket.setSoTimeout(2 * 1000);
				}

				mme_out.write(symbol + "\n");
				mme_out.flush();

				result = "attach_request";
				while(result.contains("attach_request")) {

					result = mme_in.readLine();

					if (result.compareTo("") != 0 && result.toCharArray()[0] == ' ') {
						result = new String(Arrays.copyOfRange(result.getBytes(), 1, result.getBytes().length));
					}
					result = getClosests(result);
					result = getExpectedResult(symbol, result);

					//FIXME: if attach_request is received for any query message other than enable_s1, we replace
					// response with null_action.
					//FIXME: attach_request may be received because of non-integrity protected query messages after the
					//completion of the attach procedure. We need to check this case later and add a fix for that.
					if (result.contains("attach_request") || result.contains("DONE")) {
						System.out.println("Response of " + symbol + " = Unexpected attach_request");
						continue;
					} else {
						if (result.contains("emm_status")) {
							System.out.println("Actual response of " + symbol + " = emm_status");
							result = "null_action";
						}
						if (result.contains("attach_request_guti")) {
							System.out.println("Actual response of " + symbol + " = attach_request_guti");
							result = "attach_request";
						}
						if (result.contains("detach_request")) {
							//if (symbol.startsWith("attach_accept")){
							//	result = "attach_complete";
							//}else{
							result = "null_action";
							//}
							System.out.println("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
							System.out.println("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
							System.out.println("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
							System.out.println("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
							System.out.println("Actual response of " + symbol + " = detach_request");
						}
						else {
							System.out.println("Response of " + symbol + " = " + result);
							System.out.println(symbol + "->" + result);
							return result;
						}
					}
				}
			}
		} catch (SocketTimeoutException e){
			System.out.println("Timeout occured for " + symbol);
			return "null_action";
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("Attempting to restart device and reset srsEPC. Also restarting query.");
			handle_enb_epc_failure();
			return "null_action";

		}

		try {
			if(result.compareTo("") != 0 && result.toCharArray()[0] == ' ') {
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
			System.out.println("Attempting to restart device and reset srsEPC. Also restarting query.");
			handle_enb_epc_failure();
			return "null_action";
		}


		System.out.println("####" + symbol +"/"+result + "####");




		return result;
	}
    public void pre() {
		int flag = 0;
		try {

			if (!config.combine_query) {

				String result = new String("");
				String result_for_mme = new String("");
				String result_for_ue = new String("");
				String attach_result = new String ("");
				boolean reset_done = false;

				attach_request_guti_count = 0;
				enable_s1_timeout_count = 0;
				reboot_count = 0;

				System.out.println("---- Starting RESET ----");

				//if(enb_alive() == false || mme_alive() == false){
				//	handle_enb_epc_failure();
				//	return;
				//}
				/* The EPC needs to synchronize the sqn value with UE.
				 * Thats why statelearner+srsEPC needs to perform authentication procedure for RESET operation
				 */
				do {
					try {
						//System.out.println("RESETING MME AT THE START OF PRE " + reset_mme_count + "???");
						result_for_mme = reset_mme();
						//sleep(1000);
						result_for_ue = reset_ue();
						result = new String("");
						attach_result = new String("");

						mme_socket.setSoTimeout(180*1000);
						if (flag == 0){
							flag = 1;
							//System.out.println("Sending symbol: attach_reject to MME controller to delete the UE context in attach_request");
							//mme_out.write("attach_reject\n");
							//mme_out.flush();
							//send_enable_s1();
							//result = mme_in.readLine();
							//result = new String(Arrays.copyOfRange(result.getBytes(), 1, result.getBytes().length));
							//result = getClosests(result);
						}
						send_enable_s1();

						result = mme_in.readLine();
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

						if (result.contains("attach_request_guti") || result.contains("service_request") || result.contains("tau_request")) {
							attach_request_guti_count++;
							flag = 1;
							/*
							System.out.println("Sending symbol: auth_reject to MME controller to delete the UE context");
							mme_out.write("auth_reject\n");
							mme_out.flush();
							TimeUnit.SECONDS.sleep(2);
							reboot_ue();
							*/
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
						else if (result.startsWith("attach_request") || result.startsWith("DONE")) {
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
	}


    public void handle_timeout(){
        String result = new String("");
        if(enb_alive() == false || mme_alive() == false){
            handle_enb_epc_failure();
            return;
        }
        try {
            ue_out.write("ue_reboot\n"); // reboot the UE and turn cellular network ON with 4G LTE
            ue_out.flush();
            System.out.println("Sleeping while UE reboots");
            TimeUnit.SECONDS.sleep(45);
            result = ue_in.readLine();
            System.out.println("Result for reboot: " + result);
        }catch (IOException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("TIMEOUT HANDLE DONE.");

    }

    public void is_adb_server_restart_required() {
        String result = new String("");
        if(enable_s1_count >= 50){
            //ue_socket.setSoTimeout(30*1000);
            enable_s1_count = 0;
            result = restart_ue_adb_server();
        }

    }

    public void handle_enb_epc_failure(){
        String result = new String("");

        try {
            reboot_ue();
            restart_epc_enb();
            enable_s1_timeout_count = 0;

        }catch (Exception e) {
            e.printStackTrace();
        }
        System.out.println("ENB EPC FAILURE HANDLING DONE.");

    }

    public boolean enb_alive() {
        String result = "";
        try {
            enodeb_socket.setSoTimeout(5*1000);
            enodeb_out.write("Hello\n");
            enodeb_out.flush();
            result = enodeb_in.readLine();
            System.out.println("Received from Hello message in enb alive = " + result);
            enodeb_socket.setSoTimeout(30*1000);
        }catch(SocketTimeoutException e){
            e.printStackTrace();
            return false;
        }catch (Exception e){
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
    }
    public boolean mme_alive() {
        String result = "";
        try {
            mme_socket.setSoTimeout(5*1000);
            mme_out.write("Hello\n");
            mme_out.flush();
            result = mme_in.readLine();
            System.out.println("Received from Hello message in mme alive = " + result);
            mme_socket.setSoTimeout(30*1000);

        } catch(SocketTimeoutException e){
            e.printStackTrace();
            return false;
        }catch (Exception e){
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
    }

    /**
     * Methods to kill and restart srsEPC and srsENB
     */

    public static void start_eNodeB() {
        runProcess(false, "/home/rafiul/my-computer/research/lte/code/LTEUE-State-Fuzzing/log_executor/src/start_enb.sh");
        //runProcess(false, "/home/cyber2slab/LTEUE-State-Fuzzing/log_executor/src/start_enb.sh");
        //runProcess(false, "/home/mitziu/Research/automata-learning/LTEUE-State-Fuzzing/log_executor/src/start_enb.sh");


    }

    public static void start_EPC() {
        runProcess(false, "/home/rafiul/my-computer/research/lte/code/LTEUE-State-Fuzzing/log_executor/src/start_epc.sh");
        //runProcess(false, "/home/cyber2slab/LTEUE-State-Fuzzing/log_executor/src/start_epc.sh");
        //runProcess(false, "/home/mitziu/Research/automata-learning/LTEUE-State-Fuzzing/log_executor/src/start_epc.sh");

    }

    public static void kill_eNodeb() {
        runProcess(false, "/home/rafiul/my-computer/research/lte/code/LTEUE-State-Fuzzing/log_executor/src/kill_enb.sh");
        //runProcess(false, "/home/cyber2slab/LTEUE-State-Fuzzing/log_executor/src/kill_enb.sh");
        //runProcess(false, "/home/mitziu/Research/automata-learning/LTEUE-State-Fuzzing/log_executor/src/kill_enb.sh");

    }

    public static void kill_EPC() {
        runProcess(false, "/home/rafiul/my-computer/research/lte/code/LTEUE-State-Fuzzing/log_executor/src/kill_enb.sh");
        //runProcess(false, "/home/cyber2slab/LTEUE-State-Fuzzing/log_executor/src/kill_epc.sh");
        //runProcess(false, "/home/mitziu/Research/automata-learning/LTEUE-State-Fuzzing/log_executor/src/kill_epc.sh");


    }

    public static void kill_process(String path, String nameOfProcess) {
        ProcessBuilder pb = new ProcessBuilder(path);
        Process p;
        try {
            p = pb.start();
        } catch (IOException e) {
            e.printStackTrace();
        }

        System.out.println("Killed " + nameOfProcess);
        System.out.println("Waiting a second");
        try {
            TimeUnit.SECONDS.sleep(1);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public static void start_process(String path, String nameOfProcess) {
        ProcessBuilder pb = new ProcessBuilder(path);
        Process p;
        try{
            p = pb.start();
            System.out.println(nameOfProcess + " process has started");
            System.out.println("Waiting a second");
            TimeUnit.SECONDS.sleep(1);
        } catch (IOException e){
            System.out.println("ERROR: " + nameOfProcess + " is not running after invoking script");
            System.out.println("Attempting again...");
            start_process(path, nameOfProcess);
            e.printStackTrace();
        } catch (InterruptedException e) {
            System.out.println("ERROR: " + nameOfProcess + " is not running after invoking script");
            System.out.println("Attempting again...");
            start_process(path, nameOfProcess);
            e.printStackTrace();
        }

        String line;
        try {
            Process temp = Runtime.getRuntime().exec("pidof " + nameOfProcess);
            BufferedReader input = new BufferedReader(new InputStreamReader(temp.getInputStream()));
            line = input.readLine();
            if (line == null){
                System.out.println("ERROR: " + nameOfProcess + " is not running after invoking script");
                System.out.println("Attempting again...");
                start_process(path, nameOfProcess);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        System.out.println(nameOfProcess + " has started...");
    }

    public static void runProcess(boolean isWin, String... command) {
        System.out.print("command to run: ");
        for (String s : command) {
            System.out.print(s);
        }
        System.out.print("\n");
        String[] allCommand = null;
        try {
            if (isWin) {
                allCommand = concat(WIN_RUNTIME, command);
            } else {
                allCommand = concat(OS_LINUX_RUNTIME, command);
            }
            ProcessBuilder pb = new ProcessBuilder(allCommand);
            pb.redirectErrorStream(true);
            Process p = pb.start();

            //p.waitFor();
//            BufferedReader in = new BufferedReader(new InputStreamReader(p.getInputStream()));
//            String _temp = null;
//            String line = new String("");
//            while ((_temp = in.readLine()) != null) {
//                System.out.println("temp line: " + _temp);
//                line += _temp + "\n";
//            }
//            System.out.println("result after command: " + line);

            return;

        }catch (IOException e){
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
