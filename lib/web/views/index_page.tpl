#pragma once

#include "menu.tpl"
#include "footer.tpl"

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
        " MENU_PAGE "\
        <div class='container'>\
            <div class='row'>\
                <h1>Параметры MQTT</h1>\
                <p>Версия протокола: <strong>3.1.1</strong></p>\
                <div class='mqtt-status'>\
                    %if_ap_mode%\
                    <p class='warning'>WiFi не подключен</p>\
                    <p class='warning'>MQTT не запущен</p>\
                    %endif_ap_mode%\
                    %if_sta_mode%\
                    <p>IP: <strong>%ip%</strong></p>\
                    <p>Порт: <strong>1883</strong></p>\
                    %endif_sta_mode%\
                </div>\
            </div>\
            <hr />\
            <div class='row' style='margin-bottom: 20px;'>\
                <h1>Параметры WiFi</h1>\
                <p>SSID: <strong>%ssid%</strong></p>\
            </div>\
        </div>\
        " FOOTER_PAGE "\
    </body>\
</html>"