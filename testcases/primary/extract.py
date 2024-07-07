import os
lst = [f for f in os.listdir(".") if "property" in f]
lst = sorted(lst, key=lambda x: int(x.split(".")[0]))

with open("output.txt", "w") as of:
    for fname in lst:
        with open(fname, "r") as f:
            line = f.readline()
            num = int(fname.split(".")[0])
            line = "{}> {}".format(num, line)
            of.write(line)
