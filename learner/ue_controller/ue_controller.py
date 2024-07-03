import sys
import time
import socket
import subprocess
import threading
import os
import logging
import argparse
import signal

ACK = "ACK\n".encode()
FAIL = "Fail\n".encode()
REBOOT_TIMEOUT = 120
REBOOT_WAIT_TIME = 25

def handle_turn_off_wifi_interface(device):
    if device == "SM_G920T":
        cmd = ["adb", "shell", "su", "-c", "svc", "wifi", "disable"]
        subprocess.run(cmd)
    elif device == "moto_e5_plus" or device == "HTC_U11" or device == "Blackview_A55" or device =="OnePlus_Nord_N20" or device == "T-mobile_Revvl4":
        cmd = ["adb", "shell", "dumpsys", "wifi", "|", "grep", "\"Wi-Fi is\""]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)

        if "enabled" in result.stdout.decode():
            logging.info("WiFi is enabled: need to toggle the WiFi button")
            ue_wakeup(device)

            cmd = ["adb", "shell", "am", "start", "-a", "android.intent.action.MAIN", "-n", "com.android.settings/.wifi.WifiSettings"]
            output = subprocess.run(cmd, stdout=subprocess.PIPE)
            time.sleep(3)

            cmd = ["adb", "shell", "input", "keyevent", "23"]
            subprocess.run(cmd)
            time.sleep(5)

            cmd = ["adb", "shell", "dumpsys", "wifi", "|", "grep", "\"Wi-Fi is\""]
            output = subprocess.run(cmd, stdout=subprocess.PIPE)

            if "enabled" in output.stdout.decode():
                cmd = ["adb", "shell", "input", "keyevent", "23"]
                subprocess.run(cmd)
                time.sleep(5)
        else:
            logging.info("WiFi is disabled: need not to toggle the WiFi button")
    else:
        cmd = ["adb", "shell", "svc", "wifi", "disable"]
        subprocess.run(cmd)
    time.sleep(1)

def handle_turn_on_wifi_interface(device):
    if device == "SM_G920T":
        cmd = ["adb", "shell", "su", "-c", "svc", "wifi", "enable"]
        subprocess.run(cmd)
    elif device == "moto_e5_plus" or device == "HTC_U11" or device == "Blackview_A55" or device =="OnePlus_Nord_N20" or device == "T-mobile_Revvl4":
        cmd = ["adb", "shell", "dumpsys", "wifi", "|", "grep", "\"Wi-Fi is\""]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)

        if "disabled" in result.stdout.decode():
            logging.info("WiFi is disabled: need to toggle the WiFi button")
            ue_wakeup(device)

            cmd = ["adb", "shell", "am", "start", "-a", "android.intent.action.MAIN", "-n", "com.android.settings/.wifi.WifiSettings"]
            subprocess.run(cmd)
            time.sleep(1)

            cmd = ["adb", "shell", "input", "keyevent", "23"]
            subprocess.run(cmd)
            time.sleep(1)

            cmd = ["adb", "shell", "dumpsys", "wifi", "|", "grep", "\"Wi-Fi is\""]
            output = subprocess.run(cmd, stdout=subprocess.PIPE)

            if "disabled" in output.stdout.decode():
                cmd = ["adb", "shell", "input", "keyevent", "23"]
                subprocess.run(cmd)
                time.sleep(1)
        else:
            logging.info("WiFi is enabled: need not to toggle the WiFi button")
    else:
        cmd = ["adb", "shell", "svc", "wifi", "enable"]
        subprocess.run(cmd)
    time.sleep(1)

def ue_wakeup(device):
    cmd = ["adb", "shell", "input", "keyevent", "224"]
    result = subprocess.run(cmd, stdout=subprocess.PIPE)
    time.sleep(2)
    cmd = ["adb", "shell", "input", "keyevent", "82"]
    result = subprocess.run(cmd, stdout=subprocess.PIPE)
 
def ue_reboot(device):
    cmd = ["adb", "reboot"]
    subprocess.run(cmd)
    time.sleep(1)

def check_ue_availability():
    pass

def adb_server_restart():
    logging.info("Kill the ADB server")
    cmd = ["adb", "kill-server"]
    subprocess.run(cmd)
    #time.sleep(1)

    logging.info("Start the ADB server")
    cmd = ["adb", "start-server"]
    subprocess.run(cmd)
    #time.sleep(1)

def handle_reset(client, device):
    logging.debug("Before turning off the wifi interface")
    handle_turn_off_wifi_interface(device)
    logging.debug("After turning off the wifi interface")
    logging.debug("Before turning on the wifi interface")
    handle_turn_on_wifi_interface(device)
    logging.debug("After turning on the wifi interface")
    logging.debug("Before waking up the device")
    ue_wakeup(device)
    logging.debug("After waking up the device")
    #time.sleep(1)
    logging.debug("Before sending ACK to the statelearner")
    client.send(ACK)
    logging.debug("After sending ACK to the statelearner")

