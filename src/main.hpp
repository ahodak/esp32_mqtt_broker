/*
 *  Главный файл проекта ESP-MQTT-Broker
 *  -----------------------------------------------------------------------------------------------------------------------
 *  (c) 2025 Ходак Андрей | Andrey Khodak
 *  andrey@khodak.ru | https://khodak.ru | tg: @akhodak
*/
#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PicoMQTT.h>
#include <Preferences.h>
#include <esp_log.h>

#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClientSecure.h>
#include <HTTPUpdate.h>

#include "app_version.hpp"
#include "default_values.hpp"
#include "network_params.hpp"
#include "display.hpp"
#include "WebSrv.hpp"
#include "mqtt.hpp"

extern const uint8_t binary_include_github_ca_pem_start[] asm("_binary_include_github_ca_pem_start");
extern const uint8_t binary_include_github_ca_pem_end[] asm("_binary_include_github_ca_pem_end");

#define GITHUB_HOST  "raw.githubusercontent.com"
#define VERSION_URL  "/ahodak/esp32_mqtt_broker/master/include/app_version.hpp"
#define FIRMWARE_URL "/ahodak/esp32_mqtt_broker/master/firmware/ttgo-t-display/firmware.bin"
#define OTA_ATTEMPTS 3
#define OTA_DELAY    1000

#ifdef ARDUINO_WT32_ETH01
#include <ETH.h>

#define ETH_ADDR        1
#define ETH_POWER_PIN   16//-1 //16 // Do not use it, it can cause conflict during the software reset.
#define ETH_POWER_PIN_ALTERNATIVE 16 //17
#define ETH_MDC_PIN     23
#define ETH_MDIO_PIN    18
#define ETH_TYPE        ETH_PHY_LAN8720
#define ETH_CLK_MODE    ETH_CLOCK_GPIO17_OUT // ETH_CLOCK_GPIO0_IN

#endif

#define SERIAL_BAUD_RATE 115200

#define CONNECTION_ATTEMPTS 10
#define CONNECTION_ATTEMPTS_DELAY 1000

#define AP_SSID "MQTT-Broker-Setup"
#define AP_PASSWORD "12345678"

#define DATA_DELAY 1000
#define ONE_SECOND 1000
