# 88-xiao-esp32c3: Vision Processing Module

## Overview
This component is responsible for the vision processing system of Project Eighty8, a small interactive robot designed for entertainment and companionship. The Xiao ESP32C3 integrates with a Grove Vision AI V2 module to detect and track faces and people, then sends this data to the NodeMCU controller which handles motor control.

## Hardware Components
- **Seeed Studio XIAO ESP32C3**: Compact microcontroller with built-in WiFi and Bluetooth
- **Grove Vision AI V2 Module**: Machine learning-enabled camera module that can detect objects, faces, and people
- **I2C Connection**: Used for communication between the ESP32C3 and Grove Vision AI module

## System Architecture
The ESP32C3 component serves as the "eyes" of the robot:
1. **Vision Processing**: Utilizes the Grove Vision AI module to detect faces and people
2. **Data Analysis**: Processes detection information including position, size, and confidence level
3. **Wireless Communication**: Transmits structured detection data to the NodeMCU using ESP-NOW protocol

## Vision Processing Details
The system utilizes Seeed's SSCMA (Seeed Studio Compact Machine learning Algorithm) library to:
- Run inference using pre-trained machine learning models on the Grove Vision AI module
- Detect multiple classes of objects (primarily faces and people)
- Extract bounding box coordinates, confidence scores, and object types
- Format this data for transmission to the motor control system

The detection information includes:
- X and Y coordinates of the detected object (normalized)
- Width and height of the detection box
- Confidence score (0-100%)
- Object type (FACE or PERSON)

## ESP-NOW Communication
The system uses ESP-NOW for low-latency wireless communication:
- One-way communication from ESP32C3 to NodeMCU
- Structured data messages containing detection information
- Real-time updates for smooth tracking performance

Detection data is formatted as structured messages that include:
- Target type (face or person)
- Target position (x, y coordinates)
- Target size (width, height)
- Detection confidence

## Physical Integration
The ESP32C3 with Grove Vision AI module is mounted on the robot's 2-joint neck, creating the "head" of the robot. Due to the design constraints, the robot:
- Uses its vertical neck joints (controlled by servos) for looking up and down
- Must rotate its entire body using differential steering to look left or right

## Setup and Configuration
To set up this module:
1. Connect the Grove Vision AI module to the ESP32C3 using I2C
2. Update the `receiverAddress` in the code with your NodeMCU's MAC address
3. Flash the code to the ESP32C3 using PlatformIO or Arduino IDE
4. Ensure the ESP32C3 and NodeMCU can communicate via ESP-NOW

## Dependencies
- Arduino framework
- ESP32 Arduino Core
- Seeed Arduino SSCMA library for Grove Vision AI module
- ESP-NOW library for wireless communication

## Development Notes
- The detection sensitivity can be adjusted by modifying the confidence threshold in the code
- The system is designed to handle multiple simultaneous detections
- Detection data is filtered and formatted before transmission to optimize communication efficiency

