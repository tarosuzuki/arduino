#include "ESP8266.h"
#include <SoftwareSerial.h>

// Wi-Fi SSID
#define SSID        "iutewareware2014"
// Wi-Fi PASSWORD
#define PASSWORD    "yokomasataro"
// サーバーのホスト名
#define HOST_NAME   "192.168.9.111"
// ポート番号
#define HOST_PORT   80
 
SoftwareSerial mySerial(12, 11); /* RX:12, TX:11 */
ESP8266 wifi(mySerial);

const int pin_motor1 = 7;
const int pin_motor2 = 8;
const int pin_motor_pwm = 9;
void setup_dc_motor(void) 
{
  Serial.print("setup DC motor begin\n");
  pinMode(pin_motor1,OUTPUT);
  pinMode(pin_motor2,OUTPUT);
}

void motor_stop(void) 
{
  Serial.print("stop Motor\n");
  digitalWrite(pin_motor1,LOW);
  digitalWrite(pin_motor2,LOW);
  analogWrite(pin_motor_pwm,0); 
}

void motor_forward(void) 
{
  Serial.print("forward\n");
  digitalWrite(pin_motor1,HIGH);
  digitalWrite(pin_motor2,LOW);
  analogWrite(pin_motor_pwm,120); 
}

void motor_backward(void) 
{
  Serial.print("backward\n");
  digitalWrite(pin_motor1,LOW);
  digitalWrite(pin_motor2,HIGH);
  analogWrite(pin_motor_pwm,120); 
}

void setup_wifi(void) 
{
  Serial.print("FW Version:");
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {
    Serial.print("to station + softap ok\r\n");
  } else {
    Serial.print("to station + softap err\r\n");
  }

  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print("Join AP success\r\n");
    Serial.print("IP:");
    Serial.println( wifi.getLocalIP().c_str());
  } else {
    Serial.print("Join AP failure\r\n");
  }

  if (wifi.disableMUX()) {
    Serial.print("single ok\r\n");
  } else {
    Serial.print("single err\r\n");
  }
 
  Serial.print("setup end\r\n");
}

void setup(void)
{  
  Serial.begin(9600);
  Serial.print("setup begin\r\n");
  setup_dc_motor();
  setup_wifi();

  motor_stop(); 
}

/**
 * ループ処理
 */
void loop(void)
{
  uint8_t buffer[512] = {0};

  // TCPで接続
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
     Serial.print("create tcp ok\r\n");
  } else {
    Serial.print("create tcp err\r\n");
    motor_stop(); 
  }

  // サーバーへ渡す情報
  // 例 http://test.co.jp/arduino-esp/ledGet.php
  char *sendStr = "GET /~taro/motor_control/ledGet.php HTTP/1.0\r\nHost: 192.168.9.111\r\nUser-Agent: arduino\r\n\r\n";

  wifi.send((const uint8_t*)sendStr, strlen(sendStr));

  String resultCode = "";
  uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);

  if (len > 0) {
    for(uint32_t i = 0; i < len; i++) {
      resultCode += (char)buffer[i];
    }

    // lastIndexOfでresultCodeの最後から改行を探す
    int lastLF = resultCode.lastIndexOf('\n');

    // resultCodeの長さを求める
    int resultCodeLength = resultCode.length();

    // substringで改行コードの次の文字から最後までを求める
    String resultString = resultCode.substring(lastLF+1, resultCodeLength);

    // 前後のスペースを取り除く
    resultString.trim();

    // 取得した文字列がONならば
    Serial.println(resultString);
    
    if(resultString == "forward") {
      motor_forward(); 
    } else if(resultString == "backward") {
      motor_backward(); 
    } else {
      motor_stop(); 
    }
  }

  // 200ミリ秒待つ
  delay(200);
}
