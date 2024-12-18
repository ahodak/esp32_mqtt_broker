#include "wifi_config.h"

// Конструктор
WiFiConfig::WiFiConfig() {
    this->configManager = ConfigManager();

    this->ssid(DEFAULT_SSID);
    this->password(DEFAULT_PASSWORD);

    if(!SPIFFS.exists(WIFI_CONFIG_FILE_NAME)) {
        Serial.println("Creating WiFi config file");
        this->save();
    }

    this->_isChanged = false;
}

// Получение SSID
String WiFiConfig::ssid() {
    return this->_ssid;
}

// Получение пароля
String WiFiConfig::password() {
    return this->_password;
}

// Установка SSID
void WiFiConfig::ssid(String newSsid) {
    this->_ssid = newSsid;
    this->_isChanged = true;
}

// Установка пароля
void WiFiConfig::password(String newPassword) {
    this->_password = newPassword;
    this->_isChanged = true;
}

// Загрузка настроек
bool WiFiConfig::load() {
    try {
        JsonDocument config = this->configManager.load(WIFI_CONFIG_FILE_NAME);

        this->_ssid = config["ssid"].as<String>();
        this->_password = config["password"].as<String>();
        this->_isChanged = false;

        Serial.println("WiFi config loaded");

        return true;
    } catch (const std::exception& e) {
        Serial.println("Error loading WiFi config");
        return false;
    }
}

// Сохранение настроек
bool WiFiConfig::save() {
    if (this->_isChanged) {
        try {
            JsonDocument config;
            
            config["ssid"] = this->_ssid;
            config["password"] = this->_password;

            this->configManager.save(WIFI_CONFIG_FILE_NAME, config);
            this->_isChanged = false;
        
            Serial.println("WiFi config saved");

            return true;
        } catch (const std::exception& e) {
            Serial.println("Error saving WiFi config");
            return false;
        }
    }

    return true;
}