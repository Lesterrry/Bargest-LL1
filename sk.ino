/*
COPYRIGHT FETCH DEVELOPMENT
ALL RIGHTS RESERVED

2020
*/
#define motorA  10
#define motorB  9
#define sensorA A1
#define sensorB A3
#define setterA A2
#define setterB A4

#include <EEPROM.h>

static String data = "";
static bool locked;

enum Direction{
  right,
  left
};

void setup() {
  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);
  pinMode(sensorA, INPUT);
  pinMode(sensorB, INPUT);
  pinMode(setterA, OUTPUT);
  pinMode(setterB, OUTPUT);

  digitalWrite(motorA, HIGH);
  digitalWrite(motorB, HIGH);
  analogWrite(setterA, 0);
  analogWrite(setterB, 0);
  
  Serial.begin(9600);
  //locked = EEPROM.read(0) == 1 ? true : false;
}

void turn(Direction dir){
  switch (dir){
    case right:
      digitalWrite(motorB, LOW);
      digitalWrite(motorA, HIGH);
      break;
    case left:
      digitalWrite(motorA, LOW);
      digitalWrite(motorB, HIGH);
      break;
  }
}

void freeze(){
  digitalWrite(motorB, HIGH);
  digitalWrite(motorA, HIGH);
}

void lock(){
  turn(right);
  uint32_t stamp = millis();
  while(millis() < stamp + 7000){
    if(analogRead(sensorB) == 0){
      delay(50);
      if(analogRead(sensorB) == 0){
        break;
      }
    }
  }
  freeze();

  //EEPROM.write(0,1);
  //locked = true;
}

void unlock(){
  int x = 0;
  uint32_t stamp = millis();
  turn(left);
  while(millis() < stamp + 6000){
    delay(30);
    x = analogRead(sensorA);
    if(x > 100){
      break;
    }
  }
  freeze();
  
  //EEPROM.write(0,0);
  //locked = false;
}

void loop(){

  if (Serial.available() > 0){
    data = Serial.readString();
    if(data == "lock" || data == "1"){
        lock();
    }
    else if(data == "unlock" || data == "0"){
        unlock();
    }
    else if(data == "dump" || data == "2"){
        Serial.println(locked);
    }
    
    delay(100);
  }
}
