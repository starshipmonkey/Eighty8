#ifndef EIGHTY8_DIAGNOSTICS_SYSTEM_MONITOR_H
#define EIGHTY8_DIAGNOSTICS_SYSTEM_MONITOR_H

#include <Arduino.h>
#include "../config/constants.h"
#include "../communication/i2c_manager.h"
#include "logging_macros.h"

// Callback function type for status changes
typedef void (*StatusChangeCallback)(SystemComponent component, bool isOperational);

class SystemMonitor {
public:
    static void init();
    static void update();
    static float getTemperature();
    static uint32_t getFreeHeap();
    static int8_t getWifiSignal();
    static bool checkDevices();
    
    // Register a callback for status changes
    static void registerStatusChangeCallback(StatusChangeCallback callback);
    
    // Set the status of a component
    static void setComponentStatus(SystemComponent component, bool isOperational);
    
    // Get the status of a component
    static bool getComponentStatus(SystemComponent component);
    
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
    
    static StatusChangeCallback _statusCallback;
    static bool _componentStatus[static_cast<int>(SystemComponent::TEMPERATURE) + 1];
};

#endif // EIGHTY8_DIAGNOSTICS_SYSTEM_MONITOR_H

