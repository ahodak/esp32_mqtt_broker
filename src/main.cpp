#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <PicoMQTT.h>

#include "config_manager.h"
#include "display.h"

#define SERIAL_BAUD_RATE 115200
#define MQTT_BROKER_PORT 1883
#define WEB_SERVER_PORT 80

#define DEFAULT_SSID "PashaMaster"
#define DEFAULT_PASSWORD "secret"
#define WIFI_CONNECTION_ATTEMPTS 20
#define WIFI_CONNECTION_DELAY 500

#define AP_SSID "MQTT-Broker-Setup"
#define AP_PASSWORD "12345678"

// Настройки WiFi по умолчанию
String ssid = DEFAULT_SSID;
String password = DEFAULT_PASSWORD;
bool isAP = false;

// Создаем объекты
WebServer server(WEB_SERVER_PORT);
::WiFiServer tcp_server(MQTT_BROKER_PORT);
PicoMQTT::Server broker(tcp_server);

// Обработчик главной страницы
void handleRoot() {
    String html = "<html lang='ru'>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<title>PashaMaster MQTT Broker</title>";
    html += "<style>";
    html += "body { background-color: #fff; color: #000; font-family: Arial, sans-serif; }";
    html += "h1 { color: #0000ff; }";
    html += "h2 { color: #006400; }";
    html += "span { color: #ff0000; }";
    html += "</style>";
    html += "</head>";
    html += "<body>";
    html += "<h1>PashaMaster MQTT Broker</h1>";
    html += "<h2>MQTT Settings</h2>";
    html += "Protocol version: <b>3.1.1</b><br>";
    if (isAP) {
        html += "<span>WiFi not connected</span><br>";
        html += "<span>Broker not started</span>";
    } else {
        html += "Broker IP: <b>" + WiFi.localIP().toString() + "</b><br>";
        html += "Broker port: <b>1883</b><br>";
    }
    html += "<h2>WiFi Settings</h2>";
    html += "<form action='/save' method='POST'>";
    html += "SSID: <input type='text' name='ssid' value='" + ssid + "'><br>";
    html += "Password: <input type='password' name='password' value='" + password + "'><br>";
    html += "<br>";
    html += "<input type='submit' value='Save'>";
    html += "</form>";
    html += "&copy; Золин Павел &lt;pashamasterz@yandex.ru&gt;, 2024<br>";
    html += "</body></html>";

    server.send(200, "text/html", html);
}

// Обработчик сохранения настроек
void handleSave() {
    ssid = server.arg("ssid");
    password = server.arg("password");

    saveWiFiConfig(ssid, password);
        
    server.send(200, "text/html", "Settings saved. Rebooting...");

    Serial.println("Settings saved. Rebooting...");
    delay(2000);
    ESP.restart();
}

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

    // Инициализация дисплея
    initDisplay();
    displayLine("Starting...");

    // Инициализация SPIFFS
    if (!SPIFFS.begin(true)) {
        displayError("SPIFFS Error");

        return;
    }

    // Загрузка настроек
    WiFiConfig config = loadWiFiConfig();
    ssid = config.ssid;
    password = config.password;

    // Подключение к WiFi
    WiFi.begin(ssid.c_str(), password.c_str());
    displayLine("Connecting to WiFi...");

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < WIFI_CONNECTION_ATTEMPTS) {
        delay(WIFI_CONNECTION_DELAY);
        attempts++;
        displayProgress(2);
    }

    clearDisplay();

    if (WiFi.status() == WL_CONNECTED) {
        displayLine("WiFi Connected");
        displayLine(WiFi.localIP().toString());

        isAP = false;

        // Запуск MQTT брокера
        broker.begin();
        displayLine("MQTT Broker Started");
    } else {
        displayLine("WiFi Failed");

        isAP = true;

        // Создаем точку доступа для настройки
        WiFi.softAP(AP_SSID, AP_PASSWORD);
        displayLine("AP: " + String(AP_SSID));
        displayLine("Pass: " + String(AP_PASSWORD));
        displayLine(WiFi.softAPIP().toString());
    }

    // Настройка веб-сервера
    server.on("/", handleRoot);
    server.on("/save", HTTP_POST, handleSave);
    server.begin();
}

void loop() {
    // Обработка запросов веб-сервера
    server.handleClient();

    // Обработка запросов MQTT брокера
    broker.loop();
}
