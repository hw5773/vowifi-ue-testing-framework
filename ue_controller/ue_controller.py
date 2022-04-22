import sys
import time
import socket
import subprocess
import threading
import os
import logging
import argparse
import signal

def get_opcodes():
    ret = {}
    ret[1] = "reset"
    ret[2] = "ue reboot"
    ret[3] = "adb server reboot"
    return ret

def turn_off_wifi_interface():
    cmd = ["adb", "shell", "svc", "wifi", "disable"]
    subprocess.run(cmd)
    time.sleep(1)

def turn_on_wifi_interface():
    cmd = ["adb", "shell", "svc", "wifi", "enable"]
    subprocess.run(cmd)
    time.sleep(1)

def ue_reboot():
    cmd = ["adb", "reboot"]
    subprocess.run(cmd)
    time.sleep(1)

def adb_server_restart():
    cmd = ["adb", "kill-server"]
    subprocess.run(cmd)

    cmd = ["adb", "start-server"]
    subprocess.run(cmd)

def handle_reset(client):
    turn_off_wifi_interface()
    turn_on_wifi_interface()
    time.sleep(1)
    ack = 1
    client.send(ack.to_bytes(4, 'big', signed=True))

def handle_ue_reboot(client):
    ue_reboot()
    time.sleep(30)
    ack = 1
    client.send(ack.to_bytes(4, 'big', signed=True))

def handle_adb_server_restart(client):
    adb_server_restart()
    time.sleep(1)
    ack = 1
    client.send(ack.to_bytes(4, 'big', signed=True))

def handle_client_connection(client, server):
    logging.info("Client Handler initiated")
    opcodes = get_opcodes()

    try:
        while True:
            rcvd = client.recv(4)
            opcode = int.from_bytes(rcvd, 'big', signed=True)
            if opcode in opcodes:
                logging.info("Received opcode: {} ({})".format(opcode, opcodes[opcode]))

                if opcode == 1:
                    handle_reset(client)

                elif opcode == 2:
                    handle_ue_reboot(client)

                elif opcode == 3:
                    handle_adb_server_restart(client)

    except KeyboardInterrupt:
        logging.info("Keyboard Interrupt")
        logging.info("Closing the connection ...")
        client_socket.close()
        turn_off_wifi_interface()

    except:
        logging.error("Error Occurred")
        logging.error("Closing the connection ...")
        client_socket.close()
        turn_off_wifi_interface()

    time.sleep(1)

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
    time.sleep(1)

    logging.info("Open the listening socket to receive the message to control the VoWiFi UE from the learner")

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((args.addr, args.port))

    server.listen(5)  # max backlog of connections

    try:
        client, address = server.accept()
        logging.info("Accepted connection from {}:{}".format(address[0], address[1]))
        logging.info("Handling Client Connection")
        handle_client_connection(client, server)
        logging.info("Closing the listening socket ...")
        server.close()
    except KeyboardInterrupt:
        logging.info("Keyboard Interrupt")
        logging.info("Closing the connection ...")
        server.close()
    except:
        logging.error("Error Occurred")
        logging.error("Closing the connection ...")
        server.close()

if __name__ == '__main__':
    main()

