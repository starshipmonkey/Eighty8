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
    // Calculate the turn ratio to ensure minimum motor speed
    const int MIN_MOTOR_SPEED = 700;
    int slowMotorSpeed;
    
    if (speed == 0) {
        slowMotorSpeed = 0; // If input speed is 0, slow motor should also be 0
    } else if (speed < MIN_MOTOR_SPEED) {
        slowMotorSpeed = MIN_MOTOR_SPEED; // If input speed is below minimum, use minimum
    } else {
        // Calculate the slow motor speed, ensuring it doesn't go below minimum
        // For input speed of 1000, slower motor should be at least 700
        float turnRatio = max(0.7f, (float)MIN_MOTOR_SPEED / speed);
        slowMotorSpeed = (int)(speed * turnRatio);
    }
    
    leftBackward(slowMotorSpeed);  // Set left motor to calculated speed
    rightForward(speed);           // Keep the right motor at full speed
}

void turnRight(int speed) {
    // Calculate the turn ratio to ensure minimum motor speed
    const int MIN_MOTOR_SPEED = 700;
    int slowMotorSpeed;
    
    if (speed == 0) {
        slowMotorSpeed = 0; // If input speed is 0, slow motor should also be 0
    } else if (speed < MIN_MOTOR_SPEED) {
        slowMotorSpeed = MIN_MOTOR_SPEED; // If input speed is below minimum, use minimum
    } else {
        // Calculate the slow motor speed, ensuring it doesn't go below minimum
        // For input speed of 1000, slower motor should be at least 700
        float turnRatio = max(0.7f, (float)MIN_MOTOR_SPEED / speed);
        slowMotorSpeed = (int)(speed * turnRatio);
    }
    
    leftForward(speed);            // Keep the left motor at full speed
    rightBackward(slowMotorSpeed);  // Set right motor to calculated speed
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
    
    // Minimum motor speed threshold
    const int MIN_MOTOR_SPEED = 700;
    
    // Gradually reduce speed until both motors stop
    while (leftSpeed > 0 || rightSpeed > 0) {
        // Reduce speeds
        leftSpeed = max(0, leftSpeed - deceleration);
        rightSpeed = max(0, rightSpeed - deceleration);
        
        // If speed drops below minimum threshold but is not zero, set it to zero
        if (leftSpeed > 0 && leftSpeed < MIN_MOTOR_SPEED) {
            leftSpeed = 0;
        }
        
        if (rightSpeed > 0 && rightSpeed < MIN_MOTOR_SPEED) {
            rightSpeed = 0;
        }
        
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

/**
 * Turn the robot toward a detected target
 * 
 * @param target The target to turn toward
 * @param speed The speed at which to turn (0-255)
 */
void turnTowardTarget(const Target& target, int speed) {
    // Check if the target is valid
    if (!target.isValid) {
        stopMotors();
        return;
    }

    // Camera center X position
    // Based on the actual target positions in the logs, the camera resolution appears to be different
    // Target positions are consistently around 390-420, so adjusting center accordingly
    const int CAMERA_CENTER_X = 400;
    
    // Deadzone to prevent constant small adjustments
    const int DEADZONE = 20;
    
    // Calculate how far the target is from the center
    int offsetFromCenter = target.centerX - CAMERA_CENTER_X;
    
    // If target is within the deadzone, stop turning
    if (abs(offsetFromCenter) < DEADZONE) {
        stopMotors();
        return;
    }
    
    // Adjust turning speed based on how far the target is from center
    // The farther from center, the faster the turn (up to the maximum provided speed)
    int adjustedSpeed = map(abs(offsetFromCenter), DEADZONE, CAMERA_CENTER_X, speed/3, speed);
    adjustedSpeed = constrain(adjustedSpeed, 0, speed);
    
    // Turn the robot toward the target
    if (offsetFromCenter < 0) {
        // Target is to the left, rotate counterclockwise
        rotateInPlace(adjustedSpeed, false);
    } else {
        // Target is to the right, rotate clockwise
        rotateInPlace(adjustedSpeed, true);
    }
}
