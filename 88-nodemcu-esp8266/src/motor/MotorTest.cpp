#include <Arduino.h>
#include "config/MotorControl.h"
#include "../../include/MotorTest.h"

// Motor test module - provides test functionality that can be called from the main program
// Test configuration
const int TEST_DURATION = 3000;  // Duration for each test in milliseconds
const int PAUSE_DURATION = 1000; // Pause between tests in milliseconds
const int MOTOR_SPEED = 1000;     // Standard speed for motor tests (0-255)
const int SLOW_SPEED = 500;      // Slower speed for some tests

// Test state management
enum TestState {
  START,
  FORWARD,
  BACKWARD,
  TURN_LEFT,
  TURN_RIGHT,
  ROTATE_CLOCKWISE,
  ROTATE_COUNTERCLOCKWISE,
  PRECISION_FORWARD,
  PRECISION_LEFT,
  PRECISION_RIGHT,
  GRADUAL_STOP,
  END,
  PAUSE
};

TestState currentTest = START;
TestState nextTest = FORWARD;
unsigned long stateStartTime = 0;
bool motorTestIsRunning = false;

void printTestHeader(const char* testName) {
  Serial.println("\n----------------------------------------");
  Serial.print("TEST: ");
  Serial.println(testName);
  Serial.println("----------------------------------------");
}

void motorTestSetup() {
  // Initialize serial communication
  // Serial is assumed to be already initialized in the main program
  
  // Wait a moment for the serial monitor to connect
  delay(2000);
  
  // Print welcome message
  Serial.println("\n\n========================================");
  Serial.println("      MOTOR CONTROL TEST PROGRAM       ");
  Serial.println("========================================");
  Serial.println("This program will test all motor control functions");
  Serial.println("Watch the robot's movements and check if they match");
  Serial.println("the expected behavior described in the serial output.");
  Serial.println("========================================\n");
  
  // Initialize the motors
  motorSetup();
  
  // Start the test sequence
  stateStartTime = millis();
  motorTestIsRunning = true;
  currentTest = START;
}

