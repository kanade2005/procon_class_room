import datetime

class calc_point:
    def __init__(self,classes):
        self.classes = classes
        self.classes_point = {}
        for i in classes.values():
            for j in i:
                self.classes_point[j] = 30

    def calc(self,person_num,air_switch,light_switch,standard_temp,class_temp):
        current_time = datetime.datetime.now()
        #if int(current_time.strftime('%H')) < 16:
        #    return self.classes_point
        
        for i in self.classes.values():
            count_flag = 0
            for j in i:

                #エアコンがついているかついていないか確認　ついていない場合加点　ついている場合減点　人が多いかどうかも確認10人以上で減点を半分に
                if air_switch[j] == 1 or light_switch[j] == True:
                    self.classes_point[j] = self.classes_point[j] - abs(standard_temp - float(class_temp[j])) * 1.5
                    if person_num[j] < 10:
                        self.classes_point[j] = self.classes_point[j] - 5
                        self.classes_point[j] = self.classes_point[j] - 1.5 * person_num[j]

                    else:
                        self.classes_point[j] = self.classes_point[j] - 2
                
                else:
                    self.classes_point[j] = self.classes_point[j] + 5
                

                    


        return  self.classes_point