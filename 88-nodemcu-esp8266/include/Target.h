#pragma once

#include <Arduino.h>

// Enum to define target types
enum TargetType {
    NONE = 0,
    FACE = 1,
    PERSON = 2
};

// Target structure to store information about detected objects
struct Target {
    // Whether the target is valid
    bool isValid = false;
    
    // Target type (face or person)
    TargetType type = NONE;
    
    // Target position (center coordinates)
    int16_t centerX = 0;
    int16_t centerY = 0;
    
    // Target bounding box
    int16_t x = 0;
    int16_t y = 0;
    int16_t width = 0;
    int16_t height = 0;
    
    // Confidence score (0-100)
    int16_t confidence = 0;
    
    // Size metric (used for prioritization)
    int32_t size = 0;
};

// Global current highest priority target
extern Target currentTarget;
// Function declarations
void initializeTarget();
bool isTargetAvailable();
Target getHighestPriorityTarget(int scoreThreshold = 25);
void clearTarget();
void updateTargetFromReceiver();

// Helper function to print target information to Serial
void printTargetInfo(const Target& target);

