import json

class Testcases:

    def __init__(self, fname):
        self.fname = fname
        self.targets = []
        self.possible_values = {}
        self.correct_values = {}
        self.pnum = int(fname.split("/")[-1].split(".")[0])
        self.state = None
        self.message = None
        target = None
        names = []
        self.id = None
        messages = ["enable_vowifi", "ike_sa_init_response", "ike_auth_1_response", "ike_auth_2_response", "ike_auth_3_response", "401_unauthorized", "200_ok"]

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

                    target = target.strip()
                    if target[0] == '\"':
                        target = target[1:]
                    if target[-1] == '\"':
                        target = target[:-1]
                    names.append(target)

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

            print ("js: {}".format(js))
            self.default_object = json.loads(js)

            tmp = []
            for name in names:
                if name in messages:
                    self.message = name
                else:
                    tmp.append(name)
            self.id = "{}-{}".format(self.message, '-'.join(tmp))

            print ("target message: {}".format(self.message))
            print ("id: {}".format(self.id))
            

    def get_filename(self):
        return self.fname

    def get_id(self):
        return self.id

    def get_message_name(self):
        return self.message

    def get_default_object(self):
        return self.default_object

    def get_targets(self):
        return self.targets

    def get_possible_values(self, target):
        return self.possible_values[target]

    def get_correct_values(self, target):
        return self.correct_values[target]

    def get_property_number(self):
        return self.pnum
