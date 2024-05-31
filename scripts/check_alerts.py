import os
import sys
import argparse
import logging
import time
import json
import copy

def check(ldir, ofname):
    devices = ["{}/{}".format(ldir, d) for d in os.listdir(ldir) if os.path.isdir("{}/{}".format(ldir, d))]
    of = open(ofname, "w")
    for dev in devices:
        device = dev.split("/")[-1]
        of.write("===== {} =====\n".format(device))
        results = sorted(["{}/{}".format(dev, f) for f in os.listdir(dev) if "swp" not in f])
        for result in results:
            logging.info("result: {}".format(result))
            with open(result, "r") as f:
                for line in f:
                    line = line.strip()
                    if "Oracle" in line:
                        k, v = line.split(": ")
                        if "Functional" in k:
                            if "negative" not in v:
                                logging.info("{}: function oracle alert".format(result))
                                of.write("{}: Functional Oracle: {}\n".format(result, v))

                        if "Liveness" in k:
                            if "negative" not in v:
                                logging.info("{}: liveness oracle alert".format(result))
                                of.write("{}: Liveness Oracle: {}\n".format(result, v))
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

    check(args.log_directory, args.output)

if __name__ == "__main__":
    main()
