import os
import sys
import argparse
import logging
import time
import json
import copy
import random
from testcases import Testcases

def abstract_testcases(pname):
    ret = []
    lst = ["{}/{}".format(pname, f) for f in os.listdir(pname) if "swp" not in f and not os.path.isdir("{}/{}".format(pname, f))]
    cnt = 0

    for fname in lst:
        ret.append(Testcases(fname))
        cnt += 1

    logging.info("Abstract> {} primary properties are ready".format(cnt))
    return ret

def substitution(ptcs, ename):
    ret = {}
    ret["testcases"] = []

    errors = open(ename, "r").read()
    emsgs = json.loads(errors)["errors"]
    cnt = 0

    for ptc in ptcs:
        obj = ptc.get_default_object()
        testcases = obj["testcases"]

        tnum = 0
        for testcase in testcases:
            tc = testcase["testcase"]

            for emsg in emsgs:
                t = {}
                t["testcase"] = copy.deepcopy(tc[:-1])
                t["testcase"].append(emsg)
                tnum += 1
                t["testcase"][0]["id"] = "{}-{}".format(ptc.get_property_number(), tnum)
                ret["testcases"].append(t)
                cnt += 1

    logging.info("Substitution> {} testcases are generated".format(cnt))
    return ret

def replay(ptcs):
    ret = {}
    ret["testcases"] = []

    cnt = 0

    for ptc in ptcs:
        obj = ptc.get_default_object()
        testcases = obj["testcases"]

        tnum = 0
        for testcase in testcases:
            tc = testcase["testcase"]

            t = {}
            t["testcase"] = copy.deepcopy(tc)
            r = copy.deepcopy(t["testcase"][-1])
            if "sub" not in r:
                r["sub"] = []
            s = {}
            s["name"] = "message_id"
            s["type"] = "string"
            s["value"] = "previous"
            r["sub"].append(s)

            t["testcase"].append(r)
            tnum += 1
            t["testcase"][0]["id"] = "{}-{}".format(ptc.get_property_number(), tnum)

            ret["testcases"].append(t)
            cnt += 1

    logging.info("Replay> {} testcases are generated".format(cnt))
    return ret

def _find_target(testcase, target):
    if isinstance(testcase, list):
        for msg in testcase:
            if "sub" in msg:
                ret = _find_target(msg["sub"], target)
                if ret != None:
                    return ret

            if msg["name"] == target:
                return msg
    return None

def update_values(ptcs):
    ret = {}
    ret["testcases"] = []
    cnt = 0

    for tc in ptcs:
        fname = tc.get_filename()
        obj = tc.get_default_object()
        targets = tc.get_targets()

        testcases = obj["testcases"]
        tnum = 0
        for testcase in testcases:
            for target in targets:
                pvals = tc.get_possible_values(target)
                cvals = tc.get_correct_values(target)
                avals = pvals - cvals
                msg = _find_target(testcase["testcase"], target)
                if msg != None:
                    for val in avals:
                        '''
                        if val == "min":
                            msg["value"] = 0
                        elif val == "max":
                            msg["value"] = 0xffff
                        elif val == "mean":
                            msg["value"] = 0xaaaa
                        elif val == "random":
                            msg["value"] = int(random.random())
                        '''
                        msg["value"] = val
                        msg["op"] = "update"
                        t = copy.deepcopy(testcase)
                        tnum += 1
                        t["testcase"][0]["id"] = "{}-{}".format(tc.get_property_number(), tnum)

                        ret["testcases"].append(t)
                        msg.pop("op", None)
                        cnt += 1

    logging.info("Update Values> {} testcases are generated".format(cnt))
    return ret

def _make_targets(testcase):
    ret = []
    if isinstance(testcase, list):
        msg = testcase[-1]
        if "sub" in msg:
            tmp = _make_targets(msg["sub"])
            for e in tmp:
                ret.append(e)

        if "value" in msg:
            ret.append(msg["name"])
    return ret

def drop_payloads(ptcs):
    ret = {}
    ret["testcases"] = []
    cnt = 0

    for ptc in ptcs:
        obj = ptc.get_default_object()
        testcases = obj["testcases"]
        tnum = 0

        for testcase in testcases:
            targets = _make_targets(testcase["testcase"])
            for target in targets:
                msg = _find_target(testcase["testcase"], target)
                if msg != None:
                    msg["op"] = "drop"
                    t = copy.deepcopy(testcase)
                    tnum += 1
                    t["testcase"][0]["id"] = "{}-{}".format(ptc.get_property_number(), tnum)

                    ret["testcases"].append(t)
                    msg.pop("op", None)
                    cnt += 1

    logging.info("Drop> {} testcases are generated".format(cnt))
    return ret

def message_level_manipulation(ptcs, ename):
    ret = {}
    ret["testcases"] = []

    lst1 = substitution(ptcs, ename)
    lst2 = replay(ptcs)

    for tc in lst1["testcases"]:
        ret["testcases"].append(tc)

    for tc in lst2["testcases"]:
        ret["testcases"].append(tc)

    return ret 

def attribute_level_manipulation(ptcs):
    ret = {}
    ret["testcases"] = []

    lst1 = update_values(ptcs)
    lst2 = drop_payloads(ptcs)

    for tc in lst1["testcases"]:
        ret["testcases"].append(tc)

    for tc in lst2["testcases"]:
        ret["testcases"].append(tc)
    
    return ret

def write_testcases(ofname, mtcs, atcs):
    testcases = {}
    testcases["testcases"] = []

    for tc in mtcs["testcases"]:
        testcases["testcases"].append(tc)

    for tc in atcs["testcases"]:
        testcases["testcases"].append(tc)

    with open(ofname, "w") as of:
        of.write(json.dumps(testcases, indent=2))

def generate_testcases(pname, ename, ofname):
    ptcs = abstract_testcases(pname)
    mtcs = message_level_manipulation(ptcs, ename)
    atcs = attribute_level_manipulation(ptcs)
    write_testcases(ofname, mtcs, atcs)

def command_line_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--primary", metavar="<directory of primary properties>", help="Directory of Primary Properties", type=str, default="../testcases/primary")
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
