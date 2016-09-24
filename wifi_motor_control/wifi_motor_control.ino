#include "ESP8266.h"
#include <SoftwareSerial.h>
#include <Servo.h>

// Wi-Fi SSID
#define SSID        "iutewareware2014"
// Wi-Fi PASSWORD
#define PASSWORD    "yokomasataro"
// サーバーのホスト名
#define HOST_NAME   "192.168.9.111"
// ポート番号
#define HOST_PORT   80
 

#define STATE_STOP              0
#define STATE_FORWARD           1
#define STATE_BACKWARD          2
#define STATE_LEFT_FORWARD      3
#define STATE_RIGHT_FORWARD     4
#define STATE_LEFT_BACKWARD     5
#define STATE_RIGHT_BACKWARD    6

#define WITHOUT_LIBRARY

SoftwareSerial mySerial(12, 11); /* RX:12, TX:11 */
ESP8266 wifi(mySerial);
#if !defined(WITHOUT_LIBRARY)
Servo myservo;
#endif

const int pin_motor1 = 7;
const int pin_motor2 = 8;
const int pin_motor_pwm = 9;
const int pin_servo_signal = 6;
const int servo_calibration_offset = -5;
const int servo_direction_left = 75;
const int servo_direction_right = 105;
const int servo_direction_straight = 90;
int current_state = STATE_STOP;

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
  analogWrite(pin_motor_pwm,100); 
}

void motor_backward(void) 
{
  Serial.print("backward\n");
  digitalWrite(pin_motor1,LOW);
  digitalWrite(pin_motor2,HIGH);
  analogWrite(pin_motor_pwm,100); 
}

void setup_servo_motor(void) 
{
  Serial.print("setup Servo motor begin\n");
#if defined(WITHOUT_LIBRARY)
  pinMode(pin_servo_signal,OUTPUT);
#else
  myservo.attach(pin_servo_signal);
#endif
  control_servo(servo_direction_straight);
}

void control_servo(int degree) 
{
#if defined(WITHOUT_LIBRARY)
  int microsec,i;
  int control_degree = degree + servo_calibration_offset;
  microsec = (5*control_degree)+1000;

  Serial.print("Servo motor control : ");
  Serial.print(control_degree);
  Serial.print("\n");

  for(i=0; i<20 ;i++){
    digitalWrite(pin_servo_signal, HIGH);
    delayMicroseconds(microsec); 
    digitalWrite(pin_servo_signal, LOW);
    delayMicroseconds(10000 - microsec); 
  }
#else
  int control_degree = degree + servo_calibration_offset;
  if (control_degree>70 && control_degree<110 ){
    Serial.print("Servo motor control : ");
    Serial.print(control_degree);
    Serial.print("\n");
    myservo.write(control_degree);
    delay(500);
  }
#endif
}

int convert_to_state (String data)
{
  int state = STATE_STOP;

  if (data == "stop") {
    state = STATE_STOP;
  }
  else if (data == "forward") {
    state = STATE_FORWARD;
  }
  else if (data == "backward") {
    state = STATE_BACKWARD;
  }
  else if (data == "left-forward") {
    state = STATE_LEFT_FORWARD;
  }
  else if (data == "right-forward") {
    state = STATE_RIGHT_FORWARD;
  }
  else if (data == "left-backward") {
    state = STATE_LEFT_BACKWARD;
  }
  else if (data == "right-backward") {
    state = STATE_RIGHT_BACKWARD;
  }
  else {
    state = STATE_STOP;
  }

  return state;
}

void control_motors(int state) 
{
  if (current_state == state) {
    return;
  }

  current_state = state;

  switch(state) {
  case STATE_STOP:
    motor_stop();
    control_servo(servo_direction_straight);
    break;
  case STATE_FORWARD:
    control_servo(servo_direction_straight);
    motor_forward();
    break;
  case STATE_BACKWARD:
    control_servo(servo_direction_straight);
    motor_backward();
    break;
  case STATE_LEFT_FORWARD:
    control_servo(servo_direction_left);
    motor_forward();
    break;
  case STATE_RIGHT_FORWARD:
    control_servo(servo_direction_right);
    motor_forward();
    break;
  case STATE_LEFT_BACKWARD:
    control_servo(servo_direction_right);
    motor_backward();
    break;
  case STATE_RIGHT_BACKWARD:
    control_servo(servo_direction_left);
    motor_backward();
    break;
  default:
    break;
  }
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
  setup_servo_motor(); 
  setup_wifi();

  control_motors(STATE_STOP);
}

int test_state = 0;
void loop(void)
{
  uint8_t buffer[512] = {0};

  if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
     Serial.print("create tcp ok\r\n");
  } else {
    Serial.print("create tcp err\r\n");
    control_motors(STATE_STOP);
  }

  char *sendStr = "GET /~taro/motor_control/ledGet.php HTTP/1.0\r\nHost: 192.168.9.111\r\nUser-Agent: arduino\r\n\r\n";

  wifi.send((const uint8_t*)sendStr, strlen(sendStr));

  String resultCode = "";
  uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);

  if (len > 0) {
    for(uint32_t i = 0; i < len; i++) {
      resultCode += (char)buffer[i];
    }

    int lastLF = resultCode.lastIndexOf('\n');
    int resultCodeLength = resultCode.length();
    String resultString = resultCode.substring(lastLF+1, resultCodeLength);

    resultString.trim();

    Serial.println(resultString);
  
    control_motors(convert_to_state(resultString));
  }

  /*
    control_motors(test_state);
    test_state++;
    Serial.print("test_state : ");
    Serial.print(test_state);
    Serial.print("\n");
    if (test_state > STATE_RIGHT_BACKWARD) {
      test_state = 0;
    }
  */
  delay(200);
}
