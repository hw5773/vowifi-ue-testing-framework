class Student:
    def __init__(self, name, gender, sid, gpa):
        self.name = name
        self.gender = gender
        self.student_id = sid
        self.gpa = gpa

    def set_name(self, name):
        self.name = name

    def get_name(self):
        return self.name

    def set_gender(self, gender):
        self.gender = gender

    def get_gender(self):
        return self.gender

    def set_student_id(self, sid):
        self.student_id = sid

    def get_student_id(self):
        return self.student_id

    def set_gpa(self, gpa):
        self.gpa = gpa

    def get_gpa(self):
        return self.gpa

    def print_info(self):
        print ("Name: {}, Gender: {}, ID: {}, GPA: {}".format(self.name, self.gender, self.student_id, self.gpa))
