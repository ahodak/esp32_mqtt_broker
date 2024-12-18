#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <PicoMQTT.h>


#define SERIAL_BAUD_RATE 115200
#define MQTT_BROKER_PORT 1883

#define WIFI_CONNECTION_ATTEMPTS 20
#define WIFI_ATTEMPTS_DELAY 1000

#define AP_SSID "MQTT-Broker-Setup"
#define AP_PASSWORD "12345678"
