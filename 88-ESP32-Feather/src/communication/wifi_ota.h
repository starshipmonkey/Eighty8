#ifndef WIFI_OTA_H
#define WIFI_OTA_H

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include "../config/wifi_config.h"
#include "../config/secrets.h"
#include "../config/constants.h"

class WifiOta {
public:
    // Initialize WiFi and OTA functionality
    static bool init();
    
    // Handle OTA update requests
    static void handleOtaUpdates();
    
    // Reconnect to WiFi if disconnected
    static void reconnect();
    
    // Check if WiFi is connected
    static bool isConnected();
    
    // Get WiFi signal strength
    static int getSignalStrength();
    
    // Get current IP address
    static IPAddress getIPAddress();

private:
    static bool _initialized;
    static void initOTA();
};

#endif // WIFI_OTA_H

