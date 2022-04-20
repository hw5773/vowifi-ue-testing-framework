import os
import sys
import argparse
import logging
import time

def extract_isakmp_keys(fname, last_line):
    nline = 0
    extract = False
    with open(fname, "r") as f:
        for line in f:
            nline += 1
            if nline < last_line:
                continue

            if extract:
                print (line.strip().split("  "))
                extract = False
            elif "Initiator SPI" in line:
                print (line.strip().split("] ")[1])
            elif "Responder SPI" in line:
                print (line.strip().split("] ")[1])
            elif "Sk_ai secret" in line or "Sk_ar secret" in line or "Sk_ei secret" in line or "Sk_er secret" in line:
                nbytes = int(line.strip().split("=>")[1].strip().split("bytes")[0].strip())
                extract = True

    return nline

def command_line_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--file", metavar="<syslog file path>", help="syslog filepath", type=str, required=True)
    parser.add_argument("-l", "--log", metavar="<log level (DEBUG/INFO/WARNING/ERROR/CRITICAL>", help="Log level (DEBUG/INFO/WARNING/ERROR/CRITICAL)", type=str, default="INFO")
    args = parser.parse_args()
    return args

def main():
    args = command_line_args()
    log_level = args.log
    logging.basicConfig(level=log_level)

    if not os.path.exists(args.file):
        logging.error("No file exists: {}".format(args.file))
        sys.exit(1)

    last_line = 0
    while True:
        last_line = extract_isakmp_keys(args.file, last_line)
        time.sleep(1)

if __name__ == "__main__":
    main()
