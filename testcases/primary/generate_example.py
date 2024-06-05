import os
import sys
import json
lst = [f for f in os.listdir() if "property" in f and "swp" not in f]
lst = sorted(lst, key=lambda x:int(x.split(".")[0]))
total = len(lst)
idx = 0
ofname = "example.json"

out = {}
out["testcases"] = []
for fname in lst:
    lines = ""
    print ("{} / {}".format(idx, total))
    with open(fname, "r") as f:
        try:
            for line in f:
                if "#" in line:
                    continue
                if "VALUE" in line:
                    try:
                        tmp = line.split(":")
                        tmp[1] = " 0\n"
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

    for tc in js["testcases"]:
        testcase = tc["testcase"]
        for msg in testcase:
            if "op" in msg:
                msg["op"] = "update"
            if "sub" in msg:
                for sub in msg["sub"]:
                    if "op" in sub:
                        sub["op"] = "update"
                    if "sub" in sub:
                        for ssub in sub["sub"]:
                            if "op" in ssub:
                                ssub["op"] = "update"
                            if "sub" in ssub:
                                for sssub in ssub["sub"]:
                                    if "op" in sssub:
                                        sssub["op"] = "update"
                                    if "sub" in sssub:
                                        for ssssub in sssub["sub"]:
                                            if "op" in ssssub:
                                                ssssub["op"] = "update"

        out["testcases"].append(tc)

with open(ofname, "w") as of:
    of.write(json.dumps(out, indent=2))
