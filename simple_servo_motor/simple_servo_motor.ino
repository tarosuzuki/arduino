#include <Servo.h>

#define WITHOUT_LIBRARY

#if !defined(WITHOUT_LIBRARY)
Servo myservo;  // create servo object to control a servo
#endif

int serial_read_counter = 0;
int need_to_flush_buf = 0;
const int pin_servo = 6;

#if defined(WITHOUT_LIBRARY)
void servo_control2(int angle){
  int microsec,i;
  microsec = (5*angle)+1000;
       
  for(i=0; i<20 ;i++){
    digitalWrite(pin_servo, HIGH );
    delayMicroseconds(microsec); 
    digitalWrite(pin_servo, LOW );
    delayMicroseconds(10000 - microsec); 
  }
}
#endif

void setup() {
  Serial.begin(9600);
#if defined(WITHOUT_LIBRARY)
  pinMode(pin_servo,OUTPUT);
#else
  myservo.attach(6);
  myservo.write(90);
#endif
  Serial.print("simple servo controller start.\nplease input target degree.\n");
}

#define BUF_READ_MAX  32 
void loop() {

  int i,degree;
  int recieve_length = 0;
  char buf[32];

  recieve_length = Serial.available(); 
  if(recieve_length > 0) {
    //Serial.print("recieve length : ");
    //Serial.print(recieve_length);
    //Serial.print("\n");
    for(i=0; i<recieve_length; i++) {
      if(serial_read_counter >= BUF_READ_MAX) {
        Serial.print("size over for recieve buffer (over 32) !!!\n");
        need_to_flush_buf = 1;
        break;
      }

      buf[serial_read_counter] = Serial.read();
      //Serial.print(buf[serial_read_counter], HEX);
      //Serial.print("\n");
      if(buf[serial_read_counter] == '\0' ||
         buf[serial_read_counter] == '\n' ||
         buf[serial_read_counter] == '\r') {
          buf[serial_read_counter+1] = '\0';
          need_to_flush_buf = 1;
          break;
      }
      serial_read_counter++;
    }
  }

  if (need_to_flush_buf) {
    serial_read_counter = 0;
    need_to_flush_buf = 0;

    degree = atoi(buf);
    Serial.print("degree :");
    Serial.print(buf);
    Serial.print("\n");
    
    if (degree>50 && degree<130 ){
#if defined(WITHOUT_LIBRARY)
      servo_control2(degree);
#else
      myservo.write(degree);
      delay(15);                           // waits for the servo to get there      
#endif
    }
  }
}

