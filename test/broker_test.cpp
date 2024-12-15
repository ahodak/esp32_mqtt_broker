#include <unity.h>
#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <PicoMQTT.h>

#include "../lib/config_manager/config_manager.h"

String test_ssid = "TestSSID";
String test_password = "TestPassword";

// Мок-объекты
::WiFiServer tcp_server(1883);
PicoMQTT::Server broker(tcp_server);

void setUp(void) {
    // Инициализация перед каждым тестом
    SPIFFS.begin(true);
}

void tearDown(void) {
    // Очистка после каждого теста
    SPIFFS.format();
}

// Тест сохранения конфигурации
void test_save_config(void) {
    // Действие
    saveWiFiConfig(test_ssid, test_password);
    
    // Проверка
    File file = SPIFFS.open("/config.json", "r");
    TEST_ASSERT_TRUE(file);
    
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    TEST_ASSERT_FALSE(error);
    
    TEST_ASSERT_EQUAL_STRING(test_ssid.c_str(), doc["ssid"].as<const char*>());
    TEST_ASSERT_EQUAL_STRING(test_password.c_str(), doc["password"].as<const char*>());
    
    file.close();
}

// Тест загрузки конфигурации
void test_load_config(void) {
    // Подготовка
    saveWiFiConfig(test_ssid, test_password);
    
    // Действие
    WiFiConfig config = loadWiFiConfig();
    
    // Проверка
    TEST_ASSERT_EQUAL_STRING(test_ssid.c_str(), config.ssid.c_str());
    TEST_ASSERT_EQUAL_STRING(test_password.c_str(), config.password.c_str());
}

// Тест подключения к WiFi
void test_wifi_connection(void) {
    // Подготовка
    WiFi.disconnect();
    
    // Действие
    WiFi.begin(test_ssid.c_str(), test_password.c_str());
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 5) {
        delay(500);
        attempts++;
    }
    
    // Проверка
    TEST_ASSERT_EQUAL(WL_CONNECTED, WiFi.status());
}

// Тест запуска MQTT брокера
void test_mqtt_broker_start(void) {
    // Действие
    broker.begin();
    
    // Проверка
    TEST_ASSERT_TRUE(tcp_server.hasClient());
}

void setup() {
    delay(2000);
    UNITY_BEGIN();
    
    RUN_TEST(test_save_config);
    RUN_TEST(test_load_config);
    RUN_TEST(test_wifi_connection);
    RUN_TEST(test_mqtt_broker_start);
    
    UNITY_END();
}

void loop() {
}
