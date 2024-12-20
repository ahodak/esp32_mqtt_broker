#pragma once

#include "menu.h"
#include "footer.h"

#define SUBSCRIBE_PAGE "\
<!DOCTYPE html>\
<html lang='ru'>\
    <head>\
        <title>MQTT Broker - Подписка</title>\
        <meta charset='utf-8'/>\
        <meta name='viewport' content='width=device-width, initial-scale=1'/>\
        <link rel='stylesheet' href='/styles.css'/>\
    </head>\
    <body>\
        " MENU_PAGE "\
        <div class='container'>\
            <form action='/subscribe' method='post'>\
                <div class='input-group'>\
                    <span class='input-group-addon'>Топик</span>\
                    <input class='form-control' type='text' name='topic' maxlength='32' value='topic/#' required/>\
                </div>\
                <hr />\
                <button type='submit' class='btn btn-primary marginTop0'>Подписаться</button>\
                <button type='submit' class='btn btn-secondary marginTop0' formaction='/unsubscribe'>Отписаться</button>\
                %if_message%\
                <div class='input-group info-message'>\
                    %message%\
                </div>\
                %endif_message%\
            </form>\
        </div>\
        " FOOTER_PAGE "\
    </body>\
</html>"
