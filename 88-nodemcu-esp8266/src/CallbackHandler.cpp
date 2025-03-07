#include "CallbackHandler.h"
#include "DetectionUtils.h"

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
}
