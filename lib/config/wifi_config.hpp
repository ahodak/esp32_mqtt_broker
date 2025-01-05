#pragma once

#include "config_manager.hpp"

#define WIFI_CONFIG_FILE_NAME "/wifi_config.json"
#define DEFAULT_SSID "PashaMaster"
#define DEFAULT_PASSWORD "secret"

// Класс для работы с настройками WiFi
class WiFiConfig {
private:
    String _ssid;
    String _password;
    bool _isChanged = false;

    ConfigManager configManager;

public:
    WiFiConfig();

    String ssid();
    void ssid(String newSsid);

    String password();
    void password(String newPassword);

    bool load();
    bool save();
};
