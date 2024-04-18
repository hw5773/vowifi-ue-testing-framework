import argparse
import logging
import os, sys
from student import Student

def scoring(dname):
    logging.info("Data filename: {}".format(dname))

    students = []
    with open(dname, "r") as f:
        f.readline()
        for line in f:
            logging.info(line)
            tmp = line.strip().split(",")
            students.append(Student(tmp[0],tmp[1],tmp[2],tmp[3]))

    for student in students:
        student.print_info()

def command_line_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", "--data", metavar="<data file>", help="Data file", type=str, required=True)
    parser.add_argument("-l", "--log", metavar="<log level (DEBUG/INFO/WARNING/ERROR/CRITICAL>", help="Log level (DEBUG/INFO/WARNING/ERROR/CRITICAL)", type=str, default="INFO")
    args = parser.parse_args()
    return args

def main():
    args = command_line_args()
    log_level = args.log
    logging.basicConfig(level=log_level)

    if not os.path.exists(args.data):
        logging.error("No data file exists: {}".format(args.data))
        sys.exit(1)

    scoring(args.data)

if __name__ == "__main__":
    main()
