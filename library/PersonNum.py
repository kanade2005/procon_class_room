import torch

class PersonNum:

    def __init__(self):
        pass

    def predict_person_count(self,img):
        # yolo v5 setup
        model = torch.hub.load("ultralytics/yolov5", "yolov5s")

        # predict -> save img -> count person label
        results = model(img)
        #results.save()
        df = results.pandas().xyxy[0]
        return len(df[df["name"] == "person"])