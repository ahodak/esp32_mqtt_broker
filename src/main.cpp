#include "main.h"

#include "wifi_config.h"
#include "common_config.h"
#include "display.h"
#include "WebSrv.h"

// Настройки WiFi по умолчанию
String ssid = DEFAULT_SSID;
String password = DEFAULT_PASSWORD;
bool isAP = false;

int currentRebootCycle = 0;
bool isRebooting = false;

// Создаем объекты
WiFiServer tcp_server(MQTT_BROKER_PORT);
PicoMQTT::Server broker(tcp_server);

WebServer server;
WebSrv webServer;

WiFiConfig wifiConfig;
CommonConfig commonConfig;

void handleTimerWhileRebooting()
{
	int periods = commonConfig.rebootDelay();
	if (currentRebootCycle == periods)
	{
		ESP.restart();
		return;
	}

    clearDisplay();
	displayLine("Reboot ESP in " + String(periods - currentRebootCycle) + " sec.");
}

void rebootESP()
{
	handleTimerWhileRebooting();

	// Игнорировать все сообщения для отображения во время перезагрузки
	isRebooting = true;
}

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

    // Инициализация дисплея
    initDisplay();
    displayLine("Starting...");

    // Инициализация SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS initialization failed");
    }
    Serial.println("SPIFFS initialized");

    // Загрузка настроек WiFi
    wifiConfig = WiFiConfig();
    if (!wifiConfig.load()) {
        displayError("Error loading WiFi config");
        return; 
    }

    // Загрузка общих настроек
    commonConfig = CommonConfig();
    if (!commonConfig.load()) {
        displayError("Error loading Common config");
        return;
    }

    ssid = wifiConfig.ssid();
    password = wifiConfig.password();

    // Подключение к WiFi
    WiFi.begin(ssid.c_str(), password.c_str());
    displayLine("Connecting to WiFi. SSID: " + ssid);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < WIFI_CONNECTION_ATTEMPTS) {
        delay(WIFI_ATTEMPTS_DELAY);
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

    // Инициализация веб-сервера
    webServer.init(&server, &wifiConfig, &commonConfig, isAP, &rebootESP);
}

void loop() {
    // Обработка запросов веб-сервера
    webServer.loop();

    // Обработка запросов MQTT брокера
    broker.loop();

    if (isRebooting)
    {
        currentRebootCycle++;
        handleTimerWhileRebooting();
    }
}