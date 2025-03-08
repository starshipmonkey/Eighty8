#include <Arduino.h>
#include <Servo.h>
#include "ServoControl.h"

// Create servo objects
Servo upperNeckServo;
Servo lowerNeckServo;

int upperNeckAngleMax = 180;
int upperNeckAngleMin = 0;
int lowerNeckAngleMax = 180;
int lowerNeckAngleMin = 0;

void servoSetup()
{
    Serial.begin(115200);
    upperNeckServo.attach(D7, 750, 1300); // Attach the servo to pin D1 (change as needed)
    lowerNeckServo.attach(D8, 820, 1400); // Attach the servo to pin D1 (change as needed)
}

void servoSetUpper(int angle1)
{
    if (angle1 > upperNeckAngleMax)
    {
        Serial.println("Upper neck angle is too high");
    }
    else if (angle1 < upperNeckAngleMin)
    {
        Serial.println("Upper neck angle is too low");
    }
    else
    {
        upperNeckServo.write(angle1);
    };
}

void servoSetLower(int angle2)
{
    if (angle2 > lowerNeckAngleMax)
    {
        Serial.println("Lower neck angle is too high");
    }
    else if (angle2 < lowerNeckAngleMin)
    {
        Serial.println("Lower neck angle is too low");
    }
    else
    {
        lowerNeckServo.write(angle2);
    };
}

void servoSetBoth(int angle1, int angle2)
{
    servoSetUpper(angle1);
    servoSetLower(angle2);
}

void servoUpperOneUp()
{
    int angle = upperNeckServo.read();
    angle += 1;
    servoSetUpper(angle);
}

void servoUpperOneDown()
{
    int angle = upperNeckServo.read();
    angle -= 1;
    servoSetUpper(angle);
}

void actionHeadAway()
{
    lowerNeckServo.write(0);
    upperNeckServo.write(0);
}

void actionHeadOut()
{
    lowerNeckServo.write(180);
    upperNeckServo.write(180);
}

void actionLeanIn()
{
    servoSetLower(0);
}

void actionLeanOut()
{
    servoSetLower(90);
}

void actionNeutral()
{
    servoSetLower(45);
}

void actionShiver(int shivers, int amplitude)
{
    for (int i = 0; i < shivers; i++)
    {
        servoSetUpper(upperNeckServo.read() + amplitude);
        delay(100);
        servoSetUpper(upperNeckServo.read() - amplitude);
        delay(100);
    }
}

void actionGroove(int grooves, int amplitude)
{
    for (int i = 0; i < grooves; i++)
    {
        servoSetLower(lowerNeckServo.read() + amplitude);
        delay(100);
        servoSetLower(lowerNeckServo.read() - amplitude);
        delay(100);
    }
}

void headSetup()
{
    upperNeckServo.attach(D7, 750, 1300);
    lowerNeckServo.attach(D8, 820, 1400);
}

void headUp()
{
    lowerNeckServo.write(180);
    upperNeckServo.write(180);
}

void headDown()
{
    lowerNeckServo.write(0);
    upperNeckServo.write(0);
}

void headCenter()
{
    lowerNeckServo.write(90);
    upperNeckServo.write(90);
}