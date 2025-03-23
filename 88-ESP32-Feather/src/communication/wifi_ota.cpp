#include "wifi_ota.h"
#include <ArduinoOTA.h>
#include "../diagnostics/logging.h"

// Static member initialization
bool WifiOta::_initialized = false;

// Initialize WiFi and OTA functionality
bool WifiOta::init() {
    if (_initialized) {
        Logger::info("WiFi already initialized");
        return true;
    }

    Logger::info("Initializing WiFi connection...");
    
    // Set WiFi mode
    WiFi.mode(WIFI_STA);
    
    // Begin WiFi connection
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    // Wait for connection with timeout
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Logger::debug("Connecting to WiFi...");
        
        // Check for timeout
        if (millis() - startTime > WIFI_CONNECTION_TIMEOUT) {
            Logger::error("WiFi connection timeout");
            return false;
        }
    }
    
    // Log connection information
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Connected to WiFi. IP address: %s", WiFi.localIP().toString().c_str());
    Logger::info(buffer);
    
    // Initialize mDNS
    if (!MDNS.begin(OTA_HOSTNAME)) {
        Logger::error("Error setting up MDNS responder");
        return false;
    }
    
    // Initialize OTA
    initOTA();
    
    _initialized = true;
    return true;
}

// Handle OTA update requests
void WifiOta::handleOtaUpdates() {
    if (!_initialized) {
        return;
    }
    
    ArduinoOTA.handle();
}

// Reconnect to WiFi if disconnected
void WifiOta::reconnect() {
    if (WiFi.status() == WL_CONNECTED) {
        return;
    }
    
    Logger::info("Reconnecting to WiFi...");
    
    // Begin WiFi connection
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    // Wait for connection with timeout
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Logger::debug("Reconnecting to WiFi...");
        
        // Check for timeout
        if (millis() - startTime > WIFI_CONNECTION_TIMEOUT) {
            Logger::error("WiFi reconnection timeout");
            return;
        }
    }
    
    // Log reconnection information
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Reconnected to WiFi. IP address: %s", WiFi.localIP().toString().c_str());
    Logger::info(buffer);
}

// Check if WiFi is connected
bool WifiOta::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

// Get WiFi signal strength
int WifiOta::getSignalStrength() {
    if (!isConnected()) {
        return -100; // Very weak signal value if not connected
    }
    
    return WiFi.RSSI();
}

// Get current IP address
IPAddress WifiOta::getIPAddress() {
    return WiFi.localIP();
}

// Initialize OTA updates
void WifiOta::initOTA() {
    Logger::info("Setting up OTA updates...");
    
    // Configure OTA update server
    ArduinoOTA.setHostname(OTA_HOSTNAME);
    ArduinoOTA.setPassword(OTA_PASSWORD);
    
    // OTA start callback
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else {
            type = "filesystem";
        }
        Logger::info("OTA update started: " + type);
    });
    
    // OTA end callback
    ArduinoOTA.onEnd([]() {
        Logger::info("OTA update complete");
    });
    
    // OTA progress callback
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        char buffer[50];
        snprintf(buffer, sizeof(buffer), "OTA Progress: %u%%", (progress / (total / 100)));
        Logger::debug(buffer);
    });
    
    // OTA error callback
    ArduinoOTA.onError([](ota_error_t error) {
        String errorMsg = "OTA Error: ";
        if (error == OTA_AUTH_ERROR) {
            errorMsg += "Authentication Failed";
        } else if (error == OTA_BEGIN_ERROR) {
            errorMsg += "Begin Failed";
        } else if (error == OTA_CONNECT_ERROR) {
            errorMsg += "Connect Failed";
        } else if (error == OTA_RECEIVE_ERROR) {
            errorMsg += "Receive Failed";
        } else if (error == OTA_END_ERROR) {
            errorMsg += "End Failed";
        } else {
            errorMsg += "Unknown Error";
        }
        Logger::error(errorMsg);
    });
    
    // Start OTA server
    ArduinoOTA.begin();
    Logger::info("OTA server initialized");
}
