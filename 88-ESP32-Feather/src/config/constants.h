#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>

// System version
#define FIRMWARE_VERSION "0.1.0"
#define PROJECT_NAME "Project Eighty8"
#define DEVICE_NAME "ESP32-Robot"

// System parameters
#define SERIAL_BAUD_RATE 115200
#define SYSTEM_BOOT_DELAY 1000  // ms

// WiFi and OTA parameters
#define WIFI_CONNECTION_TIMEOUT 30000  // 30 seconds
#define WIFI_RECONNECT_INTERVAL 5000   // 5 seconds
#define OTA_PORT 3232
#define OTA_HOSTNAME "eighty8-robot"
#define MAX_WIFI_CONNECT_ATTEMPTS 10
#define WIFI_RSSI_CHECK_INTERVAL 30000  // Check WiFi signal strength every 30 seconds

// Telnet server parameters
#define TELNET_PORT 23
#define MAX_TELNET_CLIENTS 2
#define TELNET_TIMEOUT 100  // ms

// I2C parameters
#define I2C_CLOCK_SPEED 400000  // 400 kHz
#define I2C_SDA_PIN 23
#define I2C_SCL_PIN 22

// Buffer sizes
#define LOG_BUFFER_SIZE 256
#define MAX_MESSAGE_SIZE 1024
#define TELNET_BUFFER_SIZE 256
#define COMMAND_BUFFER_SIZE 128

// Timeouts and intervals
#define DEFAULT_TIMEOUT 1000  // 1 second default timeout
#define SAMPLE_RATE 1000  // Sample system metrics every 1 second
#define SYSTEM_MONITOR_INTERVAL 10000  // Check system health every 10 seconds
#define TELNET_POLLING_INTERVAL 100  // Check for telnet commands every 100 ms
#define SERIAL_INIT_DELAY 500  // Short delay to ensure serial is ready (ms)
#define STATUS_CHECK_INTERVAL 30000  // Check system status every 30 seconds
#define RECOVERY_ATTEMPT_INTERVAL 60000  // Try system recovery every 60 seconds
#define I2C_CHECK_INTERVAL 15000  // Check I2C devices every 15 seconds
#define DEVICE_CHECK_INTERVAL 5000   // Check I2C devices every 5 seconds
#define HEARTBEAT_INTERVAL 10000  // Log heartbeat message every 10 seconds
#define WIFI_STRENGTH_CHECK_INTERVAL 60000  // Check WiFi signal strength every 60 seconds
#define HEAP_CHECK_INTERVAL 120000  // Check heap memory every 2 minutes
#define TEMPERATURE_CHECK_INTERVAL 300000  // Check system temperature every 5 minutes

// Logging
#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4
#define LOG_LEVEL_VERBOSE 5

// Default log level
#define DEFAULT_LOG_LEVEL LOG_LEVEL_INFO

// System monitoring thresholds
#define WEAK_WIFI_THRESHOLD -70  // dBm, threshold for weak WiFi signal warning
#define LOW_MEMORY_THRESHOLD 10000  // bytes, threshold for low memory warning
#define HIGH_TEMP_THRESHOLD 80.0f   // Celsius, threshold for high temperature warning
#define LOW_BATTERY_THRESHOLD 15  // Percent, threshold for low battery warning
#define LOW_BATTERY_THRESHOLD 15  // Percent, threshold for low battery warning
#define CRITICAL_BATTERY_THRESHOLD 5  // Percent, threshold for critical battery warning

// Error handling parameters
#define MAX_ERROR_COUNT 3           // Maximum number of errors before taking action
#define ERROR_RESET_INTERVAL 60000  // Reset error count after 1 minute
// System component status
enum class SystemComponent {
    WIFI,
    I2C,
    SENSORS,
    MEMORY,
    TEMPERATURE
};

// Error codes
enum class ErrorCode {
    NONE,
    WIFI_DISCONNECTED,
    I2C_DEVICE_FAILURE,
    LOW_MEMORY,
    HIGH_TEMPERATURE,
    SENSOR_ERROR
};

// TCA9548A I2C multiplexer addresses

// Sensor I2C addresses
#define MPU6050_ADDRESS 0x68
#define HMC5883L_ADDRESS 0x1E
#define VL53L0X_ADDRESS 0x29
#define GROVE_VISION_AI_ADDRESS 0x62

#endif // CONSTANTS_H
