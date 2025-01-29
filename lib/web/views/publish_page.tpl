#pragma once

#include "head.tpl"
#include "menu.tpl"
#include "footer.tpl"

#define PUBLISH_PAGE "\
<!DOCTYPE html>\
<html lang='ru'>\
    <head>\
        <title>MQTT Broker - Публикация</title>\
        " PAGE_HEAD "\
    </head>\
    <body>\
        " MENU_PAGE "\
        <div class='container'>\
            <h1>Публикация сообщения в топик</h1>\
            <hr/>\
            <form action='/publish' method='post' class='needs-validation' novalidate>\
                <div class='row mb-3'>\
                    <div class='col-12'>\
                        <div class='form-floating'>\
                            <input class='form-control' type='text' name='topic' maxlength='32' value='topic/1' required/>\
                            <label for='topic'>Топик</label>\
                        </div>\
                    </div>\
                </div>\
                <div class='row mb-3'>\
                    <div class='col-12'>\
                        <div class='form-floating'>\
                            <textarea class='form-control' name='payload' rows='5' style='height: auto !important;' required>{\n  \"msg\": \"Hello!\"\n}</textarea>\
                            <label for='payload'>Сообщение</label>\
                        </div>\
                    </div>\
                </div>\
                <hr/>\
                <div class='row mb-3'>\
                    <div class='col-2'>\
                        <button type='submit' class='btn btn-primary'>Отправить</button>\
                    </div>\
                    %if_message%\
                    <div class='col-10'>\
                        <div class='alert alert-primary' role='alert'>%message%</div>\
                    </div>\
                    %endif_message%\
                </div>\
            </form>\
        </div>\
        " FOOTER_PAGE "\
    </body>\
</html>"
