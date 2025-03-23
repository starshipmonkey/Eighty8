#ifndef EIGHTY8_CONFIG_PINS_H
#define EIGHTY8_CONFIG_PINS_H

#include <Arduino.h>

// ESP32 pin definitions for Adafruit HUZZAH32 Feather

// I2C pins
#define PIN_SDA 21
#define PIN_SCL 22

// I2C device addresses
#define TCA9548A_ADDRESS 0x70
#define GROVE_VISION_ADDRESS 0x60
#define MPU6050_ADDRESS 0x68
#define HMC5883L_ADDRESS 0x1E
#define VL53L0X_ADDRESS 0x29

// SPI pins (if needed)
#define PIN_MOSI 18
#define PIN_MISO 19
#define PIN_SCK 5
#define PIN_SS 33

// Motor control pins
#define PIN_MOTOR_LEFT_FWD 12
#define PIN_MOTOR_LEFT_REV 13
#define PIN_MOTOR_RIGHT_FWD 14
#define PIN_MOTOR_RIGHT_REV 27

// PWM pins for motor speed control
#define PIN_MOTOR_LEFT_PWM 15
#define PIN_MOTOR_RIGHT_PWM 32

// Servo pins
#define PIN_SERVO_JOINT1 25
#define PIN_SERVO_JOINT2 26

// Status LED pins
#define PIN_LED_BUILTIN 13  // Built-in LED on Feather
#define PIN_STATUS_LED 21   // External status LED

// Analog pins for sensors or battery monitoring
#define PIN_BATTERY_LEVEL 34  // ADC pin for battery level monitoring

// Additional pins as needed
#define PIN_RESET 4  // Reset pin
#define PIN_ENABLE 2  // Enable pin

#endif // EIGHTY8_CONFIG_PINS_H
