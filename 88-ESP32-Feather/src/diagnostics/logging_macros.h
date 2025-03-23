#ifndef EIGHTY8_DIAGNOSTICS_LOGGING_MACROS_H
#define EIGHTY8_DIAGNOSTICS_LOGGING_MACROS_H

#include "logging.h"

// Logging macros that map to Logger class methods
#define LOG_ERROR(msg, ...) Logger::error(String(msg))
#define LOG_WARN(msg, ...) Logger::warn(String(msg))
#define LOG_INFO(msg, ...) Logger::info(String(msg))
#define LOG_DEBUG(msg, ...) Logger::debug(String(msg))

#endif // EIGHTY8_DIAGNOSTICS_LOGGING_MACROS_H

