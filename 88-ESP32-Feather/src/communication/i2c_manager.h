#ifndef EIGHTY8_COMMUNICATION_I2C_MANAGER_H
#define EIGHTY8_COMMUNICATION_I2C_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include "../config/pins.h"
#include "../diagnostics/logging_macros.h"

class I2CManager {
public:
    static bool init();
    static bool isInitialized() { return _initialized; }
    static bool checkDevices();
    static void reportDeviceStatus();
    static void reset();
    
    static bool writeRegister(uint8_t deviceAddr, uint8_t reg, uint8_t data);
    static bool readRegister(uint8_t deviceAddr, uint8_t reg, uint8_t& data);
    static bool writeRegisters(uint8_t deviceAddr, uint8_t reg, const uint8_t* data, size_t length);
    static bool readRegisters(uint8_t deviceAddr, uint8_t reg, uint8_t* data, size_t length);
    
private:
    static bool _initialized;
    static TwoWire* _i2cBus;  // Pointer to Wire instance
    
    static bool isDeviceConnected(uint8_t address);
    static bool scanI2CBus();
    static void scanMultiplexerChannels();
    static void selectTCAChannel(uint8_t channel);
    static void releaseTCAChannel();
};

#endif // EIGHTY8_COMMUNICATION_I2C_MANAGER_H
