package lte.statelearner;

import java.io.*;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import lte.statelearner.lteue.LTEUEConfig;
import lte.statelearner.lteue.LTEUESUL;

import static java.lang.Thread.sleep;

public class LogExecutor {
    LTEUEConfig config;
    LTEUESUL sul;
    Socket mme_socket, enodeb_socket, ue_socket;
    BufferedWriter mme_out, enodeb_out, ue_out;
    BufferedReader mme_in, enodeb_in, ue_in;


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
            "GUTI_reallocation_complete",
            "tau_complete",
            "ul_nas_transport",
            "null_action",
            "DONE"};

    public LogExecutor(LTEUESUL sul) throws Exception {
        this.sul = sul;
    }

    public void run(String[] args) {
        List<List<String>> queries = new ArrayList<>();

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

            Boolean time_out_occured = false;

            Integer querry_num = 1;

            for (List<String> query: queries){

                Boolean exception_occured = execute_query(query);

                while (exception_occured) {
                    exception_occured = execute_query(query);
                }

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


            Boolean time_out_occured = false;

            this.sul.pre();

            boolean enb_alive = true;

            while(!this.sul.enb_alive()) {
                System.out.println("eNodeB is not alive");
                System.out.println("Invoking this.sul.restart_epc_enb and verifying it is alive after restart");
                this.sul.restart_epc_enb();
            }

            do {
                for (String command: split_line) {
                    if (command.contains("ε"))
                        continue;

                    if (time_out_occured) {
                        System.out.println("RESULT: NULL ACTION");
                        continue;
                    }

                    String result = this.sul.step(command);
                    result = getClosests(result);

                    if (result.matches("timeout")){
                        time_out_occured = true;
                        System.out.println("RESULT: NULL ACTION(TIMEOUT)");
                        continue;
                    }

                    System.out.println("RESULT: " + result);
                }
                enb_alive = this.sul.enb_alive();

                if (!enb_alive) {
                    System.out.println("eNodeB was not alive at the end of the query, restarting the query");
                    System.out.println("Invoking this.sul.restart_epc_enb and verifying it is alive after restart");
                    while (!this.sul.enb_alive()) {
                        this.sul.restart_epc_enb();
                    }
                }

            } while(!enb_alive);



        } else {
            System.out.println("Invalid command line arguments");
        }

    }

    public Boolean execute_query(List<String> query) {
        this.sul.pre();
        boolean time_out_occured = false;

        boolean exception_occured = false;

        boolean enb_alive = true;

        while(!this.sul.enb_alive()) {
            System.out.println("eNodeB is not alive");
            System.out.println("Invoking this.sul.restart_epc_enb and verifying it is alive after restart");
            this.sul.restart_epc_enb();
        }

        do {
            for (String command: query) {
                if (command.contains("ε"))
                    continue;
                System.out.println("COMMAND: " + command);

                if (time_out_occured) {
                    System.out.println("RESULT: NULL ACTION");
                    continue;
                }

                command = command.replaceAll("\\s+","");
                String result = this.sul.step(command);

                if (result.matches("EXCEPTION")) {
                    System.out.println("Exception occured, restarting query");
                    exception_occured = true;
                }

                //TODO: Add Levenshtein Distance method here

                result = getClosests(result);

                if (result.matches("timeout")){
                    time_out_occured = true;
                    System.out.println("RESULT: NULL ACTION(TIMEOUT)");
                    continue;
                }

                System.out.println("RESULT: " + result);
            }

            enb_alive = this.sul.enb_alive();

            if (!enb_alive) {
                System.out.println("eNodeB was not alive at the end of the query, restarting the query");
                System.out.println("Invoking this.sul.restart_epc_enb and verifying it is alive after restart");
                while (!this.sul.enb_alive()) {
                    this.sul.restart_epc_enb();
                }
            }

        } while(!enb_alive);

        return exception_occured;
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

        return correctWord;
    }
}

