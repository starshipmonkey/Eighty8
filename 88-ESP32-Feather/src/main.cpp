#include <Arduino.h>
#include "config/constants.h"
#include "config/pins.h"
#include "diagnostics/logging_macros.h"
#include "diagnostics/error_handler.h"
#include "diagnostics/system_monitor.h"
#include "communication/wifi_ota.h"
#include "communication/i2c_manager.h"

void setup() {
    // Initialize logging system
    Logger::begin(115200);
    LOG_INFO("Initializing system...");
    
    // Initialize error handling
    ErrorHandler::init();
    if (!ErrorHandler::hasActiveError()) {
        LOG_INFO("Error handling system initialized");
    } else {
        LOG_ERROR("Failed to initialize error handling system!");
        return;
    }
    
    // Initialize system monitoring
    SystemMonitor::init();
    LOG_INFO("System monitoring initialized");
    
    // Initialize WiFi and OTA
    if (!WifiOta::init()) {
        LOG_ERROR("Failed to initialize WiFi!");
        ErrorHandler::handleError(ErrorCode::WIFI_DISCONNECTED, "WiFi initialization failed");
        return;
    }
    LOG_INFO("WiFi initialized, IP: " + WifiOta::getIPAddress().toString());
    
    // Initialize I2C communication
    if (!I2CManager::init()) {
        LOG_ERROR("Failed to initialize I2C communication!");
        ErrorHandler::handleError(ErrorCode::I2C_DEVICE_FAILURE, "I2C initialization failed");
        return;
    }
    LOG_INFO("I2C communication initialized");
    
    LOG_INFO("System initialization complete!");
}

void loop() {
    // Handle OTA updates
    WifiOta::handleOtaUpdates();
    
    // Update system monitoring
    SystemMonitor::update();
    
    // Check system health
    checkSystemHealth();
    
    // Main loop delay
    delay(10);
}

void checkSystemHealth() {
    static unsigned long lastHealthCheck = 0;
    const unsigned long healthCheckInterval = 1000; // Check every second
    
    if (millis() - lastHealthCheck >= healthCheckInterval) {
        lastHealthCheck = millis();
        
        // Check WiFi connectivity
        if (!WifiOta::
