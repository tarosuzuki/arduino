#include "ESP8266.h"
#define SSID        "iutewareware2014"
#define PASSWORD    "yokomasataro"

SoftwareSerial mySerial(12, 11); /* RX:12, TX:11 */

/*
struct DriverData {  
  DIRECTION_DATA direction;
  SPEED_DATA speed;
};

enum DIRECTION_DATA {
  DIRECTION_STOP,
  DIRECTION_FORWARD,
  DIRECTION_BACK
};

enum SPEED_DATA {
  SPEED_NORMAL,
  SPEED_SLOW,
  SPEED_FAST
};
*/

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

void setup(void)
{
  Serial.begin(9600);
  setup_dc_motor();
}
 
void loop(void)
{
  Serial.print("stop Motor\n");
  digitalWrite(pin_motor1,LOW);
  digitalWrite(pin_motor2,LOW);
  analogWrite(pin_motor_pwm,0); 
  delay(1000);
  Serial.print("forward\n");
  digitalWrite(pin_motor1,HIGH);
  digitalWrite(pin_motor2,LOW);
  analogWrite(pin_motor_pwm,120); 
  delay(3000);
  Serial.print("backward\n");
  digitalWrite(pin_motor1,LOW);
  digitalWrite(pin_motor2,HIGH);
  analogWrite(pin_motor_pwm,120); 
  delay(3000);
}
        
