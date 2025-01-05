#pragma once

#include "menu.tpl"
#include "footer.tpl"

#define NOT_FOUND_PAGE "\
<!DOCTYPE html>\
<html lang='ru'>\
    <head>\
        <title>MQTT Broker - Страница не найдена</title>\
        <meta charset='utf-8'/>\
        <meta name='viewport' content='width=device-width, initial-scale=1'/>\
        <link rel='stylesheet' href='/styles.css'/>\
    </head>\
    <body>\
        " MENU_PAGE "\
        <div class='alert alert-danger'>\
            <strong>Страница не найдена</strong>\
        </div>\
        " FOOTER_PAGE "\
    </body>\
</html>"
