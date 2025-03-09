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

    // Start the motor test sequence (commented out for target tracking)
    Serial.println("Motor tests disabled. Using target tracking...");
    // startMotorTest();

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

            // Use target information for motor control
            if (currentTarget.type == FACE)
            {
                Serial.println("Action: Face detected - priority tracking");

                // Turn the robot toward the face
                int trackingSpeed = 150; // Adjust speed as needed (0-255)
                turnTowardTarget(currentTarget, trackingSpeed);

                // Camera center and deadzone values (must match those in turnTowardTarget function)
                const int CAMERA_CENTER_X = 160;
                const int DEADZONE = 200;

                // Calculate offset from center to determine turning direction
                int offsetFromCenter = currentTarget.centerX - CAMERA_CENTER_X;

                // Print the direction that matches the turnTowardTarget function's decision
                if (abs(offsetFromCenter) < DEADZONE)
                {
                    Serial.println("Face is centered - stopping motors");
                    // Note: stopMotors() is already called in turnTowardTarget when in deadzone
                }
                else if (offsetFromCenter < 0)
                {
                    Serial.println("Turning left to track face");
                }
                else
                {
                    Serial.println("Turning right to track face");
                }
            }
            else if (currentTarget.type == PERSON)
            {
                Serial.println("Action: Person detected - general tracking");

                // Turn toward person with speed based on confidence
                int trackingSpeed = map(currentTarget.confidence, 25, 100, 100, 200);
                turnTowardTarget(currentTarget, trackingSpeed);

                if (currentTarget.confidence > 75)
                {
                    Serial.println("High confidence detection - tracking at higher speed");
                }
                else
                {
                    Serial.println("Lower confidence detection - tracking at lower speed");
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
