#pragma once

#include "config_manager.h"

#define CONFIG_FILE_NAME "/config.json"
#define DEFAULT_REBOOT_DELAY 10

// Класс для работы с общими настройками
class CommonConfig {
private:
    int _rebootDelay;
    bool _isChanged = false;

    ConfigManager configManager;

public:
    CommonConfig();

    int rebootDelay();
    void rebootDelay(int newRebootDelay);

    bool load();
    bool save();
};