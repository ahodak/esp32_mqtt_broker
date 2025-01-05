#include "common_config.hpp"

// Конструктор
CommonConfig::CommonConfig() {
    this->configManager = ConfigManager();

    this->rebootDelay(DEFAULT_REBOOT_DELAY);
    this->dataDelay(DEFAULT_DATA_DELAY);
    this->mqttUser(DEFAULT_MQTT_USER);
    this->mqttPassword(DEFAULT_MQTT_PASSWORD);

    if(!LittleFS.exists(CONFIG_FILE_NAME)) {
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

String CommonConfig::mqttUser() {
    return this->_mqttUser;
}

void CommonConfig::mqttUser(String newMqttUser) {
    this->_mqttUser = newMqttUser;
    this->_isChanged = true;
}

String CommonConfig::mqttPassword() {
    return this->_mqttPassword;
}

void CommonConfig::mqttPassword(String newMqttPassword) {
    this->_mqttPassword = newMqttPassword;
    this->_isChanged = true;
}

// Загрузка настроек
bool CommonConfig::load() {
    try {
        JsonDocument config = this->configManager.load(CONFIG_FILE_NAME);

        this->_rebootDelay = config["reboot_delay"].as<int>();
        this->_dataDelay = config["data_delay"].as<int>();
        this->_mqttUser = config["mqtt_user"].as<String>();
        this->_mqttPassword = config["mqtt_password"].as<String>();
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
            config["mqtt_user"] = this->_mqttUser;
            config["mqtt_password"] = this->_mqttPassword;

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
