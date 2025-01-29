/*
 *  Параметры сети для проекта ESP-MQTT-Broker
 *  -----------------------------------------------------------------------------------------------------------------------
 *  (c) 2025 Ходак Андрей | Andrey Khodak
 *  andrey@khodak.ru | https://khodak.ru | tg: @akhodak
*/
#pragma once

#include <Arduino.h>

struct NetworkParams {
    String mac;
    String ip;
    String hostname;
    bool isWiFi;
    String ssid;
    String rssi;
    bool isAP;
};
