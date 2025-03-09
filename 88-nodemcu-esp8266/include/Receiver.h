#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include "Target.h"

// Define data structure for ESP-NOW communication
typedef struct {
  uint8_t msgType;      // Message type (0: heartbeat, 1: target data)
  uint8_t targetType;   // Target type (0: none, 1: face, 2: person)
  uint8_t targetCount;  // Number of targets detected
  uint16_t x;           // X coordinate of target center
  uint16_t y;           // Y coordinate of target center
  uint16_t width;       // Width of bounding box
  uint16_t height;      // Height of bounding box
  uint8_t confidence;   // Confidence score (0-100)
  uint32_t timestamp;   // Timestamp of detection
} ESPNowData;

// Callback function prototype
typedef void (*OnTargetUpdateCallback)(const Target& target);

// Function declarations
void receiverSetup();
void registerTargetUpdateCallback(OnTargetUpdateCallback callback);
bool isNewTargetAvailable();
void clearCurrentTarget();
void updateTargetTimestamps();

// Receiver state getters
bool isConnected();
unsigned long getLastMessageTime();
uint32_t getMessageCount();
uint32_t getErrorCount();

// For debugging
void printReceiverStatus();

// Internal functions (not usually called directly by other modules)
void onDataReceived(uint8_t* mac, uint8_t* data, uint8_t len);
void processReceivedTargets();
void updateCurrentTarget(const Target& newTarget);
void calculateTargetPriorities();

