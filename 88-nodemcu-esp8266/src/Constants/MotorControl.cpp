#include "Motor.cpp"

/*
   Board pin | NodeMCU GPIO |  Arduino IDE
      A-           1             5 or D1
      A+           3             0 or D3
      B-           2             4 or D2
      B+           4             2 or D4
*/

const int pwmMotorPinA = 5;
const int pwmMotorPinB = 4;
const int dirMotorPinA = 2;
const int dirMotorPinB = 0;

// Create motor objects
Motor leftMotor(pwmMotorPinA, dirMotorPinA);  // Create left motor object
Motor rightMotor(pwmMotorPinB, dirMotorPinB); // Create right motor object

void leftForward(int speed)
{
    leftMotor.setDirection(LOW);
    leftMotor.setSpeed(speed);
}

void leftBackward(int speed)
{
    leftMotor.setDirection(HIGH);
    leftMotor.setSpeed(speed);
}

void rightForward(int speed)
{
    rightMotor.setDirection(LOW);
    rightMotor.setSpeed(speed);
}

void rightBackward(int speed)
{
    rightMotor.setDirection(HIGH);
    rightMotor.setSpeed(speed);
}