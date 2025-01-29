#include "WebSrv.hpp"

// Инициализация веб-сервера
void WebSrv::init(WebServer* server,
                NetworkParams* networkParams,
                TOtaFunction otaFunction,
                TGetAppVersionFunction getAppVersionFunction,
                TRebootFunction rebootFunction,
                TPublishMessageFunction publishMessageFunction,
                TSubscribeFunction subscribeFunction,
                TUnsubscribeFunction unsubscribeFunction) {

    server->on("/", HTTP_GET, [this](){ handleShowIndex(); });
    server->on("/bootstrap.min.css", HTTP_GET, [this](){ handleBootstrapCss(); });
    server->on("/bootstrap.min.js", HTTP_GET, [this](){ handleBootstrapJs(); });
    server->on("/styles.css", HTTP_GET, [this](){ handleStyles(); });
    server->on("/scripts.js", HTTP_GET, [this](){ handleScripts(); });
    server->on("/publish", HTTP_GET, [this](){ handleShowPublish(""); });
    server->on("/publish", HTTP_POST, [this](){ handlePublish(); });
    server->on("/subscribe", HTTP_GET, [this](){ handleShowSubscribe(""); });
    server->on("/subscribe", HTTP_POST, [this](){ handleSubscribe(); });
    server->on("/unsubscribe", HTTP_POST, [this](){ handleUnsubscribe(); });
    server->on("/setup", HTTP_GET, [this](){ handleShowSettings(); });
    server->on("/setup", HTTP_POST, [this](){ handleSaveSettings(); });
    server->on("/firmware", HTTP_GET, [this](){ handleShowFirmwareUpdate(); });
    server->on("/firmware", HTTP_POST, [this](){ handleFirmwareUpgrade(); });
    server->on("/reboot", HTTP_GET, [this](){ handleReboot(); });
    server->on("/module_params", HTTP_GET, [this](){ handleGetModuleParams(); });
    server->onNotFound([this](){ handleNotFound(); });
    server->begin();

    this->_server = server;
    this->_networkParams = networkParams;
    this->otaFunction = otaFunction;
    this->getAppVersionFunction = getAppVersionFunction;
    this->rebootFunction = rebootFunction;
    this->publishMessageFunction = publishMessageFunction;
    this->subscribeFunction = subscribeFunction;
    this->unsubscribeFunction = unsubscribeFunction;
}

// Основной цикл веб-сервера
void WebSrv::loop() {
    this->_server->handleClient();
}

// Обработчик загрузки стилей
void WebSrv::handleStyles() {
    this->_server->send(200, "text/css", STYLES);
}

// Обработчик загрузки скриптов
void WebSrv::handleScripts() {
    this->_server->send(200, "text/javascript", SCRIPTS);
}

// Обработчик загрузки bootstrap.min.css
void WebSrv::handleBootstrapCss() {
    this->_server->sendHeader("Cache-Control", "max-age=86400");
    this->sendContentGzip(binary_lib_web_bootstrap_min_css_gz_start, binary_lib_web_bootstrap_min_css_gz_end - binary_lib_web_bootstrap_min_css_gz_start, "text/css");
}

// Обработчик загрузки bootstrap.min.js
void WebSrv::handleBootstrapJs() {
    this->_server->sendHeader("Cache-Control", "max-age=86400");
    this->sendContentGzip(binary_lib_web_bootstrap_min_js_gz_start, binary_lib_web_bootstrap_min_js_gz_end - binary_lib_web_bootstrap_min_js_gz_start, "text/javascript");
}

