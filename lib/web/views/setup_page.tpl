#pragma once

#include "head.tpl"
#include "menu.tpl"
#include "footer.tpl"

#define SETUP_PAGE "\
<!DOCTYPE html>\
<html lang='ru'>\
    <head>\
        <title>MQTT Broker - Настройки</title>\
        " PAGE_HEAD "\
    </head>\
    <body>\
        " MENU_PAGE "\
        <div class='container'>\
            <h1>Настройки</h1>\
            <hr/>\
            <form action='/setup' method='post' class='needs-validation' novalidate>\
                <h3>Wi-Fi</h3>\
                <div class='row mb-3'>\
                    <div class='col-6'>\
                        <div class='form-floating'>\
                            <input class='form-control' type='text' name='ssid' placeholder='SSID' maxlength='32' value='%ssid%' required/>\
                            <label for='ssid'>SSID</label>\
                        </div>\
                    </div>\
                    <div class='col-6'>\
                        <div class='form-floating'>\
                            <input class='form-control' type='password' name='password' placeholder='Пароль' maxlength='32' value='%password%' required/>\
                            <label for='password'>Пароль</label>\
                        </div>\
                    </div>\
                </div>\
                <h3>MQTT</h3>\
                <div class='row mb-3'>\
                    <div class='col-6'>\
                        <div class='form-floating'>\
                            <input class='form-control' type='text' name='mqtt_user' placeholder='Пользователь' maxlength='32' value='%mqtt_user%'/>\
                            <label for='mqtt_user'>Пользователь</label>\
                        </div>\
                    </div>\
                    <div class='col-6'>\
                        <div class='form-floating'>\
                            <input class='form-control' type='password' name='mqtt_password' placeholder='Пароль' maxlength='32' value='%mqtt_password%'/>\
                            <label for='mqtt_password'>Пароль</label>\
                        </div>\
                    </div>\
                </div>\
                <h3>Перезагрузка</h3>\
                <div class='row mb-3'>\
                    <div class='col-6'>\
                        <div class='form-floating'>\
                            <input class='form-control' type='number' name='reboot_delay' placeholder='Задержка перезагрузки' value='%delay%' required/>\
                            <label for='reboot_delay'>Перезагрузка, сек:</label>\
                        </div>\
                    </div>\
                </div>\
                <hr/>\
                <button type='submit' class='btn btn-primary'>Сохранить</button>\
            </form>\
        </div>\
        " FOOTER_PAGE "\
    </body>\
</html>"
