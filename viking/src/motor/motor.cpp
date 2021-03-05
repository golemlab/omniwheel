#include "motor.h"

Motor::Motor(int pinDir1, int pinDir2, int pinSpeed, int pinEncoder1, int pinEncoder2, int speedMin) {

this->pinDir1 = pinDir1;
this->pinDir2 = pinDir2;
this->pinSpeed = pinSpeed;
this->speedMin = speedMin;
this->pinEncoder1 = pinEncoder1;
this->pinEncoder2 = pinEncoder2;
this->encoder = 0;
}

void Motor::set_speed(float speed) {
  int dir1=LOW, dir2=LOW, _spd;
  if (speed < 0) dir2=HIGH;
  if (speed > 0) dir1=HIGH;
  analogWrite(pinSpeed, map(abs(speed), 0.0, 100.0, speedMin, 1024));
  digitalWrite(pinDir1, dir1);
  digitalWrite(pinDir2, dir2);
}

void Motor::set_encoder() {
  int n = digitalRead(this->pinEncoder2);
  if (n == 0) n = -1;
  encoder = encoder + n;
}