#pragma once

#define SETUP_PAGE "\
<!DOCTYPE html>\
<html lang='ru'>\
    <head>\
        <title>MQTT Broker - Настройки</title>\
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
        <h1>Настройки</h1>\
        <div class='container'>\
            <form action='/setup' method='post'>\
                <div class='input-group'>\
                    <span class='input-group-addon'>SSID</span>\
                    <input class='form-control' type='text' name='ssid' placeholder='SSID' maxlength='32' value='%ssid%' required/>\
                </div>\
                <div class='input-group'>\
                    <span class='input-group-addon'>Пароль</span>\
                    <input class='form-control' type='password' name='password' placeholder='Пароль' maxlength='32' value='%password%' required/>\
                </div>\
                <hr />\
                <div class='input-group'>\
                    <span class='input-group-addon'>Перезагрузка, сек:</span>\
                    <input class='form-control' type='number' name='reboot_delay' placeholder='Задержка перезагрузки' value='%delay%' required/>\
                </div>\
                <button class='btn btn-default marginTop0'>Сохранить</button>\
            </form>\
        </div>\
    </body>\
</html>"