def handle_ue_reboot(client, device):
    ue_reboot(device)
    time.sleep(5)
    cnt = 0
    start = int(time.time())
    retry = 0
    no_device = 0
    success = False
    while True:
        cmd = ["adb", "devices", "-l"]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)
        output = result.stdout.decode()
        if device in output:
            logging.info("UE reboot success")
            success = True
            break

        curr = int(time.time())
        if curr - start >= REBOOT_TIMEOUT:
            if cnt < 3:
                ue_reboot(device)
                cnt += 1
            else:
                logging.info("UE reboot failure: timeout")
                break
        time.sleep(7)
    if success:
        time.sleep(REBOOT_WAIT_TIME)
        handle_init_config(device)
        client.send(ACK)
    else:
        client.send(FAIL)

def handle_adb_server_restart(client):
    adb_server_restart()
    time.sleep(1)
    client.send(ACK)

def is_wifi_enabled():
    cmd_check =["adb", "shell", "dumpsys", "wifi" ]
    result_check = subprocess.run(cmd_check, stdout=subprocess.PIPE, text=True)
    return "Wi-Fi is enabled" in result_check.stdout
def is_vowifi_enabled():
    cmd_check = ["adb", "shell", "dumpsys", "telephony.registry"]
    result_check = subprocess.run(cmd_check, stdout=subprocess.PIPE, text=True)
    return "VoWifi enabled" in result_check.stdout

def handle_init_config(device):
    if device == "ZTE_Stage_5G":
        cmd = ["adb", "shell", "input", "tap", "500 200"]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)
        time.sleep(3)
        
        logging.debug("Checking if wifi is enabled")
    
        if is_wifi_enabled():
            logging.debug("Wifi is already enabled")
        else:
            logging.debug("Enabling Wifi")
            cmd = ["adb", "shell", "svc", "wifi", "enable"]
            result = subprocess.run(cmd, stdout=subprocess.PIPE)
            time.sleep(3)
            logging.debug("Enabled Wifi")

        logging.debug("Enable WiFi Calling")
        cmd = ["adb", "shell", "am", "start", "-a", "android.intent.action.MAIN", "-n", "com.telephony.service/.wfc.WfcAliasActivity"]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)
        time.sleep(3)
    
        if is_vowifi_enabled():
            logging.debug("Vowifi already enabled")
        else:
            for _ in range(2): 
                logging.debug("Toggle the WiFi Calling button")
                cmd = ["adb", "shell", "input", "keyevent", "23"]
                result = subprocess.run(cmd, stdout=subprocess.PIPE)
                time.sleep(3)
            logging.debug("Finish toggling the WiFi Calling button")
        
    elif device == "A13_Pro":
        logging.debug("Swipe to unlock the phone")
        cmd = ["adb", "shell", "input", "keyevent", "82"]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)
        #cmd = ["adb", "shell", "input", "swipe", "200", "500", "200", "0"]
        #result = subprocess.run(cmd, stdout=subprocess.PIPE)
        logging.debug("Enable WiFi Calling")
        cmd = ["adb", "shell", "am", "start", "-a", "android.intent.action.MAIN", "-n", "com.android.settings/.wifi.calling.WifiCallingSuggestionActivity"]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)
        time.sleep(3)

        logging.debug("Toggle the WiFi Calling button")
        for _ in range(3):
            cmd = ["adb", "shell", "input", "keyevent", "23"]
            result = subprocess.run(cmd, stdout=subprocess.PIPE)
            time.sleep(3)
        logging.debug("Finish toggling the WiFi Calling button")
    
    elif device == "I14_Pro_Nax":
        logging.debug("Menu to unlock the phone")
        cmd = ["adb", "shell", "input", "keyevent", "82"]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)
        time.sleep(1)
            
        logging.debug("Enabling swipe")
        cmd = ["adb", "shell", "input", "swipe", "200", "500", "200", "0"]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)
        time.sleep(1)
        logging.debug("Enabled swipe")
        
        logging.debug("Enabling wifi")
        cmd = ["adb", "shell", "svc", "wifi", "enable"]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)
        time.sleep(3)
        logging.debug("Enabled wifi")

        logging.debug("Enable WiFi Calling")
        cmd = ["adb", "shell", "am", "start", "-a", "android.intent.action.MAIN", "-n", "com.android.settings/.wifi.calling.WifiCallingSuggestionActivity"]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)
        time.sleep(3)
        logging.debug("Enabled Wifi Calling")
 
        logging.debug("Toggle the WiFi Calling button")
        for _ in range(2):
            cmd = ["adb", "shell", "input", "keyevent", "23"]
            result = subprocess.run(cmd, stdout=subprocess.PIPE)
            time.sleep(3)
        logging.debug("Finish toggling the WiFi Calling button")

    elif device == "Nokia_G100":
        logging.debug("Menu to unlock the phone")
        cmd = ["adb", "shell", "input", "keyevent", "82"]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)
        time.sleep(3)

        logging.debug("Enabling swipe")
        cmd = ["adb", "shell", "input", "swipe", "200", "500", "200", "0"]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)
        timedevice.sleep(3)
        logging.debug("Enabled Swipe")

        logging.debug("Enabling wifi")
        cmd = ["adb", "shell", "svc", "wifi", "enable"]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)
        time.sleep(4)
        logging.debug("Enabled wifi")

        logging.debug ("Enable WiFi Calling")
        cmd = ["adb", "shell", "am", "start", "-a", "android.intent.action.MAIN", "-n", "com.android.settings/.wifi.calling.WifiCallingSuggestionActivity"]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)
        time.sleep(3)
        logging.debug("Enabled Wifi Calling")

        logging.debug("Toggle the WiFi Calling button")
        for _ in range(2):
            cmd = ["adb", "shell", "input", "keyevent", "23"]
            result = subprocess.run(cmd, stdout=subprocess.PIPE)
            time.sleep(3)
        logging.debug("Finish toggling the WiFi Calling button")

    elif device == "Pixel_6a":
        logging.debug("Menu to unlock the phone")
        cmd = ["adb", "shell", "input", "keyevent", "82"]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)
        time.sleep(3)

        logging.debug("Enabling swipe")
        cmd = ["adb", "shell", "input", "swipe", "200", "500", "200", "0"]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)
        time.sleep(3)
        logging.debug("Enabled Swipe")

        logging.debug("Enabling wifi")
        cmd = ["adb", "shell", "svc", "wifi", "enable"]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)
        time.sleep(4)
        logging.debug("Enabled wifi")

        logging.debug ("Enable WiFi Calling")
        cmd = ["adb", "shell", "am", "start", "-a", "android.intent.action.MAIN", "-n", "com.android.settings/.wifi.calling.WifiCallingSuggestionActivity"]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)
        time.sleep(3)
        logging.debug("Enabled Wifi Calling")

        logging.debug("Toggle the WiFi Calling button")
        for _ in range(2):
            cmd = ["adb", "shell", "input", "keyevent", "23"]
            result = subprocess.run(cmd, stdout=subprocess.PIPE)
            time.sleep(3)
        logging.debug("Finish toggling the WiFi Calling button")


