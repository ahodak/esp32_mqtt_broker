#pragma once

#include "menu.h"
#include "footer.h"

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
        " MENU_PAGE "\
        <h1>%title%</h1>\
        <p>%message%</p>\
        " FOOTER_PAGE "\
    </body>\
</html>"
