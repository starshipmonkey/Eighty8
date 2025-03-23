#ifndef EIGHTY8_DIAGNOSTICS_LOGGING_H
#define EIGHTY8_DIAGNOSTICS_LOGGING_H

#include <Arduino.h>
#include <WiFi.h>
#include "../config/constants.h"

/**
 * @brief Logger class for handling system logging with multiple levels and telnet support
 * 
 * Provides static methods for logging at different severity levels (ERROR, WARN, INFO, DEBUG)
 * Supports output to both Serial and Telnet clients
 * Includes timestamp and log level prefixes
 */
class Logger {
public:
    /**
     * @brief Initialize the logging system
     * 
     * @param serialBaudRate The baud rate for serial communication
     */
    static void begin(unsigned long serialBaudRate = SERIAL_BAUD_RATE);

    /**
     * @brief Set up telnet server for remote logging
     * 
     * @param port Port to use for telnet server
     */
    static void setupTelnet(uint16_t port = TELNET_PORT);

    /**
     * @brief Process incoming telnet clients
     * 
     * Call this method regularly to handle telnet connections
     */
    static void handleTelnet();

    /**
     * @brief Log error message
     * 
     * @param message Message to log
     */
    static void error(const String& message);

    /**
     * @brief Log warning message
     * 
     * @param message Message to log
     */
    static void warn(const String& message);

    /**
     * @brief Log info message
     * 
     * @param message Message to log
     */
    static void info(const String& message);

    /**
     * @brief Log debug message
     * 
     * @param message Message to log
     */
    static void debug(const String& message);

    /**
     * @brief Set the minimum log level to display
     * 
     * @param level Minimum level to display (0=ERROR, 1=WARN, 2=INFO, 3=DEBUG)
     */
    static void setLogLevel(uint8_t level);

private:
    static WiFiServer _telnetServer;
    static WiFiClient _telnetClients[MAX_TELNET_CLIENTS];
    static uint8_t _logLevel;
    static bool _telnetEnabled;
    static bool _initialized;

    /**
     * @brief Internal logging method
     * 
     * @param level Log level (0=ERROR, 1=WARN, 2=INFO, 3=DEBUG)
     * @param prefix Prefix string for the log level
     * @param message Message to log
     */
    static void log(uint8_t level, const char* prefix, const String& message);

    /**
     * @brief Get formatted timestamp string
     * 
     * @return String Formatted timestamp (HH:MM:SS.mmm)
     */
    static String getTimestamp();
};

#endif // EIGHTY8_DIAGNOSTICS_LOGGING_H

