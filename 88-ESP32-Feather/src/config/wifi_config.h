#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <Arduino.h>
#include <WiFi.h>
#include "config/secrets.h"
#include "config/constants.h"

// WiFi mode
#define WIFI_MODE WIFI_STA  // Station mode - connects to an existing network

// Static IP configuration (optional)
// Comment out to use DHCP
//#define STATIC_IP
#ifdef STATIC_IP
  #define IP_ADDRESS IPAddress(192, 168, 1, 200)
  #define GATEWAY IPAddress(192, 168, 1, 1)
  #define SUBNET IPAddress(255, 255, 255, 0)
  #define PRIMARY_DNS IPAddress(8, 8, 8, 8)
  #define SECONDARY_DNS IPAddress(8, 8, 4, 4)
#endif

// mDNS configuration
#define ENABLE_MDNS true
#define MDNS_HOSTNAME OTA_HOSTNAME

// Telnet server configuration
#define ENABLE_TELNET true
#define TELNET_PORT 23
#define MAX_TELNET_CLIENTS 2

// OTA configuration
#define ENABLE_OTA true
// OTA password is defined in secrets.h

// WiFi power management
#define WIFI_POWER_SAVE false
#define WIFI_POWER_LEVEL WIFI_POWER_19_5dBm  // Maximum power

#endif // WIFI_CONFIG_H