def handle_client_connection(client, server, device):
    logging.info("Client handler initiated")
    count_enable_vowifi = 0

    try:
        while True:
            opcode = ""
            while not opcode.endswith("\n"):
                rcvd = client.recv(1)
                opcode += rcvd.decode()
            opcode = opcode.strip()
            logging.info("Received opcode: {}".format(opcode))

            ue_wakeup(device)
            if opcode == "reset":
                handle_reset(client, device)
            elif opcode == "device":
                name = device
                if name == "B15 model":
                    name = "NUU_B15"
                elif name == "LM_Q730":
                    name = "LG_Stylo_6"
                elif name == "Note_14":
                    name = "Ulefone_Note_14"
                logging.info("UE model name to be sent: {}".format(name))
                client.send("{}\n".format(name).encode())
            elif opcode == "ue_reboot":
                handle_ue_reboot(client, device)
            elif opcode == "adb_server_restart":
                handle_adb_server_restart(client)
            elif opcode == "wifi_off":
                handle_turn_off_wifi_interface(device)
                client.send(ACK)
            elif opcode == "wifi_on":
                handle_turn_on_wifi_interface(device)
                client.send(ACK)
            else:
                logging.info("Invalid opcode: {}".format(opcode))
    except KeyboardInterrupt:
        logging.info("Keyboard Interrupt")
        logging.info("Closing the connecting socket ...")
        client.close()
        handle_turn_off_wifi_interface(device)

    except:
        logging.error("Error occurred")
        logging.error("Closing the connecting socket ...")
        client.close()
        handle_turn_off_wifi_interface(device)

