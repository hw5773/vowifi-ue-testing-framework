import os
import sys
import argparse
import logging
import time
import json
import copy

def check(ldir, fname):
    with open(fname, "r") as f:
        dname = None
        for line in f:
            line = line.strip()
            if "===== " in line:
                tmp = line.split(" ")
                dname = tmp[1]
                continue

            if "==========" in line:
                continue

            lname = line
            fpath = "{}/{}/{}".format(ldir, dname, lname)

            try:
                os.remove(fpath)
            except:
                continue

def command_line_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", "--log-directory", metavar="<log directory>", help="Log directory", type=str, required=True)
    parser.add_argument("-i", "--input", metavar="<input filename>", help="Input filename", type=str, required=True)
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

    if not os.path.exists(args.input):
        logging.error("File not exists: {}".format(args.input))
        sys.exit(1)

    check(args.log_directory, args.input)

if __name__ == "__main__":
    main()
