#include <Arduino.h>
#include "Motor.h"

// Constructor implementation
Motor::Motor(int pwm, int dir) : pwm(pwm), dir(dir)
{
    pinMode(pwm, OUTPUT);
    pinMode(dir, OUTPUT);
}

// Set speed implementation
void Motor::setSpeed(int speed)
{
    if (speed > 1023)
    {
        speed = 1023;
    }
    else if (speed > 0 && speed < 700)
    {
        speed = 700;
    }
    analogWrite(pwm, speed);
}

// Set direction implementation
void Motor::setDirection(int direction)
{
    digitalWrite(dir, direction);
}
