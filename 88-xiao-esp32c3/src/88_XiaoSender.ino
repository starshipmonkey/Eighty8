#include <Seeed_Arduino_SSCMA.h>
#include <esp_now.h>
#include <WiFi.h>

uint8_t receiverMacAddress[] = { 0x40, 0x91, 0x51, 0x65, 0x81, 0xB3 };

SSCMA AI;
JsonDocument info;


// Helper function to decode a Base64 character
inline unsigned char decode_base64_char(char c) {
  if (c >= 'A' && c <= 'Z') return c - 'A';
  if (c >= 'a' && c <= 'z') return c - 'a' + 26;
  if (c >= '0' && c <= '9') return c - '0' + 52;
  if (c == '+') return 62;
  if (c == '/') return 63;
  return -1;
}

// Decode Base64 string
String base64_decode(const String& input) {
  // Check if the input length is valid
  if (input.length() % 4 != 0) {
    throw std::invalid_argument("Invalid Base64 input length");
  }

  String output;
  std::vector<char> temp(3);

  size_t padding = 0;
  if (input.length() >= 2) {
    if (input[input.length() - 1] == '=') padding++;
    if (input[input.length() - 2] == '=') padding++;
  }

  for (size_t i = 0; i < input.length(); i += 4) {
    unsigned char b1 = decode_base64_char(input[i]);
    unsigned char b2 = decode_base64_char(input[i + 1]);
    unsigned char b3 = input[i + 2] == '=' ? 0 : decode_base64_char(input[i + 2]);
    unsigned char b4 = input[i + 3] == '=' ? 0 : decode_base64_char(input[i + 3]);

    temp[0] = (b1 << 2) | (b2 >> 4);
    temp[1] = ((b2 & 0x0F) << 4) | (b3 >> 2);
    temp[2] = ((b3 & 0x03) << 6) | b4;

    output += temp[0];
    if (input[i + 2] != '=') output += temp[1];
    if (input[i + 3] != '=') output += temp[2];
  }

  return output;
}


void setup() {
  Serial.begin(115200);  // Increased baud rate for faster debug output
  Serial.println("Starting setup...");

  AI.begin();
  Serial.println("AI initialized");

  // Initialize WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();  // Ensure we're not connected to any network
  delay(100);         // Short delay to ensure WiFi mode is set

  Serial.println("WiFi set to STA mode");

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  Serial.println("ESP-NOW initialized");

  // Add peer
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));  // Zero out the structure
  memcpy(peerInfo.peer_addr, receiverMacAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
  } else {
    Serial.println("Peer added successfully");
  }

  // Rest of your setup code...
  DeserializationError error = deserializeJson(info, base64_decode(AI.info()));
  if (error) {
    Serial.println("Can not fetch Model Information");
    while (1) {
      delay(100);
    }
  }
  Serial.println("Setup completed");
}


void loop() {
  String message = "";
  if (!AI.invoke()) {
    Serial.println("invoke success\n");
   // Serial.println("perf: prepocess=" + String(AI.perf().prepocess) + ", inference=" + String(AI.perf().inference) + ", postpocess=" + String(AI.perf().postprocess) + "\n");

    for (int i = 0; i < AI.boxes().size(); i++) {
      message += "Box[" + String(i) + "] target=" + info["classes"][AI.boxes()[i].target].as<String>() + ", score=" + String(AI.boxes()[i].score) + ", x=" + String(AI.boxes()[i].x) + ", y=" + String(AI.boxes()[i].y) + ", w=" + String(AI.boxes()[i].w) + ", h=" + String(AI.boxes()[i].h) + "\n";
    }

    // Add similar code for classes, points, and keypoints

    Serial.print(message);  // Print to serial monitor

    // Send via ESP-NOW
    esp_err_t result = esp_now_send(receiverMacAddress, (uint8_t*)message.c_str(), message.length());

    if (result == ESP_OK) {
      Serial.println("Message sent successfully");
    } else {
      Serial.println("Error sending the message");
    }
  }

  delay(1000);
}