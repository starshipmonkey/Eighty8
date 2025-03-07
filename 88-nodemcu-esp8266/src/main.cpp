#include <Arduino.h>
#include "States.h" // Include the header file for States.cpp

// Function declarations
void servoSetup();
void servoSetUpper(int angle1);

// Create an instance of the state machine
StateMachine sm;

void setup()
{
    Serial.begin(9600); // Initialize serial communication
    servoSetup();
    sm.update(); // Initial state update
}

void loop()
{
    servoSetUpper(90);
    delay(1000);

    // Example of state transitions
    sm.transitionTo(State::Engaging);
    sm.update();
    delay(1000);

    sm.transitionTo(State::Waiting);
    sm.update();
    delay(1000);
}
