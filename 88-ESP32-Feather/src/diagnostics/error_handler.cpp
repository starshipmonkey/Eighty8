#include "error_handler.h"
#include "logging.h"

// Flag to track if an error is currently active
static bool errorActive = false;
static String currentError = "";

void handleError(const char* error) {
    errorActive = true;
    currentError = String(error);
    Logger::error(error);
}

void handleError(const String& error) {
    errorActive = true;
    currentError = error;
    Logger::error(error);
}

bool hasError() {
    return errorActive;
}

void clearError() {
    if (errorActive) {
        Logger::info("Error cleared: " + currentError);
        errorActive = false;
        currentError = "";
    }
}

#include "error_handler.h"

void handleError(const char* error) {
    logInfo(error);
    // Additional error handling logic
}

