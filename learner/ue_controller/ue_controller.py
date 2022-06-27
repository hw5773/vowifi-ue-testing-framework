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

def turn_off_wifi_interface(device):
    if device == "SM_G920T":
        cmd = ["adb", "shell", "su", "-c", "svc", "wifi", "disable"]
        subprocess.run(cmd)
    elif device == "moto_e5_plus":
        cmd = ["adb", "shell", "dumpsys", "wifi", "|", "grep", "\"Wi-Fi is\""]
        result = subprocess.run(cmd, stdout=subprocess.PIPE, text=True)

        if "enabled" in result.stdout:
            logging.info("WiFi is enabled: need to toggle the WiFi button")
            cmd = ["adb", "shell", "am", "start", "-a", "android.intent.action.MAIN", "-n", "com.android.settings/.wifi.WifiSettings"]
            subprocess.run(cmd)
            time.sleep(1)

            cmd = ["adb", "shell", "input", "keyevent", "20"]
            subprocess.run(cmd)
            time.sleep(1)

            cmd = ["adb", "shell", "input", "keyevent", "23"]
            subprocess.run(cmd)
            time.sleep(1)
        else:
            logging.info("WiFi is enabled: need not to toggle the WiFi button")
    else:
        cmd = ["adb", "shell", "svc", "wifi", "disable"]
        subprocess.run(cmd)
    time.sleep(1)

def turn_on_wifi_interface(device):
    if device == "SM_G920T":
        cmd = ["adb", "shell", "su", "-c", "svc", "wifi", "enable"]
        subprocess.run(cmd)
    elif device == "moto_e5_plus":
        cmd = ["adb", "shell", "dumpsys", "wifi", "|", "grep", "\"Wi-Fi is\""]
        result = subprocess.run(cmd, stdout=subprocess.PIPE, text=True)

        if "disabled" in result.stdout:
            logging.info("WiFi is disabled: need to toggle the WiFi button")
            cmd = ["adb", "shell", "am", "start", "-a", "android.intent.action.MAIN", "-n", "com.android.settings/.wifi.WifiSettings"]
            subprocess.run(cmd)
            time.sleep(1)

            cmd = ["adb", "shell", "input", "keyevent", "20"]
            subprocess.run(cmd)
            time.sleep(1)

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
    result = subprocess.run(cmd, stdout=subprocess.PIPE, text=True)
    cmd = ["adb", "shell", "input", "keyevent", "82"]
    result = subprocess.run(cmd, stdout=subprocess.PIPE, text=True)

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
    turn_off_wifi_interface(device)
    logging.debug("After turning off the wifi interface")
    logging.debug("Before turning on the wifi interface")
    turn_on_wifi_interface(device)
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
        result = subprocess.run(cmd, stdout=subprocess.PIPE, text=True)
        if device in result.stdout:
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
    logging.info("Client Handler initiated")
    count_enable_vowifi = 0

    try:
        while True:
            opcode = ""
            while not opcode.endswith("\n"):
                rcvd = client.recv(1)
                opcode += rcvd.decode()
            opcode = opcode.strip()
            logging.info("Received opcode: {}".format(opcode))

            if opcode == "reset":
                handle_reset(client, device)
            elif opcode == "ue_reboot":
                handle_ue_reboot(client, device)
            elif opcode == "adb_server_restart":
                handle_adb_server_restart(client)
            else:
                logging.info("Invalid opcode: {}".format(opcode))

    except KeyboardInterrupt:
        logging.info("Keyboard Interrupt")
        logging.info("Closing the connecting socket ...")
        client.close()
        turn_off_wifi_interface()

    except:
        logging.error("Error occurred")
        logging.error("Closing the connecting socket ...")
        client.close()
        turn_off_wifi_interface()

def check_device_model():
    result = subprocess.run(['adb', 'devices', '-l'], stdout=subprocess.PIPE, text=True)
    output = result.stdout

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
    elif "LM_G900TM" in output:
        device = "LM_G900TM"
        logging.info("Device model: LG Velvet 5G")
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
    turn_off_wifi_interface(device)

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

