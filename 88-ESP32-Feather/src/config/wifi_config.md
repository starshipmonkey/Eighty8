# WiFi Configuration Setup

This file explains how to set up the WiFi configuration for the ESP32 Feather project securely.

## Setup Instructions

1. Copy the example configuration file:
   ```
   cp wifi_config.h.example wifi_config.h
   ```

2. Edit `wifi_config.h` with your actual WiFi credentials:
   - Replace "YOUR_WIFI_SSID" with your WiFi network name
   - Replace "YOUR_WIFI_PASSWORD" with your WiFi password

## Security Notes

- NEVER commit `wifi_config.h` to the repository
- NEVER share your `wifi_config.h` file
- The `.gitignore` file is configured to exclude `wifi_config.h`
- Only `wifi_config.h.example` should be in version control

## Troubleshooting

If your ESP32 fails to connect to WiFi:
1. Verify your credentials in `wifi_config.h`
2. Ensure the WiFi network is available
3. Check the serial monitor for connection status

## Warning

If you accidentally commit or push wifi_config.h, immediately:
1. Change your WiFi password
2. Update local wifi_config.h with new credentials
3. Remove the sensitive file from git history
