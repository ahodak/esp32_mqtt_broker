#pragma once

#include "head.tpl"
#include "menu.tpl"
#include "footer.tpl"

#define NOT_FOUND_PAGE "\
<!DOCTYPE html>\
<html lang='ru'>\
    <head>\
        <title>MQTT Broker - Страница не найдена</title>\
        " PAGE_HEAD "\
    </head>\
    <body>\
        " MENU_PAGE "\
        <div class='alert alert-danger' role='alert'>\
            <strong>Страница не найдена</strong>\
        </div>\
        " FOOTER_PAGE "\
    </body>\
</html>"
