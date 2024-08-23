import datetime

class calc_point:
    def __init__(self,classes):
        self.classes = classes
        self.classes_point = {}
        for i in classes.values():
            for j in i:
                self.classes_point[j] = 20

    def calc(self,person_num,air_switch):
        current_time = datetime.datetime.now()
        if int(current_time.strftime('%H')) < 16:
            return self.classes_point
        
        for i in self.classes.values():
            count_flag = 0
            for j in i:
                if air_switch[j] == 1:
                    self.classes_point[j] = self.classes_point[j] - 1
                    count_flag = count_flag + 1
            
            if count_flag > 1:
                for j in i:
                    if air_switch[j] == 1:
                        self.classes_point[j] = self.classes_point[j] - 2

            for j in i:
                if person_num[j] > 9:
                    self.classes_point[j] = self.classes_point[j] + 1
                    


        return  self.classes_point