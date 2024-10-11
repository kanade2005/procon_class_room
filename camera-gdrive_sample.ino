/*esp32cam-gdrive  Ver4.02   LED_Light対応*/
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "soc/rtc_cntl_reg.h"
#include "Base64.h"
#include "esp_camera.h"
#include <Wire.h>

//#define MICON_ESP32
#define PIN_SDA  4               // GPIO4
#define PIN_SCL 13               // GPIO13
#define ADDRESS_SUB 0x1E     // Check using ../I2C_Scanner/I2C_Scanner.ino

// DeepSleep
#define SLEEP_INTERVAL_SEC 3600   //60分間隔
uint64_t starttime;

//const char* ssid = "ananpro01";
//const char* password = "anan-nctacjp";
const char* myDomain = "script.google.com";
String myScript = "/macros/s/AKfycbwp5IXyvSqVU4UGy3LOV5-4h6UVXOrLlbGb2_HPbsjdiAL83FU/exec";    //Replace with your own url
String myFilename = "filename=ESP32-CAM.jpg";
String mySsid = "&ssid=" + String(ssid);
String myRadiation = "&radiation=1234";
String mimeType = "&mimetype=image/jpeg";
String myImage = "&data=";

int waitingTime = 30000; //Wait 30 seconds to google response.
int jpg_dot = 0;
byte SleepT = 1;    //10分単位 1 -> 10分 スプレッドシートの戻り値が不正な時1を返す
byte wdt = 0;    //10分単位から8.1秒(wdt)経過数  
int Radiation = 2;   //Arduino側未接続時にAECの設定を0とするため1より大きい初期値
float Eneloop = 4.88;
float temperature = -40;
boolean shootF =1;             //カメラ撮影フラグ 0で撮影無し

#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    15
#define XCLK_GPIO_NUM     27
#define SIOC_GPIO_NUM     23

#define Y9_GPIO_NUM       19
#define Y8_GPIO_NUM       36
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       39
#define Y5_GPIO_NUM        5
#define Y4_GPIO_NUM       34
#define Y3_GPIO_NUM       35
#define Y2_GPIO_NUM       32
#define HREF_GPIO_NUM     26
#define PCLK_GPIO_NUM     21

//defined(M5CAM_MODEL_B)
#define SIOD_GPIO_NUM     22
#define VSYNC_GPIO_NUM    25
void deep_sleep(const uint64_t interval_sec)
{
  uint64_t sleeptime = interval_sec * 1000000 - (millis() - starttime) * 1000;
  esp_deep_sleep(sleeptime);  // DeepSleepモードに移行
}

