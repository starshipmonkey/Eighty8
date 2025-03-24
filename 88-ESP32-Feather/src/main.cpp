#include <Arduino.h>
#include "config/constants.h"
#include "config/pins.h"
#include "diagnostics/logging_macros.h"
#include "diagnostics/error_handler.h"
#include "diagnostics/system_monitor.h"
#include "communication/wifi_ota.h"
#include "communication/i2c_manager.h"

// Function declaration
void checkSystemHealth();
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
    
    // Initialize Telnet server
    Logger::setupTelnet(TELNET_PORT);
    LOG_INFO("Telnet server started on port " + String(TELNET_PORT));
    
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
    // Handle Telnet connections
    Logger::handleTelnet();
    
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
        
        // Check WiFi connectivity (SystemComponent::WIFI)
        if (!WifiOta::isConnected()) {
            LOG_WARN("WiFi connection lost, attempting to reconnect...");
            WifiOta::reconnect();
            
            // Report system component status
            SystemMonitor::setComponentStatus(SystemComponent::WIFI, false);
            
            // Handle error if reconnection doesn't work within timeout
            if (!WifiOta::isConnected()) {
                LOG_ERROR("Failed to reconnect to WiFi!");
                ErrorHandler::handleError(ErrorCode::WIFI_DISCONNECTED, "WiFi connection lost");
            }
        } else {
            // Check WiFi signal strength
            int signalStrength = WifiOta::getSignalStrength();
            if (signalStrength < WEAK_WIFI_THRESHOLD) {
                LOG_WARN("Weak WiFi signal: " + String(signalStrength) + " dBm");
            }
            
            // Set WiFi component status to operational
            SystemMonitor::setComponentStatus(SystemComponent::WIFI, true);
        }
        
        // Check memory status (SystemComponent::MEMORY)
        uint32_t freeHeap = SystemMonitor::getFreeHeap();
        if (freeHeap < LOW_MEMORY_THRESHOLD) {
            LOG_WARN("Low memory warning: " + String(freeHeap) + " bytes free");
            SystemMonitor::setComponentStatus(SystemComponent::MEMORY, false);
            
            // Handle critical memory condition
            if (freeHeap < LOW_MEMORY_THRESHOLD / 2) {
                LOG_ERROR("Critical memory condition: " + String(freeHeap) + " bytes free");
                ErrorHandler::handleError(ErrorCode::LOW_MEMORY, "Memory critically low");
            }
        } else {
            SystemMonitor::setComponentStatus(SystemComponent::MEMORY, true);
        }
        
        // Check temperature (SystemComponent::TEMPERATURE)
        float temperature = SystemMonitor::getTemperature();
        if (temperature > HIGH_TEMP_THRESHOLD) {
            LOG_WARN("High temperature warning: " + String(temperature) + " C");
            SystemMonitor::setComponentStatus(SystemComponent::TEMPERATURE, false);
            
            // Handle critical temperature condition
            if (temperature > HIGH_TEMP_THRESHOLD + 10.0f) {
                LOG_ERROR("Critical temperature condition: " + String(temperature) + " C");
                ErrorHandler::handleError(ErrorCode::HIGH_TEMPERATURE, "Temperature too high");
            }
        } else {
            SystemMonitor::setComponentStatus(SystemComponent::TEMPERATURE, true);
        }
        
        // Check I2C devices (SystemComponent::I2C)
        if (!I2CManager::isInitialized()) {
            LOG_ERROR("I2C bus not initialized!");
            SystemMonitor::setComponentStatus(SystemComponent::I2C, false);
            ErrorHandler::handleError(ErrorCode::I2C_DEVICE_FAILURE, "I2C bus failure");
        } else if (!I2CManager::checkDevices()) {
            LOG_WARN("Some I2C devices not responding");
            SystemMonitor::setComponentStatus(SystemComponent::I2C, false);
            
            // Report which devices are having issues
            I2CManager::reportDeviceStatus();
            
            // Try to reset the I2C bus if persistent failures
            static int i2cFailureCount = 0;
            if (++i2cFailureCount > MAX_ERROR_COUNT) {
                LOG_WARN("Attempting I2C bus reset due to persistent failures");
                I2CManager::reset();
                i2cFailureCount = 0;
            }
        } else {
            SystemMonitor::setComponentStatus(SystemComponent::I2C, true);
            // Reset failure count on success
            static int i2cFailureCount = 0;
            i2cFailureCount = 0;
        }
        
        // Note: SENSORS checks have been removed as per system requirements
        // Sensor status is now tracked through I2C component status
    }
}
