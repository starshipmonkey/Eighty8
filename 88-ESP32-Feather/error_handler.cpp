#include "error_handler.h"

// Static member initialization
ErrorCode ErrorHandler::_activeErrorCode = ErrorCode::NONE;
unsigned long ErrorHandler::_lastErrorTime = 0;
uint8_t ErrorHandler::_errorCount = 0;
bool ErrorHandler::_componentStatus[5] = {true}; // Initialize all components as operational

void ErrorHandler::init() {
    _activeErrorCode = ErrorCode::NONE;
    _lastErrorTime = 0;
    _errorCount = 0;
    for (int i = 0; i < 5; i++) {
        _componentStatus[i] = true;
    }
    LOG_INFO("Error Handler initialized");
}

void ErrorHandler::handleError(ErrorCode code, const String& message) {
    if (code == ErrorCode::NONE) {
        return;
    }

    _activeErrorCode = code;
    _lastErrorTime = millis();
    _errorCount++;

    // Log the error with its code and message
    LOG_ERROR("Error " + String(static_cast<int>(code)) + ": " + message);

    // Trigger appropriate response based on the error
    triggerErrorResponse(code);
}

void ErrorHandler::handleSystemStatus(SystemComponent component, bool isOperational) {
    int index = static_cast<int>(component);
    if (index >= 0 && index < 5) {
        _componentStatus[index] = isOperational;
        logErrorStatus();
    }
}

void ErrorHandler::clearErrors() {
    if (_activeErrorCode != ErrorCode::NONE) {
        LOG_INFO("Clearing active error: " + String(static_cast<int>(_activeErrorCode)));
        _activeErrorCode = ErrorCode::NONE;
        _errorCount = 0;
    }
}

void ErrorHandler::triggerErrorResponse(ErrorCode code) {
    // Implement specific responses based on error code
    switch (code) {
        case ErrorCode::MEMORY_LOW:
            // Memory-specific response
            LOG_WARN("Low memory condition detected");
            break;
        case ErrorCode::TEMPERATURE_HIGH:
            // Temperature-specific response
            LOG_WARN("High temperature condition detected");
            break;
        case ErrorCode::WIFI_DISCONNECTED:
            // WiFi-specific response
            LOG_WARN("WiFi connection lost");
            break;
        case ErrorCode::I2C_DEVICE_ERROR:
            // I2C-specific response
            LOG_WARN("I2C device communication error");
            break;
        default:
            LOG_WARN("Unhandled error code: " + String(static_cast<int>(code)));
            break;
    }
}

void ErrorHandler::logErrorStatus() {
    String status = "System Status: ";
    for (int i = 0; i < 5; i++) {
        status += "Component " + String(i) + ": " + 
                (_componentStatus[i] ? "OK" : "ERROR") + " | ";
    }
    LOG_INFO(status);
}

