import os
import sys
import argparse
import logging
import time
import json
import copy

def check(ldir, ofname):
    result = {}
    messages = ["ike_sa_init_response", "ike_auth_1_response", "ike_auth_2_response", "ike_auth_3_response", "401_unauthorized"]
    devices = ["{}/{}".format(ldir, d) for d in os.listdir(ldir) if os.path.isdir("{}/{}".format(ldir, d)) and d != "__pycache__"]
    of = open(ofname, "w")
    for dev in devices:
        device = dev.split("/")[-1]
        result[device] = {}

        lst = [f for f in os.listdir(dev) if f.startswith("result")]
        for m in messages:
            num = 0
            enums = []
            for f in lst:
                if m in f:
                    tmp = f.split("/")[-1].split(".")
                    enum = int(tmp[2])
                    if enum not in enums:
                        enums.append(enum)
                        num += 1
            result[device][m] = num
            logging.info("device: {} / m: {} / num: {}".format(device, m, result[device][m]))

    for device in result:
        logging.info("Device name: {}".format(device))
        
        s = 0
        for msg in messages:
            logging.info(" - {}: {}".format(msg, result[device][m]))
            s += result[device][m]

        logging.info(" - Total: {}".format(s))

def command_line_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", "--log-directory", metavar="<log directory>", help="Log directory", type=str, required=True)
    parser.add_argument("-o", "--output", metavar="<output filename>", help="Output filename", type=str, default="result.txt")
    parser.add_argument("-l", "--log", metavar="<log level (DEBUG/INFO/WARNING/ERROR/CRITICAL>", help="Log level (DEBUG/INFO/WARNING/ERROR/CRITICAL)", type=str, default="INFO")
    args = parser.parse_args()
    return args

def main():
    args = command_line_args()
    log_level = args.log
    logging.basicConfig(level=log_level)

    if not os.path.exists(args.log_directory):
        logging.error("No directory exists: {}".format(args.log_directory))
        sys.exit(1)

    ts = int(time.time())
    ofname = "{}.{}".format(args.output, ts)

    check(args.log_directory, ofname)

if __name__ == "__main__":
    main()
