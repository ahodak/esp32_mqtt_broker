#include "WebSrv.h"
#include "wifi_config.h"
#include "common_config.h"

#include "views/styles.h"
#include "views/scripts.h"
#include "views/index_page.h"
#include "views/reboot_page.h"
#include "views/setup_page.h"
#include "views/not_found_page.h"
#include "views/alert_page.h"

// Инициализация веб-сервера
void WebSrv::init(WebServer* server, WiFiConfig* wifiConfig, CommonConfig* commonConfig, bool isAP, TRebootFunction rebootFunction) {

    server->on("/", HTTP_GET, [this](){ handleRoot(); });
    server->on("/styles.css", HTTP_GET, [this](){ handleStyles(); });
    server->on("/scripts.js", HTTP_GET, [this](){ handleScripts(); });
    server->on("/setup", HTTP_GET, [this](){ handleSetup(); });
    server->on("/setup", HTTP_POST, [this](){ handleSaveWiFiSetup(); });
    server->on("/reboot", HTTP_GET, [this](){ handleReboot(); });
    server->onNotFound([this](){ handleNotFound(); });
    server->begin();

    this->_server = server;
    this->_wifiConfig = wifiConfig;
    this->_commonConfig = commonConfig;
    this->_isAP = isAP;
    this->rebootFunction = rebootFunction;
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
void WebSrv::handleRoot() {
    String html = INDEX_PAGE;
    html.replace("%ssid%", this->_wifiConfig->ssid());
    html.replace("%local_ip%", WiFi.localIP().toString());
    html.replace("%delay%", String(this->_commonConfig->rebootDelay()));

    String mqtt_settings = "";
    if (_isAP) {
        mqtt_settings += "<p class=\"warning\">WiFi не подключен</p>";
        mqtt_settings += "<p class=\"warning\">MQTT не запущен</p>";
    } else {
        mqtt_settings += "<p>IP: <strong>" + WiFi.localIP().toString() + "</strong></p>";
        mqtt_settings += "<p>Порт: <strong>1883</strong></p>";
    }
    html.replace("%mqtt_settings%", mqtt_settings);

    this->_server->send(200, "text/html", html);
}

// Обработчик страницы настройки
void WebSrv::handleSetup() {
    String html = SETUP_PAGE;
    html.replace("%ssid%", this->_wifiConfig->ssid());
    html.replace("%password%", this->_wifiConfig->password());
    html.replace("%delay%", String(this->_commonConfig->rebootDelay()));

    this->_server->send(200, "text/html", html);
}

// Обработчик сохранения настроек
void WebSrv::handleSaveWiFiSetup() {
    this->_wifiConfig->ssid(this->_server->arg("ssid"));
    this->_wifiConfig->password(this->_server->arg("password"));
    this->_commonConfig->rebootDelay(this->_server->arg("reboot_delay").toInt());
    this->_wifiConfig->save();

    String html = ALERT_PAGE;
    html.replace("%title%", "Настройки");
    html.replace("%message%", "Настройки сохранены. Перезагрузите устройство чтобы изменения вступили в силу.");
    this->_server->send(200, "text/html", html);
}

// Обработчик страницы перезагрузки
void WebSrv::handleReboot() {
    String html = REBOOT_PAGE;
    html.replace("%delay%", String(this->_commonConfig->rebootDelay()));

    this->_server->send(200, "text/html", html);

    this->rebootFunction();
}

// Обработчик страницы ошибки 404
void WebSrv::handleNotFound() {
    String html = NOT_FOUND_PAGE;
    html.replace("%delay%", String(this->_commonConfig->rebootDelay()));

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
