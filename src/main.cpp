#include "main.hpp"

// Тег для логирования
static const char* _logTAG = "Main";

// Переменные состояния
bool isConnected = false;
bool isRebooting = false;
bool isOtaUpdating = false;
int currentRebootCycle = 0;
unsigned long previousMillis = 0;
int currentSensorCycle = 0;

// Переменные конфигурации
int rebootDelay;

Display display;

NetworkParams networkParams;

WebServer server;
WebSrv webServer;
#ifdef USE_SENSORS
Sensors* temperatureSensor;
String temperatureTopic;
#endif

MQTT broker(MQTT_BROKER_PORT, "WiFiServer");

// Публикация сообщения
void publishMessage(String topic, String payload)
{
    ESP_LOGV(_logTAG, "IN: publishMessage(topic=%s, payload=%s)", topic.c_str(), payload.c_str());
    ESP_LOGI(_logTAG, "Publishing message:\ntopic=%s\npayload=%s", topic.c_str(), payload.c_str());

	broker.publish(topic, payload, 0, false, 0);

    display.clear();
    display.line("Message published:");
    display.line("topic=" + topic);
    display.line("payload=" + payload);
    ESP_LOGV(_logTAG, "OUT: publishMessage()");
}

// Подписка на топик
void subscribeToTopic(String topic_filter)
{
    ESP_LOGV(_logTAG, "IN: subscribeToTopic(topic_filter=%s)", topic_filter.c_str());
    broker.subscribe(topic_filter,
        [](String topic, String payload) {
            display.clear();
            display.line("Received message:");
            display.line("topic=" + topic);
            display.line("payload=" + payload);
        }
    );

    display.clear();
    display.line("Subscribed to topic: " + topic_filter);

    ESP_LOGV(_logTAG, "OUT: subscribeToTopic()");
}

// Отписка от топика
void unsubscribeFromTopic(String topic_filter)
{
    ESP_LOGV(_logTAG, "IN: unsubscribeFromTopic(topic_filter=%s)", topic_filter.c_str());
    broker.unsubscribe(topic_filter);

    display.clear();
    display.line("Unsubscribed from topic: " + topic_filter);

    ESP_LOGV(_logTAG, "OUT: unsubscribeFromTopic()");
}

// Обработка таймера при перезагрузке
void handleTimerWhileRebooting()
{
    ESP_LOGV(_logTAG, "IN: handleTimerWhileRebooting()");
	int periods = rebootDelay;
    ESP_LOGD(_logTAG, "Reboot delay: %d", periods);
	if (currentRebootCycle == periods)
	{
		ESP.restart();
		return;
	}

    display.clear();
	display.line("Reboot ESP in " + String(periods - currentRebootCycle) + " sec.");
    ESP_LOGV(_logTAG, "OUT: handleTimerWhileRebooting()");
}

// Перезагрузка ESP
void rebootESP()
{
    ESP_LOGV(_logTAG, "IN: rebootESP()");
	handleTimerWhileRebooting();

	// Игнорировать все сообщения для отображения во время перезагрузки
	isRebooting = true;

    ESP_LOGV(_logTAG, "OUT: rebootESP()");
}

