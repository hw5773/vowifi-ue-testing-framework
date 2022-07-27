import json

class Testcases:

    def __init__(self, fname):
        self.fname = fname
        self.targets = []
        self.possible_values = {}
        self.correct_values = {}
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
                    self.targets.append(target)

                    possible_values = set([])
                    correct_values = set([])
                    if "(" in values:
                        args = values[values.index("(")+1 : values.index(")")]
                        pvals = args[args.index("[")+1 : args.index("]")].split(",")
                        for v in pvals:
                            possible_values.add(int(v))
                        self.possible_values[target] = possible_values

                        tmp = args[args.index("]")+1:].strip()
                        if len(tmp) > 0:
                            cvals = tmp[tmp.index("[")+1 : tmp.index("]")].split(",")
                            for v in cvals:
                                correct_values.add(int(v))
                        else:
                            cvals = []
                    else:
                        possible_values.add("min")
                        possible_values.add("median")
                        possible_values.add("max")
                        possible_values.add("mean")
                        possible_values.add("random")

                    self.possible_values[target] = possible_values
                    self.correct_values[target] = correct_values

                    if len(correct_values) > 0:
                        line = "\"value\": {}".format(list(correct_values)[0])
                    else:
                        line = "\"value\": 0"

                js += line

            self.default_object = json.loads(js)

    def get_filename(self):
        return self.fname

    def get_default_object(self):
        return self.default_object

    def get_targets(self):
        return self.targets

    def get_possible_values(self, target):
        return self.possible_values[target]

    def get_correct_values(self, target):
        return self.correct_values[target]
