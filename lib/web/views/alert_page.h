#pragma once

#define ALERT_PAGE "\
<!DOCTYPE html>\
<html lang='ru'>\
    <head>\
        <title>MQTT Broker - %title%</title>\
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
        <h1>%title%</h1>\
        <p>%message%</p>\
    </body>\
</html>"
