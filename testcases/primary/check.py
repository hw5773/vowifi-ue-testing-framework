import os
import sys
import json
lst = [f for f in os.listdir() if "property" in f and "swp" not in f]
total = len(lst)
idx = 0
for fname in lst:
    lines = ""
    print ("{} / {}".format(idx, total))
    with open(fname, "r") as f:
        try:
            for line in f:
                if "#" in line:
                    continue
                if "\"op\"" in line:
                    if "[" not in line or "]" not in line:
                        print ("op error: {}".format(fname))
                        sys.exit(1)
                if "VALUE" in line:
                    try:
                        tmp = line.split(":")
                        tmp[1] = " \"none\"\n"
                        line = ":".join(tmp)
                    except Exception as error:
                        print ("index error: {}".format(fname))
                        print (error)
                lines += line
        except Exception as error:
            print ("decoding error: {}".format(fname))
            print (error)
    try:
        js = json.loads(lines)
    except Exception as error:
        print ("json error: {}".format(fname))
        print (lines)
        print (error)
        sys.exit(1)
    idx += 1
print ("{} / {}".format(idx, total))