#include <Arduino.h>      // Ensures compatibility with the Arduino framework
#include <ESP8266WiFi.h>  // Enables WiFi functionality for ESP8266
#include <espnow.h>       // Enables ESP-NOW protocol for communication
#include "DetectionUtils.h"
#include "CallbackHandler.h"

// Example function to simulate receiving data and parsing it
void simulateDataReception() {
  // Simulated data string
  String simulatedData = "Box[1] target=face, score=85, x=10, y=20, w=30, h=40\n"
                         "Box[2] target=person, score=90, x=50, y=60, w=70, h=80\n";
  
  // Split the simulated data into lines and parse each line
  int startIndex = 0;
  int endIndex = simulatedData.indexOf('\n');  // Find the first newline character
  while (endIndex != -1) {  // Loop until no more newlines are found
    // Extract a single line from the simulated data
    String line = simulatedData.substring(startIndex, endIndex);
    Serial.println("Parsing line: " + line);
    parseDetection(line);  // Parse the extracted line
    startIndex = endIndex + 1;  // Move start index to after the newline
    endIndex = simulatedData.indexOf('\n', startIndex);  // Find the next newline
  }
  // Parse the last line if it doesn't end with a newline
  if (startIndex < simulatedData.length()) {
    String line = simulatedData.substring(startIndex);
    Serial.println("Parsing last line: " + line);
    parseDetection(line);
  }
}

// Example function to demonstrate getting the highest priority target
void demonstrateHighestPriorityTarget() {
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

  // Simulate data reception and parsing
  simulateDataReception();

  // Demonstrate getting the highest priority target
  demonstrateHighestPriorityTarget();
}

void loop() {
  // The loop function is empty because all the work is done in the onDataReceive callback
  // This function is called automatically whenever data is received via ESP-NOW
}
