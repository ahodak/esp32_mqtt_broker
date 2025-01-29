#pragma once

#include "head.tpl"
#include "menu.tpl"
#include "footer.tpl"

#define SUBSCRIBE_PAGE "\
<!DOCTYPE html>\
<html lang='ru'>\
    <head>\
        <title>MQTT Broker - Подписка</title>\
        " PAGE_HEAD "\
    </head>\
    <body>\
        " MENU_PAGE "\
        <div class='container'>\
            <h1>Подписка на сообщения в топике</h1>\
            <hr/>\
            <form action='/subscribe' method='post' class='needs-validation' novalidate>\
                <div class='row mb-3'>\
                    <div class='col-12'>\
                        <div class='form-floating'>\
                            <input class='form-control' type='text' name='topic' maxlength='32' value='topic/#' required/>\
                            <label for='topic'>Топик</label>\
                        </div>\
                    </div>\
                </div>\
                <hr/>\
                <div class='row mb-3'>\
                    <div class='col-3'>\
                        <button type='submit' class='btn btn-primary'>Подписаться</button>\
                        <button type='submit' class='btn btn-secondary' formaction='/unsubscribe'>Отписаться</button>\
                    </div>\
                    %if_message%\
                    <div class='col-9'>\
                        <div class='alert alert-primary' role='alert'>%message%</div>\
                    </div>\
                    %endif_message%\
                </div>\
            </form>\
        </div>\
        " FOOTER_PAGE "\
    </body>\
</html>"
