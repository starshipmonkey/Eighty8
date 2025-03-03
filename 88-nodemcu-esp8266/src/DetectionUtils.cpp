#include "DetectionUtils.h"
#include <algorithm> // For std::max_element

// Global vectors to store detected faces and persons
std::vector<DetectedObject> detectedFaces;
std::vector<DetectedObject> detectedPersons;

// Function to determine the highest priority target
Target getHighestPriorityTarget(int scoreThreshold) {
  // Initialize target with default values
  Target target = {"none", 0, 0};  // Default to no target

  // First, check if any faces were detected
  if (!detectedFaces.empty()) {
    // Find the biggest face using std::max_element
    auto biggestFace = std::max_element(detectedFaces.begin(), detectedFaces.end(),
      [](const DetectedObject& a, const DetectedObject& b) {
        // Compare the areas of the faces
        return (a.w * a.h) < (b.w * b.h);
      });
    
    // Check if the biggest face meets the score threshold
    if (biggestFace->score >= scoreThreshold) {
      target.type = "face";
      // Calculate the center point of the face
      target.x = biggestFace->x + (biggestFace->w / 2);
      target.y = biggestFace->y + (biggestFace->h / 2);
      return target;
    }
  }

  // If no suitable face found, check for persons
  if (!detectedPersons.empty()) {
    // Find the person with the highest score
    auto highestScorePerson = std::max_element(detectedPersons.begin(), detectedPersons.end(),
      [](const DetectedObject& a, const DetectedObject& b) {
        // Compare the scores of the persons
        return a.score < b.score;
      });
    
    // Check if the highest scoring person meets the threshold
    if (highestScorePerson->score >= scoreThreshold) {
      target.type = "person";
      // Calculate the center point of the person
      target.x = highestScorePerson->x + (highestScorePerson->w / 2);
      target.y = highestScorePerson->y + (highestScorePerson->h / 2);
    }
  }

  // Return the target (will be "none" if no suitable target was found)
  return target;
}

// Function to parse a single detection from a string
void parseDetection(const String& data) {
  // Variables to store parsed data
  int boxId, score, x, y, w, h;
  char target[10];  // Will store either "face" or "person"
  
  // Use sscanf to parse the data string into individual variables
  int parsed = sscanf(data.c_str(), "Box[%d] target=%9[^,], score=%d, x=%d, y=%d, w=%d, h=%d",
                      &boxId, target, &score, &x, &y, &w, &h);

  // Check if all 7 values were successfully parsed
  if (parsed == 7) {
    // Create a DetectedObject with the parsed values
    DetectedObject obj = {boxId, score, x, y, w, h};

    // Check the target type and add the object to the appropriate vector
    if (strcmp(target, "face") == 0) {
      detectedFaces.push_back(obj);
      Serial.println("Added face detection");
    } else if (strcmp(target, "person") == 0) {
      detectedPersons.push_back(obj);
      Serial.println("Added person detection");
    } else {
      Serial.println("Unknown target: " + String(target));
    }
  } else {
    // If parsing failed, print an error message
    Serial.println("Failed to parse detection data. Parsed " + String(parsed) + " values.");
  }
}
