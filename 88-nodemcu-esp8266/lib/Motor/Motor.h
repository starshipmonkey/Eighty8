#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

// This class is used the create a motor object. It can then be used to control the motor.
class Motor
{
public:
    // Constructor to initialize the motor object
    Motor(int pwm, int dir);
    
    // Set the speed of the motor (0-1023)
    void setSpeed(int speed);
    
    // Set the direction of the motor (HIGH/LOW)
    void setDirection(int direction);

private:
    int pwm;  // PWM pin
    int dir;  // Direction pin
};

#endif // MOTOR_H

