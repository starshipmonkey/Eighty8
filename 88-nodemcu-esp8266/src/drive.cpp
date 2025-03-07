#include <Arduino.h>      // Ensures compatibility with the Arduino framework

/*
   Board pin | NodeMCU GPIO |  Arduino IDE
      A-           1             5 or D1
      A+           3             0 or D3
      B-           2             4 or D2
      B+           4             2 or D4
*/

const int pwmMotorA = 5;
const int pwmMotorB = 4;
const int dirMotorA = 2;
const int dirMotorB = 0;


int motorSpeed = 1023;

void driveSetup() {
  Serial.begin(9600);
  Serial.println();

  pinMode(pwmMotorA, OUTPUT);
  pinMode(pwmMotorB, OUTPUT);
  pinMode(dirMotorA, OUTPUT);
  pinMode(dirMotorB, OUTPUT);

  Serial.println("Motor SHield 12E Initialized");
  delay(5000);

  for (int i = 0; i <= 2; i++) {
    Serial.println("Activate A");
    analogWrite(pwmMotorA, motorSpeed);
    digitalWrite(dirMotorA, LOW);

    Serial.println("Activate B");
    analogWrite(pwmMotorB, motorSpeed);
    digitalWrite(dirMotorB, LOW);
    delay(1500);

    Serial.println("Reverse A");
    digitalWrite(dirMotorA, HIGH);

    Serial.println("Reverse B");
    digitalWrite(dirMotorB, HIGH);
    delay(1500);
  }
}

void driveLoop() {
  //stopMotors();
}

void stopMotors() {
  digitalWrite(pwmMotorA, LOW);
  digitalWrite(dirMotorA, LOW);
  Serial.println("Stop B");
  digitalWrite(pwmMotorB, LOW);
  digitalWrite(dirMotorB, LOW);
}

void moveMotors() {
  // Move motorA forward  
  Serial.println("Activate A");
  analogWrite(pwmMotorA, motorSpeed);
  digitalWrite(dirMotorA, LOW);

    // Move motorB forward
  Serial.println("Activate B");
  analogWrite(pwmMotorB, motorSpeed);
  digitalWrite(dirMotorB, LOW);

  // Wait for 1.5 seconds
  delay(1500);

// Reverse motorA
  Serial.println("Reverse A");
  digitalWrite(dirMotorA, HIGH);

    // Reverse motorB
  Serial.println("Reverse B");
  digitalWrite(dirMotorB, HIGH);

  // Wait for 1.5 seconds
  delay(1500);
}
