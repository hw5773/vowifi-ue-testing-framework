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
REBOOT_WAIT_TIME = 5

def handle_turn_off_wifi_interface(device):
    if device == "SM_G920T":
        cmd = ["adb", "shell", "su", "-c", "svc", "wifi", "disable"]
        subprocess.run(cmd)
    elif device == "moto_e5_plus" or device == "HTC_U11" or device == "Blackview_A55" or device == "OnePlus Nord N20":
        cmd = ["adb", "shell", "dumpsys", "wifi", "|", "grep", "\"Wi-Fi is\""]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)

        if "enabled" in result.stdout.decode():
            logging.info("WiFi is enabled: need to toggle the WiFi button")
            ue_wakeup(device)

            cmd = ["adb", "shell", "am", "start", "-a", "android.intent.action.MAIN", "-n", "com.android.settings/.wifi.WifiSettings"]
            output = subprocess.run(cmd, stdout=subprocess.PIPE)
            time.sleep(1)

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
    elif device == "moto_e5_plus" or device == "HTC_U11" or device == "Blackview_A55" or device =="OnePlus Nord N20":
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
    start = int(time.time())
    while True:
        cmd = ["adb", "devices", "-l"]
        result = subprocess.run(cmd, stdout=subprocess.PIPE)
        if device in result.stdout.decode():
            logging.info("UE reboot success")
            client.send(ACK)
            break
        curr = int(time.time())
        if curr - start >= REBOOT_TIMEOUT:
            logging.info("UE reboot failure: timeout")
            client.send(FAIL)
            break
        time.sleep(5)
    time.sleep(REBOOT_WAIT_TIME)

def handle_adb_server_restart(client):
    adb_server_restart()
    time.sleep(1)
    client.send(ACK)

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
                logging.info("UE model name to be sent: {}".format(device))
                client.send("{}\n".format(device).encode())
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
        device = "5062W"
        logging.info("Device model: T-mobile Revvl4")
    elif "moto_e5_plus" in output:
        device = "moto_e5_plus"
        logging.info("Device model: Motorola Moto E5 Plus")
    elif "moto_g_power" in output:
        device = "moto_g_power"
        logging.info("Device model: Motorola Moto G Power")
    elif "LM_G900TM" in output:
        device = "LM_G900TM"
        logging.info("Device model: LG Velvet 5G")
    elif "OnePlus7T" in output:
        device = "OnePlus7T"
        logging.info("Device model: OnePlus7T")
    elif "HTC_U11" in output:
        device = "HTC_U11"
        logging.info("Device model: HTC U11")
    elif "Blackview_A55" in output:
        device = "Blackview_A55"
        logging.info("Device model: A55")
    elif "OnePlus Nord N20" in output:
        device = "OnePlus Nord N20"
        logging.info("Device model: CPH2459")
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

