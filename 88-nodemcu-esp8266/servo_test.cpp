#include <Arduino.h>
#include <Servo.h>

Servo upperNeckServo;
Servo lowerNeckServo;


void HeadSetup() {
  Serial.begin(9600);
  upperNeckServo.attach(D7, 750, 1300);  // Attach the servo to pin D1 (change as needed)
  lowerNeckServo.attach(D8, 820, 1400);  // Attach the servo to pin D1 (change as needed)
}

void HeadDown() {
  lowerNeckServo.write(0);
  upperNeckServo.write(0);
}

void HeadUp() {
  lowerNeckServo.write(180);
  upperNeckServo.write(180);
}