// Обработка обновления по воздуху
void otaStart()
{
    isOtaUpdating = true;
    ESP_LOGV(_logTAG, "IN: otaStart()");

    ESP_LOGI(_logTAG, "Stopping MQTT broker");
    broker.stop();
    display.clear();
    display.line("Firmware update started");
    ESP_LOGD(_logTAG, "Source URL: %s", FIRMWARE_URL);

    WiFiClientSecure client;
    // Сертификат должен содержать данные центра сертификации который выдает сертификат для github.com
    // т.е. DigiCert Global Root CA
    client.setCACert((char *)binary_include_github_ca_pem_start);
    //client.setInsecure();

    uint8_t tryUpdate = 0;
    t_httpUpdate_return ret = HTTP_UPDATE_OK;
    do {
        tryUpdate++;
        ESP_LOGI(_logTAG, "Start of firmware upgrade from \"%s\", attempt %d", FIRMWARE_URL, tryUpdate);
      
        t_httpUpdate_return ret = httpUpdate.update(client, GITHUB_HOST, 443, FIRMWARE_URL);
        
        switch (ret) {
            case HTTP_UPDATE_FAILED:
                display.error("Firmware update failed");
                ESP_LOGE(_logTAG, "Error (%d): %s", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
                break;
            case HTTP_UPDATE_NO_UPDATES:
                ESP_LOGI(_logTAG, "Firmware updates are not available.");
                break;
            case HTTP_UPDATE_OK:
                display.line("Firmware updated");
                rebootESP();
                break;
        }
    } while ((ret != HTTP_UPDATE_OK) && (tryUpdate < OTA_ATTEMPTS));

    ESP_LOGV(_logTAG, "OUT: otaStart()");
    isOtaUpdating = false;
}

// Получение версии приложения
String getAppVersion() {
    ESP_LOGV(_logTAG, "IN: getAppVersion()");

    WiFiClientSecure client;
    // Сертификат должен содержать данные центра сертификации который выдает сертификат для github.com
    // т.е. DigiCert Global Root CA
    ESP_LOGD(_logTAG, "CA cert:\n%s", String((char *)binary_include_github_ca_pem_start).c_str());
    client.setCACert((char *)binary_include_github_ca_pem_start);
    // client.setInsecure();
    ESP_LOGI(_logTAG, "Retrieving latest version from https://%s%s...", GITHUB_HOST, VERSION_URL);
    client.connect(GITHUB_HOST, 443);
    client.print("GET " + String(VERSION_URL) + " HTTP/1.1\r\n");
    ESP_LOGV(_logTAG, "GET %s HTTP/1.1\r\n", VERSION_URL);
    client.print("Host: " + String(GITHUB_HOST) + "\r\n");
    ESP_LOGV(_logTAG, "Host: %s", GITHUB_HOST);
    client.print("Connection: close\r\n");
    ESP_LOGV(_logTAG, "Connection: close\r\n");
    client.print("\r\n");
    ESP_LOGV(_logTAG, "\r\n");
    String response = client.readString();
    ESP_LOGD(_logTAG, "Response: %s", response.c_str());
    if (response.startsWith("HTTP/1.1 200 OK")) {
        int startIndex = response.indexOf("APP_VERSION") + 20;
        int endIndex = response.lastIndexOf("\"");
        response = response.substring(startIndex, endIndex);
        ESP_LOGI(_logTAG, "Latest firmware version: %s", response.c_str());
    } else {
        response = "N/A";
        ESP_LOGE(_logTAG, "Failed to retrieve latest version.");
    }

    client.stop();

    ESP_LOGV(_logTAG, "OUT: getAppVersion()");
    return response;
}

// Инициализация
void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(2500);

    ESP_LOGV(_logTAG, "IN: setup()");
    isConnected = false;
    isRebooting = false;
    currentSensorCycle = 0;
    currentRebootCycle = 0;
    previousMillis = 0;

    Serial.println("Starting...");
    Serial.println("Firmware version: " + String(APP_VERSION));

    // Инициализация дисплея
    display.init();

    // Загрузка настроек
    display.line("Initializing config...");
    Preferences preferences;
    preferences.begin("mqtt-broker", false);
    String ssid = preferences.getString("ssid", DEFAULT_SSID);
    String password = preferences.getString("password", DEFAULT_PASSWORD);
    String hostname = preferences.getString("hostname", DEFAULT_HOSTNAME);
    String mqttUser = preferences.getString("mqttUser", DEFAULT_MQTT_USERNAME);
    String mqttPassword = preferences.getString("mqttPassword", DEFAULT_MQTT_PASSWORD);
    rebootDelay = preferences.getUInt("rebootDelay", DEFAULT_REBOOT_DELAY);
    #ifdef USE_SENSORS
    temperatureTopic = preferences.getString("temperatureTopic", DEFAULT_TEMPERATURE_TOPIC);
    float temperature0 = preferences.getFloat("temperature0", DEFAULT_TEMPERATURE_0);
    float temperature100 = preferences.getFloat("temperature100", DEFAULT_TEMPERATURE_100);
    #endif
    preferences.end();
    ESP_LOGD(_logTAG, "Config initialized");

    display.clear();

    #ifdef ARDUINO_WT32_ETH01
    display.line("Connecting to Ethernet...");

    ESP_LOGD(_logTAG, "Begin initializing Ethernet");
    pinMode(ETH_POWER_PIN_ALTERNATIVE, OUTPUT);
    digitalWrite(ETH_POWER_PIN_ALTERNATIVE, HIGH);

    ETH.begin(ETH_ADDR, ETH_POWER_PIN_ALTERNATIVE, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);
    ETH.setHostname(hostname.c_str());
    ESP_LOGD(_logTAG, "Hostname set to %s", hostname.c_str());
    ESP_LOGD(_logTAG, "Ethernet initialized");

    int attempts = 0;
    while (!((uint32_t)ETH.localIP()) && attempts < CONNECTION_ATTEMPTS) {
        delay(CONNECTION_ATTEMPTS_DELAY);
        attempts++;
        display.progress(2);
    }
    ESP_LOGD(_logTAG, "Ethernet connection attempt finished");

    display.clear();

    ESP_LOGD(_logTAG, "Checking Ethernet connection");
    if (((uint32_t)ETH.localIP())) {
        display.line("Ethernet connected");
        display.line("- " + ETH.localIP().toString());

        isConnected = true;
        networkParams.isWiFi = false;
        networkParams.mac = ETH.macAddress();
        networkParams.ip = ETH.localIP().toString();
        networkParams.hostname = String(ETH.getHostname());
        networkParams.ssid = "N/A";
        networkParams.rssi = "N/A";
        networkParams.isAP = false;
    } else {
        display.error("Ethernet connection failed");
    }
    ESP_LOGD(_logTAG, "Ethernet connection check finished");
    #endif

    ESP_LOGD(_logTAG, "isConnected: %d", isConnected);
    if (!isConnected) {
        // Подключение к WiFi
        ESP_LOGD(_logTAG, "Hostname: %s", hostname.c_str());
        WiFi.hostname(hostname.c_str());
        ESP_LOGD(_logTAG, "SSID: %s", ssid.c_str());
        WiFi.begin(ssid.c_str(), password.c_str());
        display.line("Connecting to WiFi...");
        display.line("SSID: " + ssid);

        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < CONNECTION_ATTEMPTS) {
            delay(CONNECTION_ATTEMPTS_DELAY);
            attempts++;
            display.progress(2);
        }

        display.clear();

        ESP_LOGD(_logTAG, "WiFi status: %d", WiFi.status());
        if (WiFi.status() == WL_CONNECTED) {
            display.line("WiFi connected");
            display.line("- IP: " + WiFi.localIP().toString());

            isConnected = true;
            networkParams.isAP = false;
            networkParams.ssid = WiFi.SSID();
            networkParams.ip = WiFi.localIP().toString();
        } else {
            display.error("WiFi connection failed");

            // Создаем точку доступа для настройки
            display.line("Creating Access Point...");
            WiFi.softAP(AP_SSID, AP_PASSWORD);
            display.line("- SSID: " + String(AP_SSID));
            display.line("- Pass: " + String(AP_PASSWORD));
            display.line("- IP  : " + WiFi.softAPIP().toString());

            delay(2500);

            isConnected = false;
            networkParams.isAP = true;
            networkParams.ssid = String(AP_SSID);
            networkParams.ip = WiFi.softAPIP().toString();
        }

        networkParams.isWiFi = true;
        networkParams.mac = WiFi.macAddress();
        networkParams.hostname = WiFi.getHostname();
        networkParams.rssi = "N/A";
    }

    // Инициализация веб-сервера
    ESP_LOGD(_logTAG, "Initializing WebServer");
    webServer.init(&server,
        &networkParams,
        &otaStart,
        &getAppVersion,
        &rebootESP,
        &publishMessage,
        &subscribeToTopic,
        &unsubscribeFromTopic);
    display.line("WebServer Started");

    if (isConnected) {
        // Инициализация MQTT брокера
        ESP_LOGD(_logTAG, "Initializing MQTT broker");
        broker.init(mqttUser.c_str(), mqttPassword.c_str());
        display.line("MQTT Broker Started");
        display.line("- Username: " + mqttUser);
        display.line("- Password: " + mqttPassword);
    }

    // Инициализация датчиков
    #ifdef USE_SENSORS
    ESP_LOGD(_logTAG, "Initializing sensors");
    temperatureSensor = new Sensors();
    temperatureSensor->setup();
    temperatureSensor->calibrate(temperature0, temperature100);
    #endif

    ESP_LOGV(_logTAG, "OUT: setup()");
}

// Основной цикл
void loop() {
    if (isOtaUpdating) {
        return;
    }

    // Обработка запросов веб-сервера
    webServer.loop();

    if (isConnected) {
        // Обработка запросов MQTT брокера
        broker.loop();
    }

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
            currentSensorCycle++;

            if (currentSensorCycle % DATA_DELAY == 0)
			{
                #ifdef USE_SENSORS
                if (isConnected) {
                    float temperature = temperatureSensor->getTemperature();
                    if (temperature != DEVICE_DISCONNECTED_C) {
                        publishMessage(temperatureTopic, "{\"connected\":true,\"value\":" + String(temperature, 1U) + "}");
                    }
                }
                #endif
            }
		}
    }
}
