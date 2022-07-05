import os
import sys
import re
import PyPDF2
import argparse
import logging

def get_regex():
    regex = r"([^.]*?MUST[^.]*\.)"
    r = re.compile(regex)
    return r

def extract_property(fname):
    regex = get_regex()
    f = PyPDF2.PdfFileReader(fname)
    npages = f.getNumPages()
    for i in range(1, npages):
        page = f.getPage(i)
        content = page.extractText()
        properties = re.findall(regex, content)
        print ("===== Page {} =====".format(i))
        for p in properties:
            p = p.replace("\n", "")
            print (p)

def command_line_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--file", metavar="<pdf file path to be loaded>", help="Specification file path", type=str, required=True)
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

    extract_property(args.file)

if __name__ == "__main__":
    main()
