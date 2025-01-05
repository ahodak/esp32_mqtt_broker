#pragma once

#include "config_manager.hpp"

#define CONFIG_FILE_NAME "/config.json"
#define DEFAULT_REBOOT_DELAY 10
#define DEFAULT_DATA_DELAY 10
#define DEFAULT_MQTT_USER "admin"
#define DEFAULT_MQTT_PASSWORD "admin"

// Класс для работы с общими настройками
class CommonConfig {
private:
    int _rebootDelay;
    int _dataDelay;
    String _mqttUser;
    String _mqttPassword;
    bool _isChanged = false;

    ConfigManager configManager;

public:
    CommonConfig();

    int rebootDelay();
    void rebootDelay(int newRebootDelay);

    int dataDelay();
    void dataDelay(int newDataDelay);

    String mqttUser();
    void mqttUser(String newMqttUser);

    String mqttPassword();
    void mqttPassword(String newMqttPassword);

    bool load();
    bool save();
};
