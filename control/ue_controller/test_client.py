import sys
import time
import socket
import subprocess
import threading
import os
import logging
import argparse
import signal

def command_line_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-a", "--addr", metavar="<connecting address>", help="connecting address", type=str, default="localhost")
    parser.add_argument("-p", "--port", metavar="<connecting port number>", help="connecting port number", type=int, default=7777)
    parser.add_argument("-c", "--cmd", metavar="<command>", help="command", type=str, required=True)
    parser.add_argument("-l", "--log", metavar="<log level (DEBUG/INFO/WARNING/ERROR/CRITICAL>", help="Log level (DEBUG/INFO/WARNING/ERROR/CRITICAL)", type=str, default="INFO")
    args = parser.parse_args()
    return args

def main():
    args = command_line_args()
    logging.basicConfig(level=args.log)
    logging.info("UE Controller Test Client has started...")

    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((args.addr, args.port))
    
    cmd = args.cmd

    try:
        logging.info("Send opcode: {}".format(cmd))
        cmd = cmd + "\n"
        opcode = cmd.encode()
        client.send(opcode)

        response = ""
        while not response.endswith('\n'):
            rcvd = client.recv(1)
            response += rcvd.decode()
        logging.info("Response: {}".format(response))
    except:
        logging.error("Error Occurred")
        logging.error("Closing the connection ...")

    client.close()

if __name__ == '__main__':
    main()

