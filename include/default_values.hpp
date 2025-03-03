/*
 *  Значения по умолчанию для проекта ESP-MQTT-Broker.
 *  -----------------------------------------------------------------------------------------------------------------------
 *  (c) 2025 Ходак Андрей | Andrey Khodak
 *  andrey@khodak.ru | https://khodak.ru | tg: @akhodak
*/
#pragma once

#define CONFIG_FILE "/config.json"
#define DEFAULT_SSID "YourSSID"
#define DEFAULT_PASSWORD "YourPassword"
#define DEFAULT_HOSTNAME "esp-mqtt"
#define DEFAULT_MQTT_USERNAME "admin"
#define DEFAULT_MQTT_PASSWORD "admin"
#define DEFAULT_REBOOT_DELAY 5

#ifdef USE_SENSORS
#define DEFAULT_TEMPERATURE_TOPIC "opentherm/sensors/room_temp/set"
#define DEFAULT_TEMPERATURE_0 0.0f
#define DEFAULT_TEMPERATURE_100 100.0f
#endif