void runCurrentTest() {
  switch (currentTest) {
    case START:
      printTestHeader("Starting test sequence");
      Serial.println("Make sure the robot is in an open area with sufficient space to move");
      Serial.println("The tests will begin in 3 seconds...");
      nextTest = FORWARD;
      break;
      
    case FORWARD:
      printTestHeader("Moving Forward");
      Serial.println("The robot should move forward in a straight line");
      Serial.println("Both motors should rotate forward at the same speed");
      Serial.println("Expected: Robot moves forward");
      moveForward(MOTOR_SPEED);
      nextTest = BACKWARD;
      break;
      
    case BACKWARD:
      printTestHeader("Moving Backward");
      Serial.println("The robot should move backward in a straight line");
      Serial.println("Both motors should rotate backward at the same speed");
      Serial.println("Expected: Robot moves backward");
      moveBackward(MOTOR_SPEED);
      nextTest = TURN_LEFT;
      break;
      
    case TURN_LEFT:
      printTestHeader("Turning Left");
      Serial.println("The robot should turn left while moving forward");
      Serial.println("The left motor should be slower than the right motor");
      Serial.println("Expected: Robot curves to the left");
      turnLeft(MOTOR_SPEED);
      nextTest = TURN_RIGHT;
      break;
      
    case TURN_RIGHT:
      printTestHeader("Turning Right");
      Serial.println("The robot should turn right while moving forward");
      Serial.println("The right motor should be slower than the left motor");
      Serial.println("Expected: Robot curves to the right");
      turnRight(MOTOR_SPEED);
      nextTest = ROTATE_CLOCKWISE;
      break;
      
    case ROTATE_CLOCKWISE:
      printTestHeader("Rotating Clockwise");
      Serial.println("The robot should rotate clockwise in place");
      Serial.println("The left motor should move forward, right motor backward");
      Serial.println("Expected: Robot spins clockwise");
      rotateInPlace(MOTOR_SPEED, true); // true = clockwise
      nextTest = ROTATE_COUNTERCLOCKWISE;
      break;
      
    case ROTATE_COUNTERCLOCKWISE:
      printTestHeader("Rotating Counter-Clockwise");
      Serial.println("The robot should rotate counter-clockwise in place");
      Serial.println("The right motor should move forward, left motor backward");
      Serial.println("Expected: Robot spins counter-clockwise");
      rotateInPlace(MOTOR_SPEED, false); // false = counter-clockwise
      nextTest = PRECISION_FORWARD;
      break;
      
    case PRECISION_FORWARD:
      printTestHeader("Precision Forward Movement");
      Serial.println("The robot should move forward slowly and precisely");
      Serial.println("Both motors should rotate at a reduced speed");
      Serial.println("Expected: Robot moves forward slowly");
      moveWithPrecision(SLOW_SPEED, 0); // 0 degree = straight
      nextTest = PRECISION_LEFT;
      break;
      
    case PRECISION_LEFT:
      printTestHeader("Precision Left Movement");
      Serial.println("The robot should move precisely at a 45-degree angle to the left");
      Serial.println("Left motor slower than right motor, both moving forward");
      Serial.println("Expected: Robot moves forward while curving left slightly");
      moveWithPrecision(SLOW_SPEED, -45); // -45 degrees = left
      nextTest = PRECISION_RIGHT;
      break;
      
    case PRECISION_RIGHT:
      printTestHeader("Precision Right Movement");
      Serial.println("The robot should move precisely at a 45-degree angle to the right");
      Serial.println("Right motor slower than left motor, both moving forward");
      Serial.println("Expected: Robot moves forward while curving right slightly");
      moveWithPrecision(SLOW_SPEED, 45); // 45 degrees = right
      nextTest = GRADUAL_STOP;
      break;
      
    case GRADUAL_STOP:
      printTestHeader("Gradual Stop");
      Serial.println("The robot should slow down gradually until it stops");
      Serial.println("Both motors should decrease speed gradually");
      Serial.println("Expected: Robot slows down smoothly until stopped");
      moveForward(MOTOR_SPEED); // Start by moving forward
      delay(1000); // Move for a second
      gradualStop(2000); // Gradually stop over 2 seconds
      nextTest = END;
      break;
      
    case END:
      printTestHeader("Test Sequence Complete");
      Serial.println("All motor tests have been completed");
      Serial.println("The test sequence will restart in 5 seconds...");
      stopMotors();
      nextTest = FORWARD; // Loop back to the beginning
      break;
      
    case PAUSE:
      // Just waiting during pause state
      break;
  }
}

void motorTestLoop() {
  if (!motorTestIsRunning) return;  // Exit if test is not running
  
  unsigned long currentTime = millis();
  
  // Determine if we need to transition to the next state
  if (currentTest == PAUSE) {
    // In PAUSE state, wait for PAUSE_DURATION
    if (currentTime - stateStartTime >= PAUSE_DURATION) {
      currentTest = nextTest;
      stateStartTime = currentTime;
      runCurrentTest();
    }
  } else {
    // In a test state, wait for TEST_DURATION
    if (currentTime - stateStartTime >= TEST_DURATION) {
      // Stop motors before transitioning to PAUSE
      stopMotors();
      Serial.println("\nTest complete, stopping motors...");
      
      // Transition to PAUSE state
      currentTest = PAUSE;
      stateStartTime = currentTime;
    }
    
    // Provide periodic status updates for long-running tests
    if (currentTest != START && currentTest != END && currentTest != PAUSE) {
      if ((currentTime - stateStartTime) % 1000 < 20) { // Print approximately every second
        Serial.print(".");
      }
    }
    
    // Special handling for gradual stop test
    if (currentTest == GRADUAL_STOP) {
      // No additional action needed here as gradualStop() is called in runCurrentTest()
    }
  }
}

// Function to stop the motor test if needed
void stopMotorTest() {
  motorTestIsRunning = false;
  stopMotors();  // Make sure motors are stopped when tests are interrupted
}

// Function to check if motor test is running
bool isMotorTestRunning() {
  return motorTestIsRunning;
}

// Function to start the motor test
void startMotorTest() {
  if (!motorTestIsRunning) {
    motorTestSetup();
  }
}