void setup()
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.begin(115200);
  delay(10);

  Wire.begin( PIN_SDA, PIN_SCL );
  reader();

  WiFi.mode(WIFI_STA);
  byte wnum;
  for (int retry = 0; retry < 6 ; retry++) { //6回リトライ
    Serial.println("");
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    wnum = 30;   //この時間で待ち時間を調整 30->15秒
    while ( WiFi.status() != WL_CONNECTED && wnum > 0) {
      Serial.print(".");
      delay(500);
      wnum--;
    }
    if (wnum != 0) break;
  }
  if ( wnum == 0) {
//    Serial.println("disconnect WiFi  start Deep_sleep ");
    writer(0,1);    //WiFiタイムアウトなら10分後にリトライのためArduinoに送信
    deep_sleep(SLEEP_INTERVAL_SEC);
  }

  Serial.println("");
  Serial.println("STAIP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("");

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 10000000;
  config.pixel_format = PIXFORMAT_JPEG; //YUV422,GRAYSCALE,RGB565,JPEG
  config.frame_size = FRAMESIZE_UXGA;  // Timer Camera X の最高解像度はQXGA(2048x1536)。https://github.com/espressif/esp32-camera/blob/master/driver/include/sensor.h
  config.jpeg_quality = 5;    //0-63 lower number means higher quality    (試したところ、UXGAの最高画質は2。QXGAの最高画質は3だった。写った画像の複雑さによるかもしれないが。)
  config.fb_count = 2;    //if more than one, i2s runs in continuous mode. Use only with JPEG


  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }

    //イメージセンサOV2640の初期設定
  sensor_t *sensor = esp_camera_sensor_get();
  sensor->set_hmirror(sensor, 1); //M5Cameraのモデルによって変更
  sensor->set_vflip(sensor, 1); //M5Cameraのモデルによって変更
  sensor->set_whitebal(sensor, 1); //ホワイトバランスON
  sensor->set_awb_gain(sensor, 1); //自動ホワイトバランスゲインON
  sensor->set_exposure_ctrl(sensor, 1);  //自動露出制御のON
  sensor->set_aec2(sensor, 1); //DSP自動露出制御ON
  sensor->set_brightness(sensor, 1);//up the blightness just a bit
  sensor->set_saturation(sensor, -2);//lower the saturation
  sensor->set_wb_mode(sensor, 0);    //晴れ設定
  if( Radiation <= 1 ) {
//    sensor->set_ae_level(sensor, 1); //-2～2
//    sensor->set_brightness(sensor, 2);  //-2～2
    //sensor->set_wb_mode(sensor, 3); 
    sensor->set_brightness(sensor, -2); //-2～2
  }else if(Radiation > 30){          //日射がある時明るさを下げる
    sensor->set_ae_level(sensor, -2); //-2～2
    sensor->set_brightness(sensor, -2); //-2～2
  }
  
  myRadiation = "&radiation=" + String(Radiation) + "&volt=" + String(Eneloop) + "&temperature=" + String(temperature);
  starttime = millis();
  saveCapturedImage();
  if( wdt > 74 ) wdt = 0;
  if(SleepT == 0 || SleepT > 130 || (SleepT > 30 && SleepT <= 100)  ) SleepT = 1;
  writer(wdt,SleepT);

//  Serial.print( "SleepT= " );   Serial.print( SleepT ); 
//  delay(200);
  deep_sleep(SLEEP_INTERVAL_SEC);
}


void loop() {
}

