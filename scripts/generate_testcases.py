import os
import sys
import argparse
import logging

def load_default(dname):
    pass

def load_critical_fields(fname):
    pass

def generate_testcase(pair, dobj):
    pass

def save_output(ofname, output):
    pass

def generate_testcases(default, fields, ike_errors, sip_errors, ofname):
    dobj = load_default(default)
    flst = load_critical_fields(fields)
    output = {}
    output["testcases"] = []

    for pair in flst:
        tcs = generate_testcase(pair, dobj)
        for tc in tcs:
            output["testcases"].append(tc)

    save_output(ofname, output)


def command_line_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--default", metavar="<default message>", help="Default message", type=str, default="default.json")
    parser.add_argument("-c", "--critical-fields", metavar="<critical fields>", help="Critical field", type=str, default="critical-fields")
    parser.add_argument("-e", "--ike-errors", metavar="<ike error messages file>", help="IKE Error Message File", type=str, default="../testcases/errors/ike_errors")
    parser.add_argument("-f", "--sip-errors", metavar="<sip error messages file>", help="SIP Error Message File", type=str, default="../testcases/errors/sip_errors")
    parser.add_argument("-d", "--output-directory", metavar="<output directory>", help="Output directory", type=str, default="../testcases")
    parser.add_argument("-o", "--output", metavar="<output filename>", help="Output filename", type=str, default="testcases.json")
    parser.add_argument("-l", "--log", metavar="<log level (DEBUG/INFO/WARNING/ERROR/CRITICAL>", help="Log level (DEBUG/INFO/WARNING/ERROR/CRITICAL)", type=str, default="INFO")
    args = parser.parse_args()
    return args

def main():
    args = command_line_args()
    log_level = args.log
    logging.basicConfig(level=log_level)

    if not os.path.exists(args.default):
        logging.error("No default message file exists: {}".format(args.default))
        sys.exit(1)

    if not os.path.exists(args.critical_fields):
        logging.error("No critical field file exists: {}".format(args.critical_fields))
        sys.exit(1)

    if not os.path.exists(args.ike_errors):
        logging.error("No IKE error message exists: {}".format(args.ike_errors))
        sys.exit(1)

    if not os.path.exists(args.sip_errors):
        logging.error("No SIP error message exists: {}".format(args.sip_errors))
        sys.exit(1)

    if not os.path.exists(args.output_directory):
        os.mkdir(args.output_directory)

    ofname = "{}/{}".format(args.output_directory, args.output)
    logging.info(">>>> Output filename would be {}".format(ofname))

    generate_testcases(args.default, args.critical_fields, args.ike_errors, args.sip_errors, ofname)

if __name__ == "__main__":
    main()
