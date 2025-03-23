#include "logging.h"
#include "../config/constants.h"
#include <Arduino.h>

WiFiServer Logger::_telnetServer(TELNET_PORT);
WiFiClient Logger::_telnetClients[MAX_TELNET_CLIENTS];
uint8_t Logger::_logLevel = DEFAULT_LOG_LEVEL;
bool Logger::_telnetEnabled = false;
bool Logger::_initialized = false;

void Logger::begin(unsigned long serialBaudRate) {
    if (_initialized) return;
    
    Serial.begin(serialBaudRate);
    delay(SERIAL_INIT_DELAY); // Give serial time to initialize
    
    _initialized = true;
}

void Logger::setupTelnet(uint16_t port) {
    if (!_initialized) return;
    
    _telnetServer.begin(port);
    _telnetEnabled = true;
}

void Logger::handleTelnet() {
    if (!_telnetEnabled) return;
    
    // Check for new clients
    if (_telnetServer.hasClient()) {
        // Find free slot
        int freeSlot = -1;
        for (int i = 0; i < MAX_TELNET_CLIENTS; i++) {
            if (!_telnetClients[i].connected()) {
                freeSlot = i;
                break;
            }
        }
        
        if (freeSlot != -1) {
            _telnetClients[freeSlot] = _telnetServer.available();
            _telnetClients[freeSlot].println("Welcome to Eighty8 Robot Debug Console");
        }
    }
    
    // Check clients for data
    for (int i = 0; i < MAX_TELNET_CLIENTS; i++) {
        if (_telnetClients[i].connected() && _telnetClients[i].available()) {
            // Handle client commands here if needed
            _telnetClients[i].read(); // Clear input buffer
        }
    }
}

void Logger::error(const String& message) {
    log(LOG_LEVEL_ERROR, "[ERROR] ", message);
}

void Logger::warn(const String& message) {
    log(LOG_LEVEL_WARN, "[WARN] ", message);
}

void Logger::info(const String& message) {
    log(LOG_LEVEL_INFO, "[INFO] ", message);
}

void Logger::debug(const String& message) {
    log(LOG_LEVEL_DEBUG, "[DEBUG] ", message);
}

void Logger::setLogLevel(uint8_t level) {
    _logLevel = level;
}

void Logger::log(uint8_t level, const char* prefix, const String& message) {
    if (!_initialized || level > _logLevel) return;
    
    String timestamp = getTimestamp();
    String logMessage = timestamp + " " + prefix + message;
    
    // Send to Serial
    Serial.println(logMessage);
    
    // Send to connected telnet clients
    if (_telnetEnabled) {
        for (int i = 0; i < MAX_TELNET_CLIENTS; i++) {
            if (_telnetClients[i].connected()) {
                _telnetClients[i].println(logMessage);
            }
        }
    }
}

String Logger::getTimestamp() {
    unsigned long ms = millis();
    unsigned long seconds = ms / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    
    char timestamp[32];
    snprintf(timestamp, sizeof(timestamp), "%02lu:%02lu:%02lu.%03lu",
            hours % 24, minutes % 60, seconds % 60, ms % 1000);
    return String(timestamp);
}

#include "logging.h"

void initLogging() {
    Serial.begin(115200);
    // Additional logging setup if needed
}

void logInfo(const char* message) {
    Serial.println(message);
}

