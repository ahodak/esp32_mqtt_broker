#include "WebSrv.hpp"
#include "wifi_config.hpp"
#include "common_config.hpp"

#include "views/styles.tpl"
#include "views/scripts.tpl"
#include "views/index_page.tpl"
#include "views/publish_page.tpl"
#include "views/subscribe_page.tpl"
#include "views/reboot_page.tpl"
#include "views/setup_page.tpl"
#include "views/not_found_page.tpl"
#include "views/alert_page.tpl"

// Инициализация веб-сервера
#ifdef ESP32
void WebSrv::init(WebServer* server,
#elif ESP8266
void WebSrv::init(ESP8266WebServer* server,
#endif
                WiFiConfig* wifiConfig,
                CommonConfig* commonConfig,
                bool isAP,
                TRebootFunction rebootFunction,
                TPublishMessageFunction publishMessageFunction,
                TSubscribeFunction subscribeFunction,
                TUnsubscribeFunction unsubscribeFunction) {

    server->on("/", HTTP_GET, [this](){ handleShowIndex(); });
    server->on("/styles.css", HTTP_GET, [this](){ handleStyles(); });
    server->on("/scripts.js", HTTP_GET, [this](){ handleScripts(); });
    server->on("/publish", HTTP_GET, [this](){ handleShowPublish(""); });
    server->on("/publish", HTTP_POST, [this](){ handlePublish(); });
    server->on("/subscribe", HTTP_GET, [this](){ handleShowSubscribe(""); });
    server->on("/subscribe", HTTP_POST, [this](){ handleSubscribe(); });
    server->on("/unsubscribe", HTTP_POST, [this](){ handleUnsubscribe(); });
    server->on("/setup", HTTP_GET, [this](){ handleShowSettings(); });
    server->on("/setup", HTTP_POST, [this](){ handleSaveSettings(); });
    server->on("/reboot", HTTP_GET, [this](){ handleReboot(); });
    server->onNotFound([this](){ handleNotFound(); });
    server->begin();

    this->_server = server;
    this->_wifiConfig = wifiConfig;
    this->_commonConfig = commonConfig;
    this->_isAP = isAP;
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

// Обработчик главной страницы
void WebSrv::handleShowIndex() {
    String html = INDEX_PAGE;
    replaceCommonTemplateVars(html);
    html.replace("%ssid%", this->_wifiConfig->ssid());
    html.replace("%local_ip%", WiFi.localIP().toString());
    html.replace("%ip%", WiFi.localIP().toString());
    html.replace("%mqtt_user%", this->_commonConfig->mqttUser());
    html.replace("%mqtt_password%", this->_commonConfig->mqttPassword());

    processConditionalBlock(html, "ap_mode", _isAP);
    processConditionalBlock(html, "sta_mode", !_isAP);

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

    // Устанавливаем значения по умолчанию если параметры отсутствуют
    int qos = this->_server->hasArg("qos") ? this->_server->arg("qos").toInt() : 0;
    bool retain = this->_server->hasArg("retain") ? (this->_server->arg("retain") == "on") : false;
    int message_id = this->_server->hasArg("message_id") ? this->_server->arg("message_id").toInt() : 0;

    // Валидация QoS
    if (qos < 0 || qos > 2) {
        this->handleShowPublish("Ошибка: недопустимое значение QoS");
        return;
    }

    this->publishMessageFunction(topic, payload, qos, retain, message_id);
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
    html.replace("%ssid%", this->_wifiConfig->ssid());
    html.replace("%password%", this->_wifiConfig->password());
    html.replace("%mqtt_user%", this->_commonConfig->mqttUser());
    html.replace("%mqtt_password%", this->_commonConfig->mqttPassword());
    
    this->_server->send(200, "text/html", html);
}

// Обработчик сохранения настроек
void WebSrv::handleSaveSettings() {
    if (this->_server->hasArg("ssid") && this->_server->hasArg("password")) {
        this->_wifiConfig->ssid(this->_server->arg("ssid"));
        this->_wifiConfig->password(this->_server->arg("password"));
    }
        
    if (this->_server->hasArg("mqtt_user") && this->_server->hasArg("mqtt_password")) {
        this->_commonConfig->mqttUser(this->_server->arg("mqtt_user"));
        this->_commonConfig->mqttPassword(this->_server->arg("mqtt_password"));
    }

    if (this->_server->hasArg("reboot_delay")) {
        this->_commonConfig->rebootDelay(this->_server->arg("reboot_delay").toInt());
    }
    this->_wifiConfig->save();

    String html = ALERT_PAGE;
    html.replace("%message%", "Настройки сохранены. Перезагрузите устройство чтобы изменения вступили в силу.");
    this->_server->send(200, "text/html", html);
}

// Обработчик страницы перезагрузки
void WebSrv::handleReboot() {
    String html = REBOOT_PAGE;
    replaceCommonTemplateVars(html);

    this->_server->send(200, "text/html", html);

    this->rebootFunction();
}

// Обработчик страницы ошибки 404
void WebSrv::handleNotFound() {
    String html = NOT_FOUND_PAGE;
    replaceCommonTemplateVars(html);

    this->_server->send(404, "text/html", html);
}

// Рендер строки параметра
String WebSrv::renderParameterRow(String paramName,
                                 String paramId,
                                 String paramValue,
                                 int maxLength,
                                 bool isReadonly,
                                 bool isPassword)
{
    return String("<div class='input-group'>"
                    "<span class='input-group-addon'>" + paramName + ":</span>"
                    "<input maxlength='" + maxLength + "'"
                    " type='" + (isPassword ? "password" : "text") + "'"
                    " id='" + paramId + "'"
                    " class='form-control'"
                    " " + (isReadonly ? "readonly='readonly'" : "") + " value='" + (isPassword ? "" : paramValue) + "' />"
                "</div>");
}

// Отобразить блок в зависимости от выполнения условия
void WebSrv::processConditionalBlock(String& html, const String& conditionName, bool keepBlock) {
    if (keepBlock) {
        // Удаляем только маркеры
        html.replace("%if_" + conditionName + "%", "");
        html.replace("%endif_" + conditionName + "%", "");
    } else {
        // Удаляем весь блок с содержимым
        int startPos = html.indexOf("%if_" + conditionName + "%");
        int endPos = html.indexOf("%endif_" + conditionName + "%") + String("%endif_" + conditionName + "%").length();
        html = html.substring(0, startPos) + html.substring(endPos);
    }
}

// Подставить в шаблон значения переменных
void WebSrv::replaceCommonTemplateVars(String& html) {
    html.replace("%delay%", String(this->_commonConfig->rebootDelay()));
}
