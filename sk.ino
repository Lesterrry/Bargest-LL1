/*
COPYRIGHT FETCH DEVELOPMENT
ALL RIGHTS RESERVED

2020
*/

#include <Servo.h>
#include <EEPROM.h>

Servo servo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

static int pos;    // variable to store the servo position
String data = "";
static bool locked;

void setup() {
  servo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
  locked = EEPROM.read(0) == 1 ? true : false;
  pos = locked ? 180 : 0;
  servo.write(pos);
}

void lock(){
  for (pos; pos < 180; pos += 1) {
    servo.write(pos);
    delay(15);
  }
  EEPROM.write(0,1);
  locked = true;
}

void unlock(){
  for (pos; pos > 0; pos -= 1) {
    servo.write(pos);
    delay(15);
  }
  EEPROM.write(0,0);
  locked = false;
}

void loop(){
  if (Serial.available() > 0){
    data = Serial.readString();
    if(data == "lock"){
        lock();
    }
    else if(data == "unlock"){
        unlock();
    }
    else if(data == "dump"){
        Serial.println(locked);
        Serial.println(pos);
    }
    delay(100);
  }
}
