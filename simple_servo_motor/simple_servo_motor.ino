#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int serial_read_counter = 0;
int need_to_flush_buf = 0;

void setup() {
  Serial.begin(9600);
  myservo.attach(6);
  myservo.write(90);
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
    
    if (degree>70 && degree<110 ){
      myservo.write(degree);
      delay(15);                           // waits for the servo to get there      
    }
  }
}

