#ifndef EIGHTY8_DIAGNOSTICS_ERROR_HANDLER_H
#define EIGHTY8_DIAGNOSTICS_ERROR_HANDLER_H

#include <Arduino.h>
#include "../config/constants.h"
#include "logging_macros.h"

class ErrorHandler {
public:
    static void init();
    static void handleError(ErrorCode code, const String& message);
    static void handleSystemStatus(SystemComponent component, bool isOperational);
    static bool hasActiveError() { return _activeErrorCode != ErrorCode::NONE; }
    static ErrorCode getActiveError() { return _activeErrorCode; }
    static void clearErrors();
    
private:
    static ErrorCode _activeErrorCode;
    static unsigned long _lastErrorTime;
    static uint8_t _errorCount;
    static bool _componentStatus[5]; // Matches number of SystemComponent enum values
    
    static void triggerErrorResponse(ErrorCode code);
    static void logErrorStatus();
};

#endif // EIGHTY8_DIAGNOSTICS_ERROR_HANDLER_H
