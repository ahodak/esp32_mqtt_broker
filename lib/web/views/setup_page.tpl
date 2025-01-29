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
            <form action='/setup' method='post' class='settings needs-validation' novalidate>\
                <div class='row mb-3'>\
                    <div class='col-md-6'>\
                        <div class='card bg-light mb-3'>\
                            <h5 class='card-header'>Wi-Fi</h5>\
                            <div class='card-body'>\
                                <div class='form-floating'>\
                                    <input class='form-control ssid' type='text' name='ssid' placeholder='SSID' maxlength='32' value='%ssid%' required/>\
                                    <label for='ssid'>SSID</label>\
                                </div>\
                                <div class='form-floating'>\
                                    <input class='form-control password' type='password' name='password' placeholder='Пароль' maxlength='32' value='%password%' required/>\
                                    <label for='password'>Пароль</label>\
                                </div>\
                                <div class='form-floating'>\
                                    <input class='form-control hostname' type='text' name='hostname' placeholder='esp-mqtt' maxlength='25' value='%hostname%' required/>\
                                    <label for='hostname'>Имя хоста</label>\
                                </div>\
                            </div>\
                        </div>\
                    </div>\
                    <div class='col-md-6'>\
                        <div class='card bg-light mb-3'>\
                            <h5 class='card-header'>MQTT</h5>\
                            <div class='card-body'>\
                                <div class='form-floating'>\
                                    <input class='form-control mqtt_user' type='text' name='mqtt_user' placeholder='Пользователь' maxlength='32' value='%mqtt_user%'/>\
                                    <label for='mqtt_user'>Пользователь</label>\
                                </div>\
                                <div class='form-floating'>\
                                    <input class='form-control mqtt_password' type='password' name='mqtt_password' placeholder='Пароль' maxlength='32' value='%mqtt_password%'/>\
                                    <label for='mqtt_password'>Пароль</label>\
                                </div>\
                            </div>\
                        </div>\
                    </div>\
                </div>\
                <div class='row mb-3'>\
                    <div class='col-md-6'>\
                        <div class='card bg-light mb-3'>\
                            <h5 class='card-header'>Модуль</h5>\
                            <div class='card-body'>\
                                <div class='form-floating'>\
                                    <input class='form-control' type='number' name='reboot_delay' value='%reboot_delay%' required/>\
                                    <label for='reboot_delay'>Перезагрузка, сек:</label>\
                                </div>\
                            </div>\
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
