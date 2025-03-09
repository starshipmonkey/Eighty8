#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <Arduino.h>
#include "../../lib/Motor/Motor.h"

// Motor setup function - call this from setup()
void motorSetup();

// Basic motor control functions
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft(int speed);
void turnRight(int speed);
void stopMotors();

// More advanced movement functions
void rotateInPlace(int speed, bool clockwise);
void moveWithPrecision(int leftSpeed, int rightSpeed);
void gradualStop(int deceleration);

// Status functions
bool isMoving();
int getLeftMotorSpeed();
int getRightMotorSpeed();

#endif // MOTORCONTROL_H

