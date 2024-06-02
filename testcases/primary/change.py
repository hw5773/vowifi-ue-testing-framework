import os

for i in range(71, 98):
    fname = "{}.property".format(i)
    tname = "{}.property".format(i+80)
    os.rename(fname, tname)
