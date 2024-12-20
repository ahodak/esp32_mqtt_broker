#include "main.h"

#include "wifi_config.h"
#include "common_config.h"
#include "display.h"
#include "WebSrv.h"

// Настройки WiFi по умолчанию
String ssid = DEFAULT_SSID;
String password = DEFAULT_PASSWORD;
bool isAP = false;

int rebootDelay = 0;
int currentRebootCycle = 0;
bool isRebooting = false;
unsigned long previousMillis = 0;
int dataDelay = 0;
int currentSensorCycle = 0;

// Создаем объекты
WiFiServer tcp_server(MQTT_BROKER_PORT);
PicoMQTT::Server broker(tcp_server);

WebServer server;
WebSrv webServer;

WiFiConfig wifiConfig;
CommonConfig commonConfig;

void publishMessage(String topic, String payload, bool qos = 0, bool retain = false, int message_id = 0)
{
    Serial.println("Publishing message:\ntopic=" + topic + "\npayload=" + payload + "\nqos=" + String(qos) + "\nretain=" + String(retain) + "\nmessage_id=" + String(message_id));

	broker.publish(topic, payload, qos, retain, message_id);

    clearDisplay();
    displayLine("Message published:");
    displayLine("topic=" + topic);
    displayLine("qos=" + String(qos));
    displayLine("retain=" + String(retain));
    displayLine("message_id=" + String(message_id));
}

void subscribeToTopic(String topic_filter)
{
    broker.subscribe(topic_filter,
        [](String topic, String payload) {
            clearDisplay();
            displayLine("Received message:");
            displayLine("topic=" + topic);
            displayLine("payload=" + payload);
        }
    );

    clearDisplay();
    displayLine("Subscribed to topic: " + topic_filter);
}

void unsubscribeFromTopic(String topic_filter)
{
    broker.unsubscribe(topic_filter);

    clearDisplay();
    displayLine("Unsubscribed from topic: " + topic_filter);
}

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

    currentSensorCycle = 0;
    currentRebootCycle = 0;
    previousMillis = 0;
    isRebooting = false;

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
    ssid = wifiConfig.ssid();
    password = wifiConfig.password();

    // Загрузка общих настроек
    commonConfig = CommonConfig();
    if (!commonConfig.load()) {
        displayError("Error loading Common config");
        return;
    }
    rebootDelay = commonConfig.rebootDelay();
    dataDelay = commonConfig.dataDelay();

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
    webServer.init(&server,
        &wifiConfig,
        &commonConfig,
        isAP,
        &rebootESP,
        &publishMessage,
        &subscribeToTopic,
        &unsubscribeFromTopic);
}

void loop() {
    // Обработка запросов веб-сервера
    webServer.loop();

    // Обработка запросов MQTT брокера
    broker.loop();

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= ONE_SECOND)
    {
        previousMillis = currentMillis;

        if (isRebooting)
        {
            currentRebootCycle++;
            handleTimerWhileRebooting();
        }
		else
		{
            if (currentSensorCycle % dataDelay == 0)
			{
				//Serial.println("\r\nGetting sensors data...");
				//requestSensorValues();
				//renderSensorValues();
				//if (WiFi.status() == WL_CONNECTED)
				//{
				//	sendSensorsData();
				//}
			}
		}
    }
}
