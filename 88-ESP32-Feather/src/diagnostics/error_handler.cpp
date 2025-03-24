#include "error_handler.h"
#include "logging.h"

// Static member initialization
ErrorCode ErrorHandler::_activeErrorCode = ErrorCode::NONE;
unsigned long ErrorHandler::_lastErrorTime = 0;
uint8_t ErrorHandler::_errorCount = 0;
bool ErrorHandler::_componentStatus[static_cast<int>(SystemComponent::TEMPERATURE) + 1] = {true};

void ErrorHandler::init() {
    _activeErrorCode = ErrorCode::NONE;
    _lastErrorTime = 0;
    _errorCount = 0;
    LOG_INFO("Error Handler initialized");
}

void ErrorHandler::handleError(ErrorCode code, const String& message) {
    if (code == ErrorCode::NONE) {
        return;
    }
    _activeErrorCode = code;
    _lastErrorTime = millis();
    _errorCount++;
    LOG_ERROR("Error " + String(static_cast<int>(code)) + ": " + message);
    triggerErrorResponse(code);
}

void ErrorHandler::handleSystemStatus(SystemComponent component, bool isOperational) {
    int index = static_cast<int>(component);
    if (index >= 0 && index < sizeof(_componentStatus)/sizeof(_componentStatus[0])) {
        _componentStatus[index] = isOperational;
        logErrorStatus();
    }
}

void ErrorHandler::clearErrors() {
    if (_activeErrorCode != ErrorCode::NONE) {
        LOG_INFO("Clearing error: " + String(static_cast<int>(_activeErrorCode)));
        _activeErrorCode = ErrorCode::NONE;
        _errorCount = 0;
    }
}

void ErrorHandler::triggerErrorResponse(ErrorCode code) {
    switch (code) {
        case ErrorCode::LOW_MEMORY:
            LOG_WARN("Low memory condition detected");
            break;
        case ErrorCode::HIGH_TEMPERATURE:
            LOG_WARN("High temperature condition detected");
            break;
        case ErrorCode::WIFI_DISCONNECTED:
            LOG_WARN("WiFi connection lost");
            break;
        case ErrorCode::I2C_DEVICE_FAILURE:
            LOG_WARN("I2C device communication error");
            break;
        case ErrorCode::SENSOR_ERROR:
            LOG_WARN("Sensor error detected");
            break;
        default:
            LOG_WARN("Unhandled error code: " + String(static_cast<int>(code)));
            break;
    }
}

void ErrorHandler::logErrorStatus() {
    String status = "System Status:";
    for (int i = 0; i <= static_cast<int>(SystemComponent::TEMPERATURE); i++) {
        status += " Component " + String(i) + ": " + 
                (_componentStatus[i] ? "OK" : "ERROR") + " |";
    }
    LOG_INFO(status);
}
