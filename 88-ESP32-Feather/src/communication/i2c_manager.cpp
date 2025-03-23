#include "i2c_manager.h"

bool I2CManager::_initialized = false;
TwoWire* I2CManager::_i2cBus = &Wire;

bool I2CManager::init() {
    if (_initialized) {
        LOG_INFO("I2C already initialized");
        return true;
    }
    
    LOG_INFO("Initializing I2C communication...");
    
    // Initialize main I2C bus
    _i2cBus->begin(PIN_SDA, PIN_SCL);
    _i2cBus->setClock(400000); // Set to 400kHz
    
    // Scan for I2C devices
    if (!scanI2CBus()) {
        LOG_ERROR("Failed to find any I2C devices");
        return false;
    }
    
    // Check if multiplexer is available
    if (isDeviceConnected(TCA9548A_ADDRESS)) {
        LOG_INFO("I2C multiplexer found at address 0x70");
        scanMultiplexerChannels();
    } else {
        LOG_WARN("I2C multiplexer not found, direct I2C communication will be used");
    }
    
    _initialized = true;
    LOG_INFO("I2C initialization complete");
    return true;
}

bool I2CManager::checkDevices() {
    if (!_initialized) return false;
    return scanI2CBus();
}

void I2CManager::reportDeviceStatus() {
    if (!_initialized) return;
    scanI2CBus();
    scanMultiplexerChannels();
}

void I2CManager::reset() {
    _initialized = false;
    init();
}

bool I2CManager::writeRegister(uint8_t deviceAddr, uint8_t reg, uint8_t data) {
    if (!_initialized || !isDeviceConnected(deviceAddr)) return false;
    
    _i2cBus->beginTransmission(deviceAddr);
    _i2cBus->write(reg);
    _i2cBus->write(data);
    return _i2cBus->endTransmission() == 0;
}

bool I2CManager::readRegister(uint8_t deviceAddr, uint8_t reg, uint8_t& data) {
    if (!_initialized || !isDeviceConnected(deviceAddr)) return false;
    
    _i2cBus->beginTransmission(deviceAddr);
    _i2cBus->write(reg);
    if (_i2cBus->endTransmission(false) != 0) return false;
    
    if (_i2cBus->requestFrom(deviceAddr, (uint8_t)1) != 1) return false;
    data = _i2cBus->read();
    return true;
}

bool I2CManager::writeRegisters(uint8_t deviceAddr, uint8_t reg, const uint8_t* data, size_t length) {
    if (!_initialized || !isDeviceConnected(deviceAddr)) return false;
    
    _i2cBus->beginTransmission(deviceAddr);
    _i2cBus->write(reg);
    _i2cBus->write(data, length);
    return _i2cBus->endTransmission() == 0;
}

bool I2CManager::readRegisters(uint8_t deviceAddr, uint8_t reg, uint8_t* data, size_t length) {
    if (!_initialized || !isDeviceConnected(deviceAddr)) return false;
    
    _i2cBus->beginTransmission(deviceAddr);
    _i2cBus->write(reg);
    if (_i2cBus->endTransmission(false) != 0) return false;
    
    if (_i2cBus->requestFrom(deviceAddr, (uint8_t)length) != length) return false;
    for (size_t i = 0; i < length; i++) {
        data[i] = _i2cBus->read();
    }
    return true;
}

bool I2CManager::isDeviceConnected(uint8_t address) {
    _i2cBus->beginTransmission(address);
    return _i2cBus->endTransmission() == 0;
}

bool I2CManager::scanI2CBus() {
    uint8_t deviceCount = 0;
    
    LOG_INFO("Scanning I2C bus for devices...");
    
    for (uint8_t address = 1; address < 127; address++) {
        if (isDeviceConnected(address)) {
            deviceCount++;
            LOG_INFO("I2C device found at address 0x" + String(address, HEX));
            
            if (address == TCA9548A_ADDRESS) {
                scanMultiplexerChannels();
            }
        }
    }
    
    LOG_INFO("Found " + String(deviceCount) + " I2C device(s)");
    return deviceCount > 0;
}

void I2CManager::scanMultiplexerChannels() {
    if (!isDeviceConnected(TCA9548A_ADDRESS)) return;
    
    for (uint8_t channel = 0; channel < 8; channel++) {
        selectTCAChannel(channel);
        LOG_DEBUG("Scanning I2C multiplexer channel " + String(channel));
        
        for (uint8_t address = 1; address < 127; address++) {
            if (address == TCA9548A_ADDRESS) continue;
            
            if (isDeviceConnected(address)) {
                LOG_INFO("Device at channel " + String(channel) + 
                       ", address 0x" + String(address, HEX));
            }
        }
    }
    
    releaseTCAChannel();
}

void I2CManager::selectTCAChannel(uint8_t channel) {
    if (channel > 7) return;
    
    _i2cBus->beginTransmission(TCA9548A_ADDRESS);
    _i2cBus->write(1 << channel);
    _i2cBus->endTransmission();
}

void I2CManager::releaseTCAChannel() {
    _i2cBus->beginTransmission(TCA9548A_ADDRESS);
    _i2cBus->write(0);  // Disable all channels
    _i2cBus->endTransmission();
}
