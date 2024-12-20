#include "config_manager.h"

// Конструктор
ConfigManager::ConfigManager() {
}

// Сохранение настроек
void ConfigManager::save(String filename, JsonDocument config) {
    File file = SPIFFS.open(filename, "w");
    if (!file) {
        Serial.println("Error opening config file");
        return;
    }
    
    serializeJson(config, file);

    file.close();

    Serial.println("Config saved");
}

// Загрузка настроек
JsonDocument ConfigManager::load(String filename) {
    if (SPIFFS.exists(filename)) {
        File file = SPIFFS.open(filename, "r");
        JsonDocument result;
        DeserializationError error = deserializeJson(result, file);
        
        if (error) {
            Serial.println("Error loading config");
        }
        
        file.close();

        return result;
    }
    else {
        Serial.println("Config file " + filename + " not found");
        return JsonDocument();
    }
}

// Вывод конфигурации в Serial
void ConfigManager::print(String filename) {
    JsonDocument config = this->load(filename);
    serializeJson(config, Serial);
}
