#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <Arduino.h>

// Function declarations
void servoSetup();
void servoSetUpper(int angle1);
void servoSetLower(int angle2);
void servoSetBoth(int angle1, int angle2);
void servoUpperOneUp();
void servoUpperOneDown();
void actionHeadAway();
void actionHeadOut();
void actionLeanIn();
void actionLeanOut();
void actionNeutral();
void actionShiver(int shivers, int amplitude);
void actionGroove(int grooves, int amplitude);
void headSetup();
void headUp();
void headDown();
void headCenter();

#endif // SERVO_CONTROL_H