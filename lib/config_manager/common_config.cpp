#include "common_config.h"

// Конструктор
CommonConfig::CommonConfig() {
    this->configManager = ConfigManager();

    this->rebootDelay(DEFAULT_REBOOT_DELAY);
    this->dataDelay(DEFAULT_DATA_DELAY);

    if(!SPIFFS.exists(CONFIG_FILE_NAME)) {
        Serial.println("Creating Common config file");
        this->save();
    }

    this->_isChanged = false;
}

// Получение задержки перезагрузки
int CommonConfig::rebootDelay() {
    return this->_rebootDelay;
}

// Установка задержки перезагрузки
void CommonConfig::rebootDelay(int newRebootDelay) {
    this->_rebootDelay = newRebootDelay;
    this->_isChanged = true;
}

// Получение периода получения данных
int CommonConfig::dataDelay() {
    return this->_dataDelay;
}

// Установка периода получения данных
void CommonConfig::dataDelay(int newDataDelay) {
    this->_dataDelay = newDataDelay;
    this->_isChanged = true;
}

// Загрузка настроек
bool CommonConfig::load() {
    try {
        JsonDocument config = this->configManager.load(CONFIG_FILE_NAME);

        this->_rebootDelay = config["reboot_delay"].as<int>();
        this->_dataDelay = config["data_delay"].as<int>();
        this->_isChanged = false;
        Serial.println("Common config loaded");

        return true;
    } catch (const std::exception& e) {
        Serial.println("Error loading Common config");
        return false;
    }
}

// Сохранение настроек
bool CommonConfig::save() {
    if (this->_isChanged) {
        try {
            JsonDocument config;

            config["reboot_delay"] = this->_rebootDelay;
            config["data_delay"] = this->_dataDelay;

            this->configManager.save(CONFIG_FILE_NAME, config);
            this->_isChanged = false;

            Serial.println("Common config saved");

            return true;
        } catch (const std::exception& e) {
            Serial.println("Error saving Common config");
            return false;
        }
    }

    return true;
}
