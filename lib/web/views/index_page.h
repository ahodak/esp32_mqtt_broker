#pragma once

#define INDEX_PAGE "\
<!DOCTYPE html>\
<html lang='ru'>\
    <head>\
        <title>MQTT Broker</title>\
        <meta charset='utf-8'/>\
        <meta name='viewport' content='width=device-width, initial-scale=1'/>\
        <link rel='stylesheet' href='/styles.css'/>\
    </head>\
    <body>\
        <div class='text-center'>\
            <button class='btn btn-primary' onclick='window.location.href=\"/\"'>Главная</button>\
            <button class='btn btn-primary' onclick='window.location.href=\"/setup\"'>Настройка</button>\
            <button class='btn btn-danger' onclick='window.location.href=\"/reboot?delay=%delay%\"'>Перезагрузка</button>\
        </div>\
        <h1>MQTT Broker</h1>\
        <h2>Параметры MQTT</h2>\
        <p>Версия протокола: <strong>3.1.1</strong></p>\
        %mqtt_settings%\
        <h2>Параметры WiFi</h2>\
        <p>SSID: <strong>%ssid%</strong></p><br/>\
        <p>&copy; Золин Павел &lt;pashamasterz@yandex.ru&gt;, 2024</p>\
    </body>\
</html>"
