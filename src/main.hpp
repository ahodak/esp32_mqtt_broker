#pragma once

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <PicoMQTT.h>

#ifdef ESP32
#include <WiFi.h>
#include <WebServer.h>
#elif ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#endif

#define SERIAL_BAUD_RATE 115200

#define WIFI_CONNECTION_ATTEMPTS 20
#define WIFI_ATTEMPTS_DELAY 1000

#define AP_SSID "MQTT-Broker-Setup"
#define AP_PASSWORD "12345678"

#define ONE_SECOND 1000
