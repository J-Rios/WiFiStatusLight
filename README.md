# WiFiStatusLight

RGB Light to monitorize and shows WiFi connection status.

Actual Network Status LED Show:
- LED RED ON: Device not connected to AP.
- LED RED Blinking: Device connected but waiting for IP.
- LED GREEN ON: Device connected and has IP, but no Internet connection.
- LED BLUE ON: Device connected, with IP and Internet connection. Everything is OK.
- LED PURPLE ON: Device is receiving an OTA update and is upgrading to new version.

### Notes:
- Device ESP32.
- Platformio project using Visual Studio Code.
- Project using Espressif ESP-IDF Framework (Platformio platform-espressif32 v1.5.0 == ESP-IDF v3.1.1).
