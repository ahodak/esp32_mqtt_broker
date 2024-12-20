#pragma once

#include "config_manager.h"

#define CONFIG_FILE_NAME "/config.json"
#define DEFAULT_REBOOT_DELAY 10
#define DEFAULT_DATA_DELAY 10

// Класс для работы с общими настройками
class CommonConfig {
private:
    int _rebootDelay;
    int _dataDelay;
    bool _isChanged = false;

    ConfigManager configManager;

public:
    CommonConfig();

    int rebootDelay();
    void rebootDelay(int newRebootDelay);

    int dataDelay();
    void dataDelay(int newDataDelay);

    bool load();
    bool save();
};
