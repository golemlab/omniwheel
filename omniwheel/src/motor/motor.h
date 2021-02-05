#include "Arduino.h"

class Motor {
  public:
    Motor(int, int, int, int, int, int);
    void move();
    void set_speed(float);
    void set_encoder();
    int pin_encoder() {return this->pinEncoder1;}
  private:
    float speed;
    int pinDir1;
    int pinDir2;
    int pinSpeed;
    int speedMin;
    int encoder;
    int pinEncoder1;
    int pinEncoder2;
};