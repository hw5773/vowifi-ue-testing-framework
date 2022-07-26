import os
import sys
import argparse
import logging
import time
import json
from testcases import Testcases

def abstract_testcases(pname):
    ret = []
    lst = ["{}/{}".format(pname, f) for f in os.listdir(pname) if "swp" not in f]

    for fname in lst:
        ret.append(Testcases(fname))

    return ret

def substitution(ptcs, ename):
    ret = []

    errors = open(ename, "r").read()
    emsgs = json.loads(errors)

    for msg in emsgs["errors"]:
        print (msg)

    return ret

def replay(ptcs):
    ret = []
    return ret

def update_values(ptcs):
    ret = []
    return ret

def drop_payloads(ptcs):
    ret = []
    return ret

def message_level_manipulation(ptcs, ename):
    lst1 = substitution(ptcs, ename)
    lst2 = replay(ptcs)

    ret = lst1.append(lst2)
    return ret 

def attribute_level_manipulation(ptcs):
    lst1 = update_values(ptcs)
    lst2 = drop_payloads(ptcs)
    
    ret = lst1.append(lst2)
    return ret

def write_testcases(ofname, mtcs, atcs):
    testcases = {}
    testcases["testcases"] = []

    with open(ofname, "w") as of:
        of.write(json.dumps(testcases))

def generate_testcases(pname, ename, ofname):
    ptcs = abstract_testcases(pname)
    mtcs = message_level_manipulation(ptcs, ename)
    atcs = attribute_level_manipulation(ptcs)
    write_testcases(ofname, mtcs, atcs)

def command_line_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--primary", metavar="<directory of primary properties>", help="Directory of Primary Properties", type=str, required=True)
    parser.add_argument("-e", "--errors", metavar="<error messages file>", help="Error Message File", type=str, default="../testcases/errors/errors")
    parser.add_argument("-d", "--output-directory", metavar="<output directory>", help="Output directory", type=str, default="../testcases")
    parser.add_argument("-o", "--output", metavar="<output filename>", help="Output filename", type=str, default="testcases.json")
    parser.add_argument("-l", "--log", metavar="<log level (DEBUG/INFO/WARNING/ERROR/CRITICAL>", help="Log level (DEBUG/INFO/WARNING/ERROR/CRITICAL)", type=str, default="INFO")
    args = parser.parse_args()
    return args

def main():
    args = command_line_args()
    log_level = args.log
    logging.basicConfig(level=log_level)

    if not os.path.exists(args.primary):
        logging.error("No directory exists: {}".format(args.primary))
        sys.exit(1)

    if not os.path.exists(args.errors):
        logging.error("No error message exists: {}".format(args.errors))
        sys.exit(1)

    if not os.path.exists(args.output_directory):
        os.mkdir(args.output_directory)

    ofname = "{}/{}".format(args.output_directory, args.output)
    logging.info(">>>> Output filename would be {}".format(ofname))

    generate_testcases(args.primary, args.errors, ofname)

if __name__ == "__main__":
    main()
