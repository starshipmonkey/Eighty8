#include <Arduino.h>

// This class is used the create a motor object. It can then be used to control the motor.

class Motor
{
public:
    Motor(int pwm, int dir) : pwm(pwm), dir(dir) // Constructor to initialize the motor object
    {
        pinMode(pwm, OUTPUT);
        pinMode(dir, OUTPUT);
    }

    void setSpeed(int speed)
    {
        if (speed > 1023)
        {
            speed = 1023;
        }
        analogWrite(pwm, speed);
    }

    void setDirection(int direction)
    {
        digitalWrite(dir, direction);
    };

private:
    int pwm;
    int dir;
};