def check_device_model():
    result = subprocess.run(['adb', 'devices', '-l'], stdout=subprocess.PIPE)
    output = result.stdout.decode()

    if "G920T" in output:
        device = "SM_G920T"
        logging.info("Device model: Samsung Galaxy S6 G920T")
    elif "SM_A21" in output:
        device = "SM_A21"
        logging.info("Device model: Samsung Galaxy A21")
    elif "5062W" in output:
        device = "T-mobile_Revvl4"
        logging.info("Device model: T-mobile Revvl4")
    elif "moto_e5_plus" in output:
        device = "moto_e5_plus"
        logging.info("Device model: Motorola Motoinput E5 Plus")
    elif "moto_g_power" in output:
        device = "moto_g_power"
        logging.info("Device model: Motorola Moto G Power")
    elif "LM_G900TM" in output:
        device = "LM_G900TM"
        logging.info("Device model: LG Velvet 5G")
    elif "LM_Q730" in output:
        device = "LM_Q730"
        logging.info("Device model: LG Stylo 6")
    elif "OnePlus7T" in output:
        device = "OnePlus7T"
        logging.info("Device model: OnePlus7T")
    elif "OnePlus9R" in output:
        device = "OnePlus9R"
        logging.info("Device model: OnePlus9R")
    elif "HTC_U11_life" in output:
        device = "HTC_U11"
        logging.info("Device model: HTC U11")
    elif "Blackview_A55" in output:
        device = "Blackview_A55"
        logging.info("Device model: A55")
    elif "Z8850K" in output:
        device = "ZTE_Stage_5G"
        logging.info("Device model: ZTE STAGE 5G")
    elif "A55" in output:
        device = "A55"
        logging.info("Device model: Blackview_A55")
    elif "A13_Pro" in output:
        device = "A13_Pro"
        logging.info("Device model: UMIDIGI A13 Pro")
    elif "Pixel_4a" in output:
        device = "Pixel_4a"
        logging.info("Device model: Google Pixel 4a")
    elif "Pixel_6a" in output:
        device = "Pixel_6a"
        logging.info("Device model: Google Pixel 6a")
    elif "SOV41" in output:
        device = "SOV41"
        logging.info("Device model: Xperia 5")
    elif "2201117TG" in output:
        device = "Redmi_Note2"
        logging.info("Device model: Redmi Note 2")
    elif "LG_Q730" in output:
        device = "LG_Q730"
        logging.info("Device model: LG Stylo 6")
    elif "T608M" in output:
        device = "TCL40XL"
        logging.info("Device model: TCL 40XL")
    elif "I14_Pro_max" in output:
        device = "I14_Pro_Nax"
        logging.info("Device model: Blackcyber I14 Pro Max")
    elif "I15_Pro_Max" in output:
        device = "I15_Pro_Max"
        logging.info("Device model: Blackcyber I15 Pro Max")
    elif "Note_14" in output:
        device = "Note_14"
        logging.info("Device model: Ulefone Note 14")
    elif "B15 model" in output:
        device = "B15 model"
        logging.info("Device model: NUU B15")
    elif "Nokia_G100" in output:
        device = "Nokia_G100"
        logging.info("Device model: Nokia G100")
    elif "CPH2459" in output:
        device = "One_Plus_Nord_N20"
        logging.info("Device mode: One Plus Nord N20")
    elif "motorola_edge_30_pro":
        device = "Motorola_Edge_30_Pro"
        logging.info("Device model: Motorola Edge 30 Pro")
    else:
        device = "others"
        logging.info("Device model: Others")

    return device

def command_line_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-a", "--addr", metavar="<listening address>", help="listening address", type=str, default="localhost")
    parser.add_argument("-p", "--port", metavar="<listening port number>", help="listening port number", type=int, default=7777)
    parser.add_argument("-l", "--log", metavar="<log level (DEBUG/INFO/WARNING/ERROR/CRITICAL>", help="Log level (DEBUG/INFO/WARNING/ERROR/CRITICAL)", type=str, default="INFO")
    args = parser.parse_args()
    return args

def main():
    args = command_line_args()
    logging.basicConfig(level=args.log)
    logging.info("UE Controller Server has started...")

    if os.geteuid() != 0:
        logging.error("The script should run with the root privilege")
        sys.exit(1)

    adb_server_restart()

    logging.info("Open the listening socket to receive the message to control the VoWiFi UE from the learner")

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((args.addr, args.port))

    server.listen(5)
    device = check_device_model()
    handle_turn_off_wifi_interface(device)
    handle_init_config(device)
    #ue_reboot(device)

    try:
        client, address = server.accept()
        logging.info("Accepted connection from {}:{}".format(address[0], address[1]))
        logging.info("Handling the connection with the client")
        handle_client_connection(client, server, device)
        logging.info("Closing the listening socket ...")
        server.close()
    except:
        logging.error("Error occurred")
        logging.error("Closing the listening socket ...")
        server.close()

if __name__ == '__main__':
    main()

