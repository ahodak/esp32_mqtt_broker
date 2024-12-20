#pragma once

#include "menu.h"
#include "footer.h"

#define PUBLISH_PAGE "\
<!DOCTYPE html>\
<html lang='ru'>\
    <head>\
        <title>MQTT Broker - Публикация</title>\
        <meta charset='utf-8'/>\
        <meta name='viewport' content='width=device-width, initial-scale=1'/>\
        <link rel='stylesheet' href='/styles.css'/>\
    </head>\
    <body>\
        " MENU_PAGE "\
        <div class='container'>\
            <form action='/publish' method='post'>\
                <div class='input-group'>\
                    <span class='input-group-addon'>Топик</span>\
                    <input class='form-control' type='text' name='topic' maxlength='32' value='topic/1' required/>\
                </div>\
                <div class='input-group'>\
                    <span class='input-group-addon'>Сообщение</span>\
                    <textarea class='form-control' name='payload' rows='5' required>{\n  \"msg\": \"Hello!\"\n}</textarea>\
                </div>\
                <div class='input-group'>\
                    <span class='input-group-addon'>Доставка</span>\
                    <select class='form-control' name='qos'>\
                        <option value='0'>как максимум один раз</option>\
                        <option value='1'>как минимум один раз</option>\
                    </select>\
                </div>\
                <div class='input-group'>\
                    <span class='input-group-addon'>Сохранять <i class='info-icon' title='Сохранить сообщение и отправить новым подписчикам сразу после подписки на топик'>(i)</i></span>\
                    <input class='form-control' type='checkbox' name='retain' value='0'/>\
                </div>\
                <div class='input-group'>\
                    <span class='input-group-addon'>ID сообщения</span>\
                    <input class='form-control' type='number' name='message_id' value='0' required/>\
                </div>\
                <hr/>\
                <button class='btn btn-primary marginTop0'>Отправить</button>\
                %if_message%\
                <div class='info-message'>%message%</div>\
                %endif_message%\
            </form>\
        </div>\
        " FOOTER_PAGE "\
    </body>\
</html>"
