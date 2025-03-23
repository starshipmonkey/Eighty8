#ifndef EIGHTY8_DIAGNOSTICS_SYSTEM_MONITOR_H
#define EIGHTY8_DIAGNOSTICS_SYSTEM_MONITOR_H

#include <Arduino.h>
#include "../config/constants.h"
#include "../communication/i2c_manager.h"
#include "logging_macros.h"

class SystemMonitor {
public:
    static void init();
    static void update();
    static float getTemperature();
    static uint32_t getFreeHeap();
    static int8_t getWifiSignal();
    static bool checkDevices();
    
private:
    static unsigned long _lastMemoryCheck;
    static unsigned long _lastTemperatureCheck;
    static unsigned long _lastWifiCheck;
    static unsigned long _lastDeviceCheck;
    
    static float _temperature;
    static uint32_t _freeHeap;
    static int8_t _wifiSignal;
    static bool _devicesOk;
    
    static void checkMemory();
    static void checkTemperature();
    static void checkWifi();
    static void checkI2CDevices();
};

#endif // EIGHTY8_DIAGNOSTICS_SYSTEM_MONITOR_H

#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include <Arduino.h>
#include "../config/constants.h"

// System component identifiers
enum SystemComponent {
  COMPONENT_WIFI,
  COMPONENT_I2C,
  COMPONENT_TELNET,
  COMPONENT_MEMORY,    // Added memory component
  COMPONENT_TEMPERATURE, // Added temperature component
  COMPONENT_BATTERY,   // Battery status component
  COMPONENT_COUNT      // Used to determine the total number of components
};

// Callback function type for status changes
typedef void (*StatusChangeCallback)(SystemComponent component, bool isOperational);

class SystemMonitor {
public:
  // Initialize the system monitor
  static bool init();
  
  // Update system monitor with latest data
  static void update();
  
  // Register a callback for status changes
  static void registerStatusChangeCallback(StatusChangeCallback callback);
  
  // Set the status of a component
  static void setComponentStatus(SystemComponent component, bool isOperational);
  
  // Get the status of a component
  static bool getComponentStatus(SystemComponent component);
  
  // Get free heap memory
  static size_t getFreeHeap();
  
  // Get system temperature
  static float getSystemTemperature();
  
private:
  static StatusChangeCallback _statusCallback;
  static bool _componentStatus[COMPONENT_COUNT];
};

#endif // SYSTEM_MONITOR_H

