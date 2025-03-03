// Include necessary libraries
#include <ESP8266WiFi.h>  // Enables WiFi functionality for ESP8266
#include <espnow.h>       // Enables ESP-NOW protocol for communication
#include <vector>         // Allows use of dynamic arrays (vectors)

// Define a structure to hold information about detected objects (faces or persons)
struct DetectedObject {
  int id;    // Unique identifier for the detection
  int score; // Confidence score of the detection (0-100)
  int x;     // X-coordinate of the top-left corner of the bounding box
  int y;     // Y-coordinate of the top-left corner of the bounding box
  int w;     // Width of the bounding box
  int h;     // Height of the bounding box
};

// Create two vectors (dynamic arrays) to store detected faces and persons
std::vector<DetectedObject> detectedFaces;    // Will store all detected faces
std::vector<DetectedObject> detectedPersons;  // Will store all detected persons

// Define a structure to hold information about the highest priority target
struct Target {
  String type;  // Type of target: "face", "person", or "none"
  int x;        // X-coordinate of the center of the target
  int y;        // Y-coordinate of the center of the target
};

// Function to determine the highest priority target
Target getHighestPriorityTarget(int scoreThreshold = 25) {
  // Initialize target with default values
  Target target = {"none", 0, 0};  // Default to no target

  // First, check if any faces were detected
  if (!detectedFaces.empty()) {
    // Find the biggest face using std::max_element
    // This function iterates through the vector and finds the maximum element
    // based on the comparison function we provide (lambda function)
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
  // %d = integer, %9[^,] = string up to 9 characters until a comma is found
  int parsed = sscanf(data.c_str(), "Box[%d] target=%9[^,], score=%d, x=%d, y=%d, w=%d, h=%d",
                      &boxId, target, &score, &x, &y, &w, &h);

  // Check if all 7 values were successfully parsed
  if (parsed == 7) {
    // Create a DetectedObject with the parsed values
    DetectedObject obj = {boxId, score, x, y, w, h};

    // Check the target type and add the object to the appropriate vector
    if (strcmp(target, "face") == 0) {  // strcmp compares strings, returns 0 if they're equal
      detectedFaces.push_back(obj);  // Add the object to the end of detectedFaces vector
      Serial.println("Added face detection");
    } else if (strcmp(target, "person") == 0) {
      detectedPersons.push_back(obj);  // Add the object to the end of detectedPersons vector
      Serial.println("Added person detection");
    } else {
      Serial.println("Unknown target: " + String(target));
    }
  } else {
    // If parsing failed, print an error message
    Serial.println("Failed to parse detection data. Parsed " + String(parsed) + " values.");
  }
}

// Callback function that will be executed when data is received
void onDataReceive(uint8_t *mac, uint8_t *data, uint8_t len) {
  String receivedData = "";
  // Convert received bytes to a string
  for (int i = 0; i < len; i++) {
    receivedData += (char)data[i];
  }
  
  Serial.println("Received raw data:");
  Serial.println(receivedData);

  // Clear previous detections to start fresh
  detectedFaces.clear();
  detectedPersons.clear();

  // Split the received data into lines and parse each line
  int startIndex = 0;
  int endIndex = receivedData.indexOf('\n');  // Find the first newline character
  while (endIndex != -1) {  // Loop until no more newlines are found
    // Extract a single line from the received data
    String line = receivedData.substring(startIndex, endIndex);
    Serial.println("Parsing line: " + line);
    parseDetection(line);  // Parse the extracted line
    startIndex = endIndex + 1;  // Move start index to after the newline
    endIndex = receivedData.indexOf('\n', startIndex);  // Find the next newline
  }
  // Parse the last line if it doesn't end with a newline
  if (startIndex < receivedData.length()) {
    String line = receivedData.substring(startIndex);
    Serial.println("Parsing last line: " + line);
    parseDetection(line);
  }

  // Print the parsed data (for debugging)
  Serial.println("Detected Faces:");
  for (const auto& face : detectedFaces) {
    Serial.printf("  ID: %d, Score: %d, X: %d, Y: %d, W: %d, H: %d\n",
                  face.id, face.score, face.x, face.y, face.w, face.h);
  }
  Serial.println("Detected Persons:");
  for (const auto& person : detectedPersons) {
    Serial.printf("  ID: %d, Score: %d, X: %d, Y: %d, W: %d, H: %d\n",
                  person.id, person.score, person.x, person.y, person.w, person.h);
  }

  // Get the highest priority target
  Target highestPriorityTarget = getHighestPriorityTarget();
  
  // Print the highest priority target information
  Serial.println("Highest Priority Target:");
  Serial.printf("  Type: %s, X: %d, Y: %d\n", 
                highestPriorityTarget.type.c_str(), 
                highestPriorityTarget.x, 
                highestPriorityTarget.y);
  Serial.println("--------------------");
}

void setup() {
  // Initialize Serial Monitor for debugging output
  Serial.begin(115200);  // Set baud rate to 115200
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP-NOW Role to slave (receiver)
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  
  // Register callback function to be called when data is received
  esp_now_register_recv_cb(onDataReceive);
}

void loop() {
  // The loop function is empty because all the work is done in the onDataReceive callback
  // This function is called automatically whenever data is received via ESP-NOW
}
