import argparse
import os
import json
import logging

def parsing(line):
    line = line.strip()
    idx = line.find(" ")
    js = json.loads(line[idx:].strip()[1:-1])
    for k in js:
        if js[k]["activated"] == True:
            print ("{}: {}".format(k, js[k]))
    print ("")

def read_setting(fname):
    with open(fname, "r") as f:
        read_next = False
        for line in f:
            if "EF.IST" in line:
                read_next = True
                continue

            if "EF.UST" in line:
                read_next = True
                continue

            if read_next:
                if "update" in line:
                    parsing(line)
                read_next = False

def command_line_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--filename", required=True, metavar="<filename to be read>", help="Filename to be read", type=str)
    parser.add_argument("-l", "--log", metavar="<log level=DEBUG/INFO/WARNING/ERROR>", help="Log level (DEBUG/INFO/WARNING/ERROR)", type=str, default="INFO")
    args = parser.parse_args()
    return args

def main():
    args = command_line_args()
    logging.basicConfig(level=args.log)

    read_setting(args.filename)

if __name__ == "__main__":
    main()
