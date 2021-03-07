#include <Arduino.h>
#include <Wire.h>
#include "motor/motor.h"

float speed_x=0, speed_y=0, speed_w=0;

const int pinAIN1=22, pinAIN2=23, pinASpeed=4, pinAEncoder1=2, pinAEncoder2=24;
Motor motorA(pinAIN1, pinAIN2, pinASpeed, pinAEncoder1, pinAEncoder2, 0);

const int pinBIN1=26, pinBIN2=27, pinBSpeed=5, pinBEncoder1=3, pinBEncoder2=25;
Motor motorB(pinBIN1, pinBIN2, pinBSpeed, pinBEncoder1, pinBEncoder2, 0);

const int pinCIN1=30, pinCIN2=31, pinCSpeed=6, pinCEncoder1=18, pinCEncoder2=29;
Motor motorC(pinCIN1, pinCIN2, pinCSpeed, pinCEncoder1, pinCEncoder2, 0);

void parse_string(String inputString, float* speed_X, float* speed_Y, float* speed_W) {
  unsigned int message_substring = 0;
  String substr = "";
  for (unsigned int i = 0 ; i < inputString.length(); i++) {
    if (inputString[i] == ',' || i == inputString.length()-1) {
      message_substring++;
      switch (message_substring) {
        case 1:
          *speed_X = substr.toFloat();
          substr = "";
        case 2:
          *speed_Y = substr.toFloat();
          substr = "";
        case 3:
          *speed_W = substr.toFloat();
          substr = "";
      }
    }
    else substr += (char)inputString[i];
  }
}

void requestEvent(){Wire.write(1);}
void receiveEvent(int howMany)
{
  String input = "";
  while(1 < Wire.available())
  {
    input += Wire.read();
  }
   parse_string(input, &speed_x, &speed_y, &speed_w);
}

void incrementA(){motorA.set_encoder();}
void incrementB(){motorB.set_encoder();}
void incrementC(){motorC.set_encoder();}


void move(float speed_x,float speed_y,float speed_w) {
  float speed_A = (-speed_x / sin(PI / 3)) + (-speed_y * cos(PI / 3)) - speed_w;
  float speed_B = speed_y - speed_w;
  float speed_C = (speed_x / sin(PI / 3)) + (-speed_y * cos(PI / 3)) - speed_w;
  float norm = 1.0;
  if (abs(speed_A) > 1 || abs(speed_B) > 1 || abs(speed_C) > 1) {
    if (abs(speed_A) >= abs(speed_B)) {
      norm = abs(speed_A);
      if (abs(speed_C) > abs(speed_A)) norm = abs(speed_C);

    }
    else {
      norm = abs(speed_B);
      if (abs(speed_C) > abs(speed_B)) norm = abs(speed_C);
    }
  }
  motorA.set_speed((speed_A / norm) * 100.0);
  motorB.set_speed((speed_B / norm) * 100.0);
  motorC.set_speed((speed_C / norm) * 100.0);
}


void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  attachInterrupt(motorA.pin_encoder(), incrementA, RISING); //2, 3, 18, 19
  attachInterrupt(motorB.pin_encoder(), incrementB, RISING); //2, 3, 18, 19
  attachInterrupt(motorC.pin_encoder(), incrementC, RISING); //2, 3, 18, 19
  Wire.begin(2); // 20, 21
  //PWM: 4 - 13, 44 - 46
  pinMode(pinAIN1, OUTPUT);
	pinMode(pinAIN2, OUTPUT);
	pinMode(pinASpeed, OUTPUT);
  pinMode(pinBIN1, OUTPUT);
	pinMode(pinBIN2, OUTPUT);
	pinMode(pinBSpeed, OUTPUT);
  pinMode(pinCIN1, OUTPUT);
	pinMode(pinCIN2, OUTPUT);
	pinMode(pinCSpeed, OUTPUT);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop(){
  move(speed_x, speed_y, speed_w);
  Serial.print(speed_x);
  Serial.print("  ");
  Serial.print(speed_y);
  Serial.print("  ");
  Serial.println(speed_w);
  delay(200);
}