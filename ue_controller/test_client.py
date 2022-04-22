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

def command_line_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-a", "--addr", metavar="<connecting address>", help="connecting address", type=str, default="localhost")
    parser.add_argument("-p", "--port", metavar="<connecting port number>", help="connecting port number", type=int, default=7777)
    parser.add_argument("-c", "--cmd", metavar="<command>", help="command", type=int, required=True)
    parser.add_argument("-l", "--log", metavar="<log level (DEBUG/INFO/WARNING/ERROR/CRITICAL>", help="Log level (DEBUG/INFO/WARNING/ERROR/CRITICAL)", type=str, default="INFO")
    args = parser.parse_args()
    return args

def main():
    args = command_line_args()
    logging.basicConfig(level=args.log)
    logging.info("UE Controller Test Client has started...")

    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((args.addr, args.port))
    opcodes = get_opcodes()
    
    if args.cmd not in opcodes:
        logging.error("Invalid opcode: {}".format(args.cmd))
        sys.exit(1)

    cmd = args.cmd

    try:
        logging.info("Send opcode: {} ({})".format(cmd, opcodes[cmd]))
        client.send(cmd.to_bytes(4, 'big', signed=True))

        response = client.recv(4)
        logging.info("Response: {}".format(response))
    except:
        logging.error("Error Occurred")
        logging.error("Closing the connection ...")

    client.close()

if __name__ == '__main__':
    main()

