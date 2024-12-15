#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

#ifndef CONFIG_FILE_NAME
#define CONFIG_FILE_NAME "/config.json"
#endif

void saveConfig(JsonDocument config);
JsonDocument loadConfig();

struct WiFiConfig {
    String ssid;
    String password;
};

void saveWiFiConfig(String newSsid, String newPassword);
WiFiConfig loadWiFiConfig();

#endif