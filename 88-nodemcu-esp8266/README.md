# NodeMCU ESP8266 - Motor Control & Tracking System

This component serves as the "brain" of Project Eighty8, handling motion control, target tracking, and decision-making functionality for the robot.

## System Overview

The NodeMCU ESP8266 receives target detection data (faces and people) from the ESP32C3 + Grove Vision AI module via ESP-NOW wireless communication, then:

1. Prioritizes detected targets based on type and characteristics
2. Controls differential steering for horizontal tracking and movement
3. Manages servo motors for the 2-joint neck to handle vertical tracking

## Hardware Components

- **NodeMCU ESP8266 12E**: Main microcontroller
- **Differential Drive System**: Two DC motors for left/right movement
- **2-Joint Neck Servos**: Controls vertical aiming of the camera

## Key Features

### Target Processing

- Receives ESP-NOW messages containing target detection data
- Separates detections into face and person categories
- Prioritizes targets with face detections having higher priority than person detections
- For faces: Prioritizes based on size (larger faces = closer = higher priority)
- For people: Prioritizes based on confidence score

### Motion Control

#### Differential Steering
Due to the robot's design constraints, the head cannot look side-to-side. Therefore:
- Horizontal tracking is achieved by rotating the entire robot body
- The differential drive system adjusts the relative speeds of left/right wheels
- The robot turns toward targets by varying motor speeds

#### Vertical Tracking
- Servo motors control the 2-joint neck
- Allows the camera to tilt up and down to track targets vertically
- Compensates for different target heights and distances

## Technical Implementation

### Motor Control

The system provides precise control over the robot's movement:
- Forward/reverse motion with variable speed
- Turn-in-place by reversing one motor
- Differential steering to make smooth arcs while moving

### Target Selection Logic

The software automatically selects the most relevant target using this priority system:
1. Face detections take precedence over person detections
2. When multiple faces are detected, the largest face (closest) is selected
3. When only person detections are available, the highest confidence detection is selected
4. Targets must exceed a minimum confidence threshold (default: 25%)

## Code Structure

- **Main.cpp**: System initialization and main loop
- **Receiver.cpp**: Handles ESP-NOW communication and target data parsing
- **MotorControl.cpp**: Controls differential drive motors
- **ServoControl.cpp**: Manages neck servo positioning
- **Target.h**: Defines target data structures and priorities

## Setup Instructions

1. Configure the ESP8266 with PlatformIO using the provided platformio.ini
2. Connect the servo motors to the designated pins
3. Connect the motor controller to the appropriate pins
4. Ensure the ESP8266 has the same channel and MAC address settings as configured in the ESP32C3 sender

## Communication Protocol

The ESP8266 receives data from the ESP32C3 in the following format:
- Target type (face/person)
- Confidence value (0-100)
- Bounding box coordinates (x, y, width, height)
- Timestamp

This data is then processed to determine optimal motor and servo positions for tracking.

