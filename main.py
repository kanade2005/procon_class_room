from flask import Flask
from flask import request, make_response, jsonify
from flask_cors import CORS
from library.picturerGet import PictureGet #画像をダウンロード メソッド→download(画像名) 先生のドライブから取得する
from library.spreadsheet import SpreadSheet #スプレッドシートのデータを取得() メソッド→get_latest_data(シート名,列数) 一番下の行のデータを取得
from library.PersonNum import PersonNum #AIを用いて人数取得　メソッド→predict_person_count(picture//画像の名前)
from library.temperature import Temperature #APIを用いて気温を取得する　メソッド→get_Temperature()
from library.point import calc_point #ポイントの計算をしている メソッド→calcで計算
from library.light_check import Light
import datetime
import mysql.connector

app = Flask(__name__)


#温度・人数・照明の有無を取得
@app.route('/data', methods = ['GET'])
def data():

    #クラスを保存する配列
    classes = ["1-1","1-2","1-3","1-4","2I","3I","4I","5I"]
    classes_group = {"1年":["1-1","1-2","1-3","1-4"],"I":["2I","3I","4I","5I"]}
    tempearture = {} #各クラスの最新の温度を保存する
    check = {} #各クラスに空調がついているか確認する
    person_num = {}
    count = 0 #カウント用変数

    spreadsheet = SpreadSheet() #スプレッドシートライブラリをインスタンス化
    picture = PictureGet() #ピクチャーライブラリをインスタンス化
    personNum = PersonNum() #人数カウントAIをインスタンス化
    tempearture_standard = Temperature() #気温ライブラリをインスタンス化
    point = calc_point(classes_group) #ポイントを計算するライブラリを取得
    light = Light()

    #最新の温度を取得
    tempearture = spreadsheet.get_latest_data_view("プロコンテストデータ","シート9")
    #最新画像の名前を取得
    #後々、全クラスの画像を取得できるようにする
    picture_name = spreadsheet.get_latest_data_front("Cam_受信_センター",4,"データ")

    #画像をダウンロード
    picture.download(picture_name)

    picture_path = f"picture//{picture_name}"
    #画像の人数をAIで測定する
    person_num["1-1"] = personNum.predict_person_count(picture_path)
    #ほんとはここで全クラスの写真をAIで判定して保存する 以下サンプルデータ
    person_num["1-2"] = 1
    person_num["1-3"] = 0
    person_num["1-4"] = 0
    person_num["2I"] = 0
    person_num["3I"] = 0
    person_num["4I"] = 12
    person_num["5I"] = 0
    

    #徳島の気温を取得
    temp = tempearture_standard.get_temperature()

    #空調がついているか確認する
    for i in tempearture.values():            
        differenceTemp = float(temp) - float(i)
        if abs(differenceTemp) > 3:
            check[classes[count]] = 1
        else:   
            check[classes[count]] = 0
        count = count + 1


    #照明の点灯の有無を確認
    light_c = {}
    light_c['1-1'] = light.is_light_on(picture_path)
    light_c['1-2'] = light.is_light_on(picture_path)
    light_c['1-3'] = light.is_light_on(picture_path)
    light_c['1-4'] = light.is_light_on(picture_path)
    light_c['2I'] = light.is_light_on(picture_path)
    light_c['3I'] = light.is_light_on(picture_path)
    light_c['4I'] = light.is_light_on(picture_path)
    light_c['5I'] = light.is_light_on(picture_path)

    #ポイントの計算
    point_list = point.calc(person_num,check,light_c,temp,tempearture)

    response = {'tempearture':tempearture,'person_number':person_num,'temp_standard':temp,'checkTemp':check,'point':point_list,'light':light_c}

    return response
    #response = {'result': text}
    #return make_response(jsonify(response))

#1週間の気温を取得
@app.route('/weekTemp',methods=['GET'])
def weekTemp():

    #データを保存する辞書型の変数
    week_data = {}
    spreadSheet = SpreadSheet() #スプレッドシートライブラリをインスタンス化

    result = spreadSheet.get_week_data_view("プロコンテストデータ","シート9")

    return result


@app.route('/weekPoint',methods=['GET'])
def weekPoint():

    #データを保存する辞書型の変数
    week_data = {}
    spreadSheet = SpreadSheet() #スプレッドシートライブラリをインスタンス化

    result = spreadSheet.get_week_data_view("プロコンテストデータ","point")

    return result

#flaskの基本設定
if __name__ == '__main__':
    app.run(debug=False, port=5050, threaded=True)