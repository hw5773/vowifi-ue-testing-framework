import os
import sys
import argparse
import logging
import time
import json
import copy

def check(ldir, ofname):
    devices = ["{}/{}".format(ldir, d) for d in os.listdir(ldir) if os.path.isdir("{}/{}".format(ldir, d)) and d != "__pycache__"]
    of = open(ofname, "w")
    for dev in devices:
        device = dev.split("/")[-1]
        of.write("===== {} =====\n".format(device))
        results = sorted(["{}/{}".format(dev, f) for f in os.listdir(dev) if "swp" not in f and "result" in f])
        for result in results:
            logging.info("result: {}".format(result))
            fname = result.split("/")[-1]
            msg = fname.split(".")[1].split("-")[1]
            msgs = ""
            with open(result, "r") as f:
                read = False
                for line in f:
                    if "Result:" in line:
                        read = False
                    if read:
                        msgs += line
                    if "Message:" in line:
                        read = True

            print ("msgs: {}".format(msgs))
            print ("msg: {} / msg not in msgs: {}".format(msg, msg not in msgs))
            if msg not in msgs:
                of.write("{}\n".format(fname))
        of.write("===============\n")
    of.close()

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