void saveCapturedImage() {
  Serial.println("Connect to " + String(myDomain));
  WiFiClientSecure client;
  client.setInsecure();

  byte wnum = 10;   //この時間で待ち時間を調整 10->30秒
  while ( client.connect(myDomain, 443) != 1 && wnum > 0) {
      Serial.print(".");
      delay(3000);
      wnum--;
  }
  if (wnum == 0){
    Serial.println("disconnect myDomain deep_sleep & restart ");
    esp_deep_sleep(5000000);  // myDomain接続タイムアウトならdeep_sleepモードに移行し5秒後に再起動
  }else{
    Serial.println("Connection successful");

    camera_fb_t * fb = NULL;
//    esp_camera_fb_get();   // 写真を撮影してフレームバッファを得る。属性値：fb->width, fb->height, fb->format, fb->buf, fb->len
//    esp_camera_fb_return(fb);   // システムにフレームバッファを使い終わったことを知らせる。
//    delay(1000);
    fb = esp_camera_fb_get();   // 写真を撮影してフレームバッファを得る。属性値：fb->width, fb->height, fb->format, fb->buf, fb->len
    if (!fb) {
      Serial.println("Camera capture failed");    // 取得に失敗した。
      delay(1000);
      ESP.restart();
      return;
    }
    Serial.printf("frame buffer size: %u x %u\n", fb->width, fb->height);    

    Serial.println("Step 1: calicurating data size...");    // Base64とurlencodeされたデータのサイズを数える。
    int index = 0;
    uint8_t *p = fb->buf;
    int rest = fb->len;       // 元のサイズ
    int base64EncodedSize = 0;  // Base64 後のサイズ
    int urlencodedSize = 0;   // Base64 + urlencode 後のサイズ
    while (rest > 0)
    {
      char output[2048 +1];    // 一度に出力するBase64化されたデータを入れるバッファ (base64_encode()が末尾にヌルを入れるので、1バイト追加。)
      int srcLen = rest > 1536 ? 1536 : rest;   // このサイクルでエンコードする元データサイズ(最大はバッファの 3/4 のサイズ)
      int encLen = base64_encode(output, (char *)p + index, srcLen);   // Base64エンコードする。
      base64EncodedSize += encLen;
      if (encLen > 0) {
        String str = urlencode(String(output));   // URLエンコードする。
        urlencodedSize += str.length();
      }
      index += srcLen;
      rest -= srcLen;
    }
    Serial.printf("frame buffer size: %u\n", fb->len);
    Serial.printf("after Base64 encoding: %u\n", base64EncodedSize);
    Serial.printf("frame buffer size: %u\n", urlencodedSize);


    Serial.println("Step 2: Sending a captured image to Google Drive.");
    String Data = myFilename + mySsid + myRadiation + mimeType + myImage;
    for( int i = 0 ; i < 2 ; i++ ){      //retry loop

      Serial.println("Send to G_Drive.");
      Serial.println(mySsid);

      client.println("POST " + myScript + " HTTP/1.1");
      client.println("Host: " + String(myDomain));
      client.println("Content-Length: " + String(Data.length() + urlencodedSize));    // ここでデータの長さを書く必要があるので、Step 1 が必要。
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.println();

      client.print(Data);

      index = 0;
      p = fb->buf;
      rest = fb->len;
      Serial.printf("Estimated cycle: %u\n", rest / 1536);
      while (rest > 0 && client.connected())
      {
        char output[2048 +1];    // 一度に出力するBase64化されたデータを入れるバッファ (base64_encode()が末尾にヌルを入れるので、1バイト追加。)
        int srcLen = rest > 1536 ? 1536 : rest;   // このサイクルでエンコードする元データサイズ(最大はバッファの 3/4 のサイズ)
        int encLen = base64_encode(output, (char *)p + index, srcLen);    // Base64エンコードする。
        if (encLen > 0) {
          String str = urlencode(String(output));   // URLエンコードする。
          client.write((uint8_t *)(str.c_str()), str.length());   // データを送信する。
          index += srcLen;
          rest -= srcLen;
          Serial.print(".");
        }
      }
      Serial.println();
    
      Serial.println("Waiting for response.");
      long int StartTime = millis();
      while (!client.available()) {
        Serial.print(".");
        delay(100);
        if ((StartTime + waitingTime) < millis()) {
          Serial.println();
          Serial.println("No response.");
          //If you have no response, maybe need a greater value of waitingTime
          SleepT=1;    //google responseタイムアウトなら1分以内にリトライのためArduinoに70,1送信
          wdt=70;
          client.stop();
          return;      //タイムアウトならここでメソッドを終了。
        }
      }
      Serial.println();
      String getStr = "";
      while (client.available()) {
        char s = client.read();
//        Serial.print(s);
        getStr += s;
      }
      Serial.println();
      int no_x = getStr.indexOf("nextDataSend:");
      if( no_x > 0 ){                          //応答文字列中に"nextDataSend:"が見つかったら
        int no_x2 = getStr.indexOf(",",no_x);
        SleepT = (int16_t)((getStr.substring(no_x+13,no_x2)).toInt());   
        Serial.println(SleepT);
        int no_x3 = getStr.indexOf("wdt:");
        if( no_x3 > 0){                      //応答文字列中に"wdt:"が見つかったら
          int no_x4 = getStr.indexOf("}",no_x3);
          Serial.println(getStr.substring(no_x3+4,no_x4)); 
          Serial.println((getStr.substring(no_x3+4,no_x4)).toInt()); 
          wdt = (int16_t)((getStr.substring(no_x3+4,no_x4)).toInt()); 
          break;
        }else wdt = 0;
        break;
      }else{        //google レスポンス文字列から値が取れなかった場合
        SleepT = 1;    //10後にリトライのためArduinoに0,1送信
        wdt = 0;
        jpg_dot = 0;
        myFilename = "filename=Retry";
        Data = myFilename + mySsid + myRadiation + mimeType + myImage;
      }
    }
    client.flush();
    esp_camera_fb_return(fb);   // システムにフレームバッファを使い終わったことを知らせる。
  }      
  client.stop();
}

