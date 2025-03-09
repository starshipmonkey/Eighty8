#ifndef MOTOR_TEST_H
#define MOTOR_TEST_H

#include <Arduino.h>

// Motor test initialization
void motorTestSetup();

// Motor test main loop function - call this from the main loop to run the test
void motorTestLoop();

// Function to stop the motor test if needed
void stopMotorTest();

// Function to check if motor test is running
bool isMotorTestRunning();

// Function to start the motor test
void startMotorTest();

#endif // MOTOR_TEST_H

