import json

class Testcases:

    def __init__(self, fname):
        self.fname = fname
        self.target = ""
        self.possible_values = set([])
        self.correct_values = set([])
        target = None

        with open(fname, "r") as f:
            js = ""
            for line in f:
                line = line.strip()

                if not line:
                    continue 

                if line[0] == '#':
                    continue

                if "name" in line:
                    _, target = line.split(":")
                    target = target.strip()
                    if target[-1] == ',':
                        target = target[:-1]

                if "VALUE" in line:
                    if line[-1] == ',':
                        comma = True
                    else:
                        comma = False

                    _, values = line.split(":")
                    self.target = target

                    if "(" in values:
                        args = values[values.index("(")+1 : values.index(")")]
                        pvals = args[args.index("[")+1 : args.index("]")].split(",")
                        for v in pvals:
                            self.possible_values.add(int(v))

                        tmp = args[args.index("]")+1:].strip()
                        if len(tmp) > 0:
                            cvals = tmp[tmp.index("[")+1 : tmp.index("]")].split(",")
                            for v in cvals:
                                self.correct_values.add(int(v))
                        else:
                            cvals = []
                    else:
                        self.possible_values.add("min")
                        self.possible_values.add("median")
                        self.possible_values.add("max")
                        self.possible_values.add("mean")
                        self.possible_values.add("random")

                    line = "\"value\": 0"

                js += line

            self.default_object = json.loads(js)

    def get_default_object(self):
        return self.default_object

    def get_target(self):
        return self.target

    def get_possible_values(self):
        return self.possible_values

    def get_correct_values(self):
        return self.correct_values