//https://github.com/zenmanenergy/ESP8266-Arduino-Examples/
String urlencode(String str)
{
  String encodedString = "";
  char c;
  char code0;
  char code1;
  char code2;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == ' ') {
      encodedString += '+';  jpg_dot++;
    } else if (isalnum(c)) {
      encodedString += c;  jpg_dot++;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) {
        code0 = c - 10 + 'A';
      }
      code2 = '\0';
      encodedString += '%';  jpg_dot++;
      encodedString += code0;  jpg_dot++;
      encodedString += code1;  jpg_dot++;
      //encodedString+=code2;
    }
    yield();
  }
  return encodedString;
}

void writer(byte x,byte y) {
    Serial.print("writer: ");
    Serial.println(x);
    Wire.beginTransmission(ADDRESS_SUB);    // transmit to device #1E
    Wire.write("OK,");            // sends 3 bytes
    Wire.write(x);                  // sends 1 byte
    Wire.write(y);                  // sends 1 byte
    Wire.endTransmission();         // stop transmitting
}

void reader(void) {
  Serial.print("reader: ");
    Wire.requestFrom(ADDRESS_SUB, 12);   // request 12 bytes from sub device #1E
    String data = Wire_receive();
        Serial.println("");
//        Serial.println(data.substring(0,4));
//        Serial.println(data.substring(4));      
        Radiation = HexAtoi(data.substring(2,4)+data.substring(0,2));
        Serial.println(Radiation);
        Eneloop =  HexAtoi(data.substring(6,8)+data.substring(4,6))/100.0;
        if( Radiation == 0 && Eneloop > 13 && Eneloop < 17 ) {   //カメラ撮影しないフラグ判定
          Eneloop = Eneloop -10.0;      //カメラ撮影しないフラグを除去
          shootF = 0;                   //カメラ撮影しないフラグ設定
        }else    shootF = 1;
        Serial.println(Eneloop);
        temperature =  HexAtoi(data.substring(10)+data.substring(8,10))/5.0-40.0;
        Serial.println(temperature);
}

String Wire_receive(){ //取得したデータ数
    char buf[64];
    int buf_len=0;
    String data = "";  

    buf_len =  Wire_recv(buf);     //受信成功なら2以上が戻される

    if ( buf_len >= 2 ){         //buf_lenが2以上ならdata処理を実行
      for(int i=0;i<buf_len;i++){  //1文字以下ならfor文は実行されない
        Serial.write(buf[i]);
        data +=  char(buf[i]);
      }
    }
    return data;
}

int Wire_recv(char *buf) {//データ取得
    char *start = buf;
    byte wnum =100;   //この時間で待ち時間を調整 100->10秒
    while (wnum != 0) {
      delay(100);
      wnum--;
      while( Wire.available() ) {     // sub may send less than requested
        *buf++ = Wire.read();       // receive a byte as character
      }
      *buf++ = '\0';    //\0の区切りを追加
      return (buf - start);      //受信文字列+\0で3以上が戻される
    }
    return 0;
}

int HexAtoi(String str) {
    char sNum[20];
    str.toCharArray(sNum,20);
    int ret = 0;
    for( int i = 0; i < 20 ; i++ ){
        if(sNum[i] == NULL) break;
        ret <<= 4;
        if ( sNum[i] >= 'A' && sNum[i] <= 'F' ) {
            ret += sNum[i] - 'A' + 10;
        } else if ( sNum[i] >= 'a' && sNum[i] <= 'f' ) {
            ret += sNum[i] - 'a' + 10;
        } else if ( sNum[i] >= '0' && sNum[i] <= '9' ) {
            ret += sNum[i] - '0';
        } else {
            return -1;
        }    
    }
    return ret;
}
