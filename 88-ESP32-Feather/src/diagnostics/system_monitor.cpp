#include "system_monitor.h"
#include "../communication/wifi_ota.h"

// Static member initialization
unsigned long SystemMonitor::_lastMemoryCheck = 0;
unsigned long SystemMonitor::_lastTemperatureCheck = 0;
unsigned long SystemMonitor::_lastWifiCheck = 0;
unsigned long SystemMonitor::_lastDeviceCheck = 0;

float SystemMonitor::_temperature = 0.0f;
uint32_t SystemMonitor::_freeHeap = 0;
int8_t SystemMonitor::_wifiSignal = 0;
bool SystemMonitor::_devicesOk = false;
StatusChangeCallback SystemMonitor::_statusCallback = nullptr;
bool SystemMonitor::_componentStatus[static_cast<int>(SystemComponent::TEMPERATURE) + 1] = {true};
void SystemMonitor::init() {
    _lastMemoryCheck = millis();
    _lastTemperatureCheck = millis();
    _lastWifiCheck = millis();
    _lastDeviceCheck = millis();
    
    // Initial checks
    checkMemory();
    checkTemperature();
    checkWifi();
    checkI2CDevices();
}

void SystemMonitor::update() {
    unsigned long currentTime = millis();
    
    // Check memory periodically
    if (currentTime - _lastMemoryCheck >= HEAP_CHECK_INTERVAL) {
        checkMemory();
        _lastMemoryCheck = currentTime;
    }
    
    // Check temperature periodically
    if (currentTime - _lastTemperatureCheck >= TEMPERATURE_CHECK_INTERVAL) {
        checkTemperature();
        _lastTemperatureCheck = currentTime;
    }
    
    // Check WiFi periodically
    if (currentTime - _lastWifiCheck >= WIFI_STRENGTH_CHECK_INTERVAL) {
        checkWifi();
        _lastWifiCheck = currentTime;
    }
    
    // Check I2C devices periodically
    if (currentTime - _lastDeviceCheck >= DEVICE_CHECK_INTERVAL) {
        checkI2CDevices();
        _lastDeviceCheck = currentTime;
    }
}

float SystemMonitor::getTemperature() {
    return _temperature;
}

uint32_t SystemMonitor::getFreeHeap() {
    return _freeHeap;
}

int8_t SystemMonitor::getWifiSignal() {
    return _wifiSignal;
}

bool SystemMonitor::checkDevices() {
    return _devicesOk;
}

void SystemMonitor::checkMemory() {
    _freeHeap = ESP.getFreeHeap();
    
    if (_freeHeap < LOW_MEMORY_THRESHOLD) {
        LOG_WARN("Low memory warning: " + String(_freeHeap) + " bytes remaining");
        SystemMonitor::setComponentStatus(SystemComponent::MEMORY, false);
    }
}

void SystemMonitor::checkTemperature() {
    // ESP32 internal temperature sensor reading
    _temperature = (temperatureRead() - 32) / 1.8; // Convert to Celsius
    
    if (_temperature > HIGH_TEMP_THRESHOLD) {
        LOG_WARN("High temperature warning: " + String(_temperature) + "°C");
        SystemMonitor::setComponentStatus(SystemComponent::TEMPERATURE, false);
    }
}

void SystemMonitor::checkWifi() {
    if (!WifiOta::isConnected()) {
        _wifiSignal = -100; // Very weak signal value
        LOG_WARN("WiFi is disconnected");
        SystemMonitor::setComponentStatus(SystemComponent::WIFI, false);
        return;
    }
    
    _wifiSignal = WifiOta::getSignalStrength();
    
    if (_wifiSignal < WEAK_WIFI_THRESHOLD) {
        LOG_WARN("Weak WiFi signal: " + String(_wifiSignal) + " dBm");
        SystemMonitor::setComponentStatus(SystemComponent::WIFI, false);
    }
}

void SystemMonitor::checkI2CDevices() {
    _devicesOk = I2CManager::checkDevices();
    
    if (!_devicesOk) {
        LOG_WARN("One or more I2C devices are not responding");
        I2CManager::reportDeviceStatus();
        SystemMonitor::setComponentStatus(SystemComponent::I2C, false);
    }
}

void SystemMonitor::registerStatusChangeCallback(StatusChangeCallback callback) {
    _statusCallback = callback;
}

void SystemMonitor::setComponentStatus(SystemComponent component, bool isOperational) {
    int componentIndex = static_cast<int>(component);
    if (componentIndex < 0 || componentIndex >= sizeof(_componentStatus)/sizeof(_componentStatus[0])) {
        return;
    }
    
    if (_componentStatus[componentIndex] != isOperational) {
        _componentStatus[componentIndex] = isOperational;
        if (_statusCallback) {
            _statusCallback(component, isOperational);
        }
    }
}

bool SystemMonitor::getComponentStatus(SystemComponent component) {
    int componentIndex = static_cast<int>(component);
    if (componentIndex < 0 || componentIndex >= sizeof(_componentStatus)/sizeof(_componentStatus[0])) {
        return false;
    }
    return _componentStatus[componentIndex];
}

