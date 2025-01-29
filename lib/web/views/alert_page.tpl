#pragma once

#include "head.tpl"
#include "menu.tpl"
#include "footer.tpl"

#define ALERT_PAGE "\
<!DOCTYPE html>\
<html lang='ru'>\
    <head>\
        <title>MQTT Broker - %title%</title>\
        " PAGE_HEAD "\
    </head>\
    <body>\
        " MENU_PAGE "\
        <div class='container'>\
            <div class='alert alert-primary' role='alert'>\
                %message%\
            </div>\
        </div>\
        " FOOTER_PAGE "\
    </body>\
</html>"
