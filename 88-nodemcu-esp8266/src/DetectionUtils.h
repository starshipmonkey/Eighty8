#ifndef DETECTIONUTILS_H
#define DETECTIONUTILS_H

#include <Arduino.h>
#include <vector>
#include <String>

// Define a structure to hold information about detected objects (faces or persons)
struct DetectedObject {
  int id;    // Unique identifier for the detection
  int score; // Confidence score of the detection (0-100)
  int x;     // X-coordinate of the top-left corner of the bounding box
  int y;     // Y-coordinate of the top-left corner of the bounding box
  int w;     // Width of the bounding box
  int h;     // Height of the bounding box
};

// Define a structure to hold information about the highest priority target
struct Target {
  String type;  // Type of target: "face", "person", or "none"
  int x;        // X-coordinate of the center of the target
  int y;        // Y-coordinate of the center of the target
};

// Function declarations
Target getHighestPriorityTarget(int scoreThreshold);
void parseDetection(const String& line);

// Global vectors to store detected faces and persons
extern std::vector<DetectedObject> detectedFaces;
extern std::vector<DetectedObject> detectedPersons;

#endif // DETECTIONUTILS_H
