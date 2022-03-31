import os
import sys
import argparse
import logging

def translate(kname):
    keys = []
    with open(kname, "r") as f:
        for line in f:
            if line[0:2] != "1:" and line[0:2] != "2:":
                continue

            t, l, v = line.strip().split(":")
            t = int(t)
            l = int(l)

            if l > 32:
                continue

            v = v.split(",")
            if v in keys:
                continue
            else:
                keys.append(v)

            if t == 1:
                print ("Encryption Key ({} bytes): ".format(l))
            elif t == 2:
                print ("Integrity Key ({} bytes): ".format(l))

            for i in range(l):
                print ("{:02x}".format(int(v[i])), end='')
            print ("")

def command_line_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-k", "--key", metavar="<key file name>", help="Key File Name", type=str, required=True)
    parser.add_argument("-o", "--output", metavar="<output file name>", help="Output File Name", type=str)
    parser.add_argument("-l", "--log", metavar="<log level (DEBUG/INFO/WARNING/ERROR/CRITICAL)>", help="Log level (DEBUG/INFO/WARNING/ERROR/CRITICAL)", type=str)
    args = parser.parse_args()
    return args

def main():
    args = command_line_args()
    logLevel = args.log
    logging.basicConfig(level=logLevel)

    if not os.path.exists(args.key):
        logging.error("File not exists: {}".format(args.key))
        sys.exit(1)

    translate(args.key)

if __name__ == "__main__":
    main()
