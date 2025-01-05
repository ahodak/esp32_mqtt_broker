#pragma once

#include "menu.tpl"
#include "footer.tpl"

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
        " MENU_PAGE "\
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
                <hr />\
                <button type='submit' class='btn btn-primary marginTop0'>Сохранить</button>\
            </form>\
        </div>\
        " FOOTER_PAGE "\
    </body>\
</html>"
