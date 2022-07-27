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

public class VoWiFiUEConfig extends LearningConfig {
  String alphabet;
  String hostname;
  String outputDir;

  String ueControllerIPAddress;
  String epdgControllerIPAddress;
  String imsControllerIPAddress;

  int uePort;
  int epdgPort;
  int imsPort;

  String epdgStartCmd;
  String epdgStopCmd;
  String imsStartCmd;
  String imsStopCmd;

  boolean combineQuery;
  String delimiterInput;
  String delimiterOutput;

  String reliabilityTestcasePath;

  public VoWiFiUEConfig(String filename) throws IOException {
    super(filename);
  }

  public VoWiFiUEConfig(LearningConfig config) {
    super(config);
  }

  @Override
  public void loadProperties() {
    super.loadProperties();

    if(properties.getProperty("alphabet") != null)
      alphabet = properties.getProperty("alphabet");

    if(properties.getProperty("hostname") != null)
      hostname = properties.getProperty("hostname");

    if(properties.getProperty("output_dir") != null)
      outputDir = properties.getProperty("output_dir");

    if(properties.getProperty("ue_controller_ip_address") != null)
      ueControllerIPAddress = properties.getProperty("ue_controller_ip_address");

    if(properties.getProperty("epdg_controller_ip_address") != null)
      epdgControllerIPAddress = properties.getProperty("epdg_controller_ip_address");

    if(properties.getProperty("ims_controller_ip_address") != null)
      imsControllerIPAddress = properties.getProperty("ims_controller_ip_address");

    if(properties.getProperty("ue_port") != null)
      uePort = Integer.parseInt(properties.getProperty("ue_port"));

    if(properties.getProperty("epdg_port") != null)
      epdgPort = Integer.parseInt(properties.getProperty("epdg_port"));

    if(properties.getProperty("ims_port") != null)
      imsPort = Integer.parseInt(properties.getProperty("ims_port"));

    if(properties.getProperty("epdg_start_cmd") != null)
      epdgStartCmd = properties.getProperty("epdg_start_cmd");

    if(properties.getProperty("epdg_stop_cmd") != null)
      epdgStopCmd = properties.getProperty("epdg_stop_cmd");

    if(properties.getProperty("ims_start_cmd") != null)
      imsStartCmd = properties.getProperty("ims_start_cmd");

    if(properties.getProperty("ims_stop_cmd") != null)
      imsStopCmd = properties.getProperty("ims_stop_cmd");

    if(properties.getProperty("reliability_test") != null)
      reliabilityTestcasePath = properties.getProperty("reliability_test");

    if(properties.getProperty("combine_query") != null)
      combineQuery = Boolean.parseBoolean(properties.getProperty("combine_query"));
    else
      combineQuery = false;

    if(properties.getProperty("delimiter_input") != null)
      delimiterInput = properties.getProperty("delimiter_input");
    else
      delimiterInput = ";";

    if(properties.getProperty("delimiter_output") != null)
      delimiterOutput = properties.getProperty("delimiter_output");
    else
      delimiterOutput = ";";
  }

  public boolean getCombineQuery() {
    return combineQuery;
  }

  public String getOutputDir() {
    return outputDir;
  }

  public String getUEControllerIPAddress() {
    return ueControllerIPAddress;
  }

  public String getEPDGControllerIPAddress() {
    return epdgControllerIPAddress;
  }

  public String getIMSControllerIPAddress() {
    return imsControllerIPAddress;
  }

  public int getUEControllerPort() {
    return uePort;
  }

  public int getEPDGControllerPort() {
    return epdgPort;
  }

  public int getIMSControllerPort() {
    return imsPort;
  }

  public String getEPDGStartCmd() {
    return epdgStartCmd;
  }

  public String getEPDGStopCmd() {
    return epdgStopCmd;
  }

  public String getIMSStartCmd() {
    return imsStartCmd;
  }

  public String getIMSStopCmd() {
    return imsStopCmd;
  }

  public String getReliabilityTest() {
    return reliabilityTestcasePath;
  }
}
