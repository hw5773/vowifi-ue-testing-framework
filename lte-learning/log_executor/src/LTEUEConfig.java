/*
 *  Copyright (c) 2016 Joeri de Ruiter
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


import java.io.IOException;

public class LTEUEConfig extends LearningConfig {
    String alphabet;
    String hostname;
    String ue_controller_ip_address;
    String enodeb_controller_ip_address;
    String mme_controller_ip_address;
    int mme_port;
    int ue_port;
    int enodeb_port;

    boolean combine_query;
    String delimiter_input;
    String delimiter_output;

    public LTEUEConfig(String filename) throws IOException {
        super(filename);
    }

    public LTEUEConfig(LearningConfig config) {
        super(config);
    }

    @Override
    public void loadProperties() {
        super.loadProperties();

        if(properties.getProperty("alphabet") != null)
            alphabet = properties.getProperty("alphabet");

        if(properties.getProperty("hostname") != null)
            hostname = properties.getProperty("hostname");

        if(properties.getProperty("ue_controller_ip_address") != null)
            ue_controller_ip_address = properties.getProperty("ue_controller_ip_address");

        if(properties.getProperty("ue_controller_ip_address") != null)
            ue_controller_ip_address = properties.getProperty("ue_controller_ip_address");

        if(properties.getProperty("enodeb_controller_ip_address") != null)
            enodeb_controller_ip_address = properties.getProperty("enodeb_controller_ip_address");

        if(properties.getProperty("mme_controller_ip_address") != null)
            mme_controller_ip_address = properties.getProperty("mme_controller_ip_address");

        if(properties.getProperty("mme_port") != null)
            mme_port = Integer.parseInt(properties.getProperty("mme_port"));

        if(properties.getProperty("ue_port") != null)
            ue_port = Integer.parseInt(properties.getProperty("ue_port"));

        if(properties.getProperty("enodeb_port") != null)
            enodeb_port = Integer.parseInt(properties.getProperty("enodeb_port"));

        if(properties.getProperty("combine_query") != null)
            combine_query = Boolean.parseBoolean(properties.getProperty("combine_query"));
        else
            combine_query = false;

        if(properties.getProperty("delimiter_input") != null)
            delimiter_input = properties.getProperty("delimiter_input");
        else
            delimiter_input = ";";

        if(properties.getProperty("delimiter_output") != null)
            delimiter_output = properties.getProperty("delimiter_output");
        else
            delimiter_output = ";";
    }

    public boolean getCombineQuery() {
        return combine_query;
    }
}