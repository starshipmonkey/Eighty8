# Project Eighty8

## Overview
Project Eighty8 is an experimental robot designed for novel interaction, entertainment, and companionship. The robot autonomously detects and tracks human faces and bodies, physically moving to maintain "attention" on people in its environment.

## Purpose
This project explores human-machine interaction by creating a device that:
- Automatically detects human presence
- Prioritizes faces over bodies for tracking
- Physically moves and orients itself toward subjects of interest
- Creates a sense of companionship through responsive movement

## System Architecture
Project Eighty8 utilizes a two-device system:

### 1. Vision Processing System (Xiao ESP32C3)
- **Hardware**: ESP32C3 microcontroller + Grove Vision AI V2 camera
- **Function**: 
  - Runs machine learning models to detect faces and people
  - Processes video feed in real-time
  - Calculates target position, size, and confidence values
  - Transmits detection data to the motor control system

### 2. Motor Control System (NodeMCU ESP8266)
- **Hardware**: ESP8266 microcontroller + DC motors + Servo motors
- **Function**:
  - Receives target data via ESP-NOW wireless protocol
  - Prioritizes and selects targets based on type and proximity
  - Controls differential steering for horizontal tracking
  - Manages servo motors for vertical neck movement

## Physical Design

The robot features:

- **Mobility System**: Differential steering using two DC motors that allow the robot to rotate in place and move in any direction by varying the relative speed of the motors
- **Articulated Camera**: A 2-joint neck controlled by servo motors, allowing vertical (up/down) aiming of the camera
- **Design Constraint**: The robot must turn its entire body to track targets horizontally, as the neck only provides vertical movement

### Movement & Tracking Mechanics
- **Horizontal Tracking**: When a target moves left or right, the entire robot body rotates using differential steering
- **Vertical Tracking**: The 2-joint neck adjusts to look up or down at targets of different heights
- **Target Prioritization**: Faces are given higher priority than bodies, and larger faces (closer to the robot) receive highest attention

![Screenshot from 2025-03-04 09-16-57](https://github.com/user-attachments/assets/939131a4-f4ea-44aa-b6f4-9f3b496a7ded)
![Screenshot from 2025-03-04 09-17-18](https://github.com/user-attachments/assets/d99ccf16-1747-488a-a5f1-613c7ad502d0)
![Screenshot from 2025-03-04 09-18-04](https://github.com/user-attachments/assets/f45adf84-0d1f-488a-84a3-8853af41180c)
![Screenshot from 2025-03-04 09-17-35](https://github.com/user-attachments/assets/70de6219-a047-4b6f-a827-abcfe1df7301)
![Screenshot from 2025-03-04 09-19-54](https://github.com/user-attachments/assets/8438ab42-3f1d-4edc-ba41-da7b7336b5c8)
![Screenshot from 2025-03-04 09-20-31](https://github.com/user-attachments/assets/d51c4f98-3f8d-4edf-a049-5b78a21bb0f2)

## How It Works

### Vision Detection
1. The Grove Vision AI module continuously processes camera images using embedded machine learning
2. When faces or people are detected, their positions and dimensions are calculated
3. This data is formatted and transmitted via ESP-NOW wireless protocol to the NodeMCU

### Target Selection
1. The NodeMCU receives detection data from the ESP32C3
2. Targets are categorized as either FACE or PERSON
3. The system prioritizes:
   - Faces over people
   - Larger faces (presumed to be closer)
   - Higher-confidence detections

### Motor Control
1. Target coordinates are converted to motor positions
2. DC motors adjust robot body orientation for horizontal tracking
3. Servo motors adjust the neck position for vertical tracking
4. The system continuously updates motor positions as targets move

## Technical Implementation

- **ESP-NOW Communication**: Provides low-latency wireless communication between the two microcontrollers
- **Real-time Vision Processing**: Uses Seeed's SSCMA library for efficient on-device machine learning
- **Differential Steering**: Controls robot rotation by varying relative motor speeds
- **Servo Control**: Provides precise positioning of the 2-joint neck for vertical tracking
- **Target Management**: Implements sophisticated algorithms to select and track the most relevant target

## Project Components
- Grove Vision AI V2 (Camera with embedded ML processing)
- Xiao ESP32C3 (Vision processing controller)
- Node MCU ESP8266 12E (Motor control system)
- DC Motors (Differential steering)
- Servo Motors (2-joint vertical neck control)

