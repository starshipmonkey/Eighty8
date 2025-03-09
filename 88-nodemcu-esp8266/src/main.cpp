#include <Arduino.h>
#include "Receiver.h"
#include "config/ServoControl.h"
#include "config/MotorControl.h"
#include "Target.h"

// Uncomment the line below to enable motor tests
#include "../include/MotorTest.h"

// Variables for periodic target information display
unsigned long lastTargetPrintTime = 0;
const unsigned long targetPrintInterval = 2000; // Print every 2 seconds

void setup()
{
    receiverSetup(); // This already initializes Serial
    // servoSetup();

    // Uncomment this section to run motor tests
    //  Initialize motors (if not already initialized)
    motorSetup();

    // Start the motor test sequence
    Serial.println("Starting motor tests...");
    startMotorTest();

    // Print startup message
    Serial.println("\nEighty8 system initialized!");
    Serial.println("Waiting for target detection data...");
}

void loop()
{
    // Get the current time
    unsigned long currentTime = millis();

    // Check if it's time to print target information
    if (currentTime - lastTargetPrintTime >= targetPrintInterval)
    {
        // Update the last print time
        lastTargetPrintTime = currentTime;

        // Check if a target is available
        if (isTargetAvailable())
        {
            // Print a header
            Serial.println("\n----- Current Target Status -----");

            // Print information about the current target
            printTargetInfo(currentTarget);

            // Example of using target information for decision-making
            if (currentTarget.type == FACE)
            {
                Serial.println("Action: Face detected - priority tracking");

                // Example: You could use the target position to move servos
                // For instance, if the face is on the left side of the frame
                if (currentTarget.centerX < 120)
                {
                    Serial.println("Target is on the left side - adjust servos accordingly");
                }
                else if (currentTarget.centerX > 200)
                {
                    Serial.println("Target is on the right side - adjust servos accordingly");
                }
                else
                {
                    Serial.println("Target is centered - maintain position");
                }
            }
            else if (currentTarget.type == PERSON)
            {
                Serial.println("Action: Person detected - general tracking");

                // Example: You could use the target confidence for decision-making
                if (currentTarget.confidence > 75)
                {
                    Serial.println("High confidence detection - consider movement");
                }
                else
                {
                    Serial.println("Lower confidence detection - maintain position");
                }
            }
        }
        else
        {
            Serial.println("\nNo valid target currently detected");
        }
    }

    // Other loop operations can continue here
}
