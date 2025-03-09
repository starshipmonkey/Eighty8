#include "../include/config/MotorControl.h"

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
    leftMotor.setDirection(HIGH);
    leftMotor.setSpeed(speed);
}

void leftBackward(int speed)
{
    leftMotor.setDirection(LOW);
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

// Motor setup function
void motorSetup() {
    // Initialize pins if needed
    pinMode(dirMotorPinA, OUTPUT);
    pinMode(dirMotorPinB, OUTPUT);
    // PWM pins are automatically set up by the Motor class
}

// Basic motor control functions
void moveForward(int speed) {
    leftForward(speed);
    rightForward(speed);
}

void moveBackward(int speed) {
    leftBackward(speed);
    rightBackward(speed);
}

void turnLeft(int speed) {
    leftBackward(speed / 2);  // Slow down or stop the left motor
    rightForward(speed);      // Keep the right motor moving forward
}

void turnRight(int speed) {
    leftForward(speed);       // Keep the left motor moving forward
    rightBackward(speed / 2); // Slow down or stop the right motor
}

void stopMotors() {
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(0);
}

// More advanced movement functions
void rotateInPlace(int speed, bool clockwise) {
    if (clockwise) {
        leftForward(speed);
        rightBackward(speed);
    } else {
        leftBackward(speed);
        rightForward(speed);
    }
}

void moveWithPrecision(int leftSpeed, int rightSpeed) {
    // Set direct speeds for each motor
    if (leftSpeed >= 0) {
        leftForward(leftSpeed);
    } else {
        leftBackward(-leftSpeed);  // Convert negative speed to positive for backward
    }
    
    if (rightSpeed >= 0) {
        rightForward(rightSpeed);
    } else {
        rightBackward(-rightSpeed); // Convert negative speed to positive for backward
    }
}

void gradualStop(int deceleration) {
    int leftSpeed = getLeftMotorSpeed();
    int rightSpeed = getRightMotorSpeed();
    
    // Gradually reduce speed until both motors stop
    while (leftSpeed > 0 || rightSpeed > 0) {
        // Reduce speeds
        leftSpeed = max(0, leftSpeed - deceleration);
        rightSpeed = max(0, rightSpeed - deceleration);
        
        // Apply new speeds
        leftMotor.setSpeed(leftSpeed);
        rightMotor.setSpeed(rightSpeed);
        
        // Short delay for gradual deceleration
        delay(50);
    }
    
    // Ensure motors are completely stopped
    stopMotors();
}

// Status functions
bool isMoving() {
    return (getLeftMotorSpeed() > 0 || getRightMotorSpeed() > 0);
}

int getLeftMotorSpeed() {
    // We don't have a direct way to get the current speed from the Motor class
    // In a real implementation, we would store the speed in a variable or read from the motor
    // For this example, we'll return 0 as a placeholder
    return 0; // Placeholder - would need proper implementation
}

int getRightMotorSpeed() {
    // Same as getLeftMotorSpeed, we need a proper implementation
    return 0; // Placeholder - would need proper implementation
}
