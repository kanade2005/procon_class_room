import cv2
import numpy as np

class Light:
    def __init__(self):
        pass

    def is_light_on(self,image_path):
        # 画像を読み込む
        image = cv2.imread(image_path)

        # 画像のサイズを取得
        height, width = image.shape[:2]

        # 蛍光灯があると予想される領域を切り取る（画像上部を対象とする）
        top_region = image[0:int(height/4), :]  # 画像上部1/4の領域

        # グレースケールに変換
        gray_top_region = cv2.cvtColor(top_region, cv2.COLOR_BGR2GRAY)

        # 平均輝度を計算
        mean_brightness = np.mean(gray_top_region)

        # 閾値を設定して判定（ここでは150以上を明かりがついていると判定）
        if mean_brightness > 150:
            return True
        else:
            return False