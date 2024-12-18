#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

// Класс для работы с конфигурацией
class ConfigManager {   
public:
    // Конструктор
    ConfigManager();
    // Сохранение конфигурации
    void save(String filename, JsonDocument config);
    // Загрузка конфигурации
    JsonDocument load(String filename);
    // Вывод конфигурации в Serial
    void print(String filename);
};