// Обработчик главной страницы
void WebSrv::handleShowIndex() {
    String html = INDEX_PAGE;
    replaceCommonTemplateVars(html);

    html.replace("%module_chip_model%", ESP.getChipModel());
    html.replace("%module_chip_revision%", " рев. " + String(ESP.getChipRevision()));
    html.replace("%module_chip_cores%", String(ESP.getChipCores()));
    html.replace("%module_heap_size%", format_memory(ESP.getHeapSize()));
    html.replace("%module_heap_max%", format_memory(ESP.getMaxAllocHeap()));

    html.replace("%module_chip_speed%", String(ESP.getCpuFreqMHz()));
    html.replace("%module_chip_flash_size%", format_memory(ESP.getFlashChipSize(), 0));

    html.replace("%module_heap_free%", format_memory(ESP.getFreeHeap()));
    
    html.replace("%uptime%", format_duration(millis() / 1000));

    Preferences preferences;
    preferences.begin("mqtt-broker", false);
    html.replace("%mqtt_user%", preferences.getString("mqttUser", DEFAULT_MQTT_USERNAME));
    html.replace("%mqtt_password%", preferences.getString("mqttPassword", DEFAULT_MQTT_PASSWORD));
    preferences.end();
    html.replace("%mac_address%", _networkParams->mac);
    html.replace("%ip%", _networkParams->ip);
    html.replace("%hostname%", _networkParams->hostname);
    html.replace("%ssid%", _networkParams->ssid);
    html.replace("%rssi%", _networkParams->rssi);

    processConditionalBlock(html, "wifi_mode", _networkParams->isWiFi);
    
    if (_networkParams->isWiFi) {
        html.replace("%network%", "WiFi");
        processConditionalBlock(html, "ap_mode", _networkParams->isAP);
        processConditionalBlock(html, "sta_mode", !_networkParams->isAP);
    }
    else
        html.replace("%network%", "Ethernet");

    this->_server->send(200, "text/html", html);
}

// Обработчик страницы публикации сообщения
void WebSrv::handleShowPublish(String message) {
    String html = PUBLISH_PAGE;
    replaceCommonTemplateVars(html);
    showMessage(html, message);
    this->_server->send(200, "text/html", html);
}

// Обработчик публикации сообщения
void WebSrv::handlePublish() {
    // Проверяем наличие обязательных параметров
    if (!this->_server->hasArg("topic") || !this->_server->hasArg("payload")) {
        this->handleShowPublish("Ошибка: отсутствуют обязательные параметры");
        return;
    }

    String topic = this->_server->arg("topic");
    String payload = this->_server->arg("payload");
    
    // Проверяем что топик не пустой
    if (topic.length() == 0) {
        this->handleShowPublish("Ошибка: топик не может быть пустым");
        return;
    }

    this->publishMessageFunction(topic, payload);
    this->handleShowPublish("Опубликовали");
}

// Обработчик страницы подписки
void WebSrv::handleShowSubscribe(String message) {
    String html = SUBSCRIBE_PAGE;
    replaceCommonTemplateVars(html);
    showMessage(html, message);
    this->_server->send(200, "text/html", html);
}

void WebSrv::showMessage(String& html, String message) {
    processConditionalBlock(html, "message", !message.isEmpty());
    if (!message.isEmpty()) {
        html.replace("%message%", message);
    }
}

// Обработчик подписки
void WebSrv::handleSubscribe() {
    if (!this->_server->hasArg("topic")) {
        this->handleShowSubscribe("Ошибка: отсутствует топик для подписки");
        return;
    }

    String topic = this->_server->arg("topic");
    if (topic.length() == 0) {
        this->handleShowSubscribe("Ошибка: топик не может быть пустым");
        return;
    }

    this->subscribeFunction(topic);
    this->handleShowSubscribe("Подписались");
}

// Обработчик отписки
void WebSrv::handleUnsubscribe() {
    if (!this->_server->hasArg("topic")) {
        this->handleShowSubscribe("Ошибка: отсутствует топик для отписки");
        return;
    }

    String topic = this->_server->arg("topic");
    if (topic.length() == 0) {
        this->handleShowSubscribe("Ошибка: топик не может быть пустым");
        return;
    }

    this->unsubscribeFunction(topic);
    this->handleShowSubscribe("Отписались");
}

// Обработчик страницы настройки
void WebSrv::handleShowSettings() {
    String html = SETUP_PAGE;
    replaceCommonTemplateVars(html);
    Preferences preferences;
    preferences.begin("mqtt-broker", false);
    html.replace("%ssid%", preferences.getString("ssid", DEFAULT_SSID));
    html.replace("%password%", preferences.getString("password", DEFAULT_PASSWORD));
    html.replace("%hostname%", preferences.getString("hostname", DEFAULT_HOSTNAME));
    html.replace("%mqtt_user%", preferences.getString("mqttUser", DEFAULT_MQTT_USERNAME));
    html.replace("%mqtt_password%", preferences.getString("mqttPassword", DEFAULT_MQTT_PASSWORD));
    html.replace("%reboot_delay%", String(preferences.getUInt("rebootDelay", DEFAULT_REBOOT_DELAY)));
    preferences.end();
    this->_server->send(200, "text/html", html);
}

// Обработчик сохранения настроек
void WebSrv::handleSaveSettings() {
    ESP_LOGI(_logTAG, "Saving settings...");
    Preferences preferences;
    preferences.begin("mqtt-broker", false);
    if (this->_server->hasArg("ssid") && this->_server->hasArg("password") && this->_server->hasArg("hostname")) {
        preferences.putString("ssid", this->_server->arg("ssid"));
        preferences.putString("password", this->_server->arg("password"));
        preferences.putString("hostname", this->_server->arg("hostname"));
    }

    if (this->_server->hasArg("mqtt_user") && this->_server->hasArg("mqtt_password")) {
        preferences.putString("mqttUser", this->_server->arg("mqtt_user"));
        preferences.putString("mqttPassword", this->_server->arg("mqtt_password"));
    }

    if (this->_server->hasArg("reboot_delay")) {
        preferences.putUInt("rebootDelay", this->_server->arg("reboot_delay").toInt());
    }

    ESP_LOGI(_logTAG, "Settings saved");
    preferences.end();
    handleShowSettings();
}

// Обработчик страницы обновления прошивки
void WebSrv::handleShowFirmwareUpdate() {
    String new_version = this->getAppVersionFunction();
    String html = OTA_PAGE;
    replaceCommonTemplateVars(html);
    html.replace("%version%", String(APP_VERSION, 1U));
    html.replace("%new_version%", new_version);

    if (new_version == "N/A") {
        processConditionalBlock(html, "has_new_version", false);
    }
    else {
        processConditionalBlock(html, "has_new_version", new_version.toFloat() > String(APP_VERSION).toFloat());
    }

    this->_server->send(200, "text/html", html);
}

// Обработчик обновления прошивки
void WebSrv::handleFirmwareUpgrade() {
    String html = ALERT_PAGE;
    replaceCommonTemplateVars(html);

    html.replace("%message%", "Прошивка обновлена");
    // <script>setTimeout(function() { window.location.href = '/'; }, 5000);</script>
    this->_server->send(200, "text/html", html);

    ESP_LOGI(_logTAG, "Starting OTA update");

    if (this->otaFunction) {
        this->otaFunction();
    } else {
        ESP_LOGE(_logTAG, "OTA function is not set");
    }
}

// Обработчик страницы перезагрузки
void WebSrv::handleReboot() {
    String html = REBOOT_PAGE;
    replaceCommonTemplateVars(html);

    this->_server->send(200, "text/html", html);

    if (this->rebootFunction) { 
        this->rebootFunction();
    } else {
        ESP_LOGE(_logTAG, "Reboot function is not set");
    }
}

void WebSrv::handleGetModuleParams() {
    String rssi = (this->_networkParams->isWiFi) ? String(WiFi.RSSI()) : "N/A";

    String params = rssi + "," +
                    format_duration(millis() / 1000) + "," +
                    format_memory(ESP.getFreeHeap());
    this->_server->send(200, "text/plain", params);
}

// Обработчик страницы ошибки 404
void WebSrv::handleNotFound() {
    String html = NOT_FOUND_PAGE;
    replaceCommonTemplateVars(html);

    this->_server->send(404, "text/html", html);
}

// Отобразить блок в зависимости от выполнения условия
void WebSrv::processConditionalBlock(String& html, const String& conditionName, bool keepBlock) {
    if (conditionName.isEmpty()) {
        ESP_LOGE(_logTAG, "Condition name is empty");
        return;
    }

    if (keepBlock) {
        // Удаляем только маркеры
        html.replace("%if_" + conditionName + "%", "");
        html.replace("%endif_" + conditionName + "%", "");
    } else {
        // Удаляем весь блок с содержимым
        int startPos = html.indexOf("%if_" + conditionName + "%");
        int endPos = html.indexOf("%endif_" + conditionName + "%") + String("%endif_" + conditionName + "%").length();
        if (startPos != -1 && endPos != -1) {
            html.replace(html.substring(startPos, endPos), "");
        }
    }
}

// Подставить в шаблон значения переменных
void WebSrv::replaceCommonTemplateVars(String& html) {
    Preferences preferences;
    preferences.begin("mqtt-broker", false);
    int rebootDelay = preferences.getUInt("rebootDelay", DEFAULT_REBOOT_DELAY);
    preferences.end();
    html.replace("%delay%", String(rebootDelay));
    html.replace("%version%", String(APP_VERSION, 1U));
}

void WebSrv::sendContentGzip(const unsigned char *content, size_t length, const char *mime_type)
{
    this->_server->sendHeader("Content-encoding", "gzip");
    this->_server->setContentLength(length);
    this->_server->send(200, mime_type, "");
    this->_server->sendContent(reinterpret_cast<const char *>(content), length);
}
