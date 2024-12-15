#include "config_manager.h"

// Сохранение настроек
void saveConfig(JsonDocument config) {
    File file = SPIFFS.open(CONFIG_FILE_NAME, "w");
    if (!file) {
        Serial.println("Error opening config file");
        return;
    }
    
    serializeJson(config, file);

    file.close();

    Serial.println("Config saved");
}

// Загрузка настроек
JsonDocument loadConfig() {
    if (SPIFFS.exists(CONFIG_FILE_NAME)) {
        File file = SPIFFS.open(CONFIG_FILE_NAME, "r");
        JsonDocument result;
        DeserializationError error = deserializeJson(result, file);
        
        if (error) {
            Serial.println("Error loading config");
        }
        else {
            Serial.println("Config loaded");
        }
        
        file.close();

        return result;
    }
    else {
        Serial.println("Config file not found");
        return JsonDocument();
    }
}

// Сохранение настроек WiFi
void saveWiFiConfig(String newSsid, String newPassword) {
    JsonDocument config;

    config["ssid"] = newSsid;
    config["password"] = newPassword;

    saveConfig(config);
}

// Загрузка настроек WiFi
WiFiConfig loadWiFiConfig() {
    JsonDocument config = loadConfig();

    if (config["ssid"].is<String>() && config["password"].is<String>()) { 
        Serial.println("WiFi config loaded");
        return {config["ssid"].as<String>(), config["password"].as<String>()};
    }
    else {
        Serial.println("Config file does not contain ssid or password");
        return {"", ""} ;
    }
}
