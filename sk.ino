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
  //analogWrite(setterA, 0);
  //analogWrite(setterB, 0);
  
  Serial.begin(9600);
  //locked = EEPROM.read(0) == 1 ? true : false;
}

void turn(Direction dir){
  switch (dir){
    case right:
      digitalWrite(motorA, LOW);
      digitalWrite(motorB, HIGH);
      break;
    case left:
      digitalWrite(motorB, LOW);
      digitalWrite(motorA, HIGH);
      break;
  }
}

void freeze(){
  digitalWrite(motorB, HIGH);
  digitalWrite(motorA, HIGH);
}

void lock(){

  EEPROM.write(0,1);
  locked = true;
}

void unlock(){

  EEPROM.write(0,0);
  locked = false;
}

void loop(){
  if(analogRead(sensorB) == 0){
    delay(50);
    if(analogRead(sensorB) == 0){
      Serial.println("100");
    }
    else{
      Serial.println("0");
    }
  }else{
    Serial.println("0");
  }
  //Serial.println(analogRead(sensorB));
  //Serial.println(digitalRead(sensorB));
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
    else if(data == "5"){
        turn(left);
        delay(500);
        freeze();
    }
    else if(data == "6"){
        turn(right);
        delay(500);
        freeze();
    }
    
    delay(100);
  }
}
