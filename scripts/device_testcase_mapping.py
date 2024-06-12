import os
import sys
import argparse
import logging
import time
import json
import copy

def check(ldir, ofname):
    results = ["{}/{}".format(ldir, f) for f in os.listdir(ldir) if os.path.isfile("{}/{}".format(ldir, f)) and ".txt" in f]
    functional = {}
    liveness = {}

    for result in results:
        with open(result, "r") as f:
            uename = None
            for line in f:
                if "===== " in line:
                    tmp = line.strip().split(" ")
                    tmp = tmp[1:-1]
                    uename = ' '.join(tmp)
                    continue

                if "==========" in line:
                    continue

                tmp = line.strip().split(": ")
                testcase = tmp[0].strip()
                testcase = testcase.split("/")[-1].split(".")[-3]
                oracle = tmp[1].strip()

                if "Functional" in oracle:
                    if testcase not in functional:
                        functional[testcase] = set([])
                    functional[testcase].add(uename)

                if "Liveness" in oracle:
                    if testcase not in liveness:
                        liveness[testcase] = set([])
                    liveness[testcase].add(uename)

    with open(ofname, "w") as of:
        keys = functional.keys()
        of.write("Result: the functional oracle\n")
        for key in keys:
            lst = list(functional[key])
            line = "{},{}\n".format(key, ','.join(lst))
            of.write(line)

        keys = liveness.keys()
        of.write("\nResult: the liveness oracle\n")
        for key in keys:
            lst = list(liveness[key])
            line = "{},{}\n".format(key, ",".join(lst))
            of.write(line)

def command_line_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", "--log-directory", metavar="<log directory>", help="Log directory", type=str, required=True)
    parser.add_argument("-o", "--output", metavar="<output filename>", help="Output filename", type=str, default="device_testcase_mapping")
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
