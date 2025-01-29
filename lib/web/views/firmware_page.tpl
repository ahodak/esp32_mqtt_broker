#pragma once

#include "head.tpl"
#include "menu.tpl"
#include "footer.tpl"

#define OTA_PAGE "\
<!DOCTYPE html>\
<html lang='ru'>\
    <head>\
        <title>MQTT Broker - Настройки</title>\
        " PAGE_HEAD "\
    </head>\
    <body>\
        " MENU_PAGE "\
        <div class='container'>\
            <h1>Обновление прошивки</h1>\
            <hr/>\
            <p>Текущая версия: %version%</p>\
%if_has_new_version%\
            <p>Новая версия: %new_version%</p>\
            <form action='/firmware' method='post'>\
                <button type='submit' class='btn btn-primary'>Обновить</button>\
            </form>\
%endif_has_new_version%\
%if_new_version_unavailable%\
            <p class='text-danger'>Не удалось получить информацию о последней версии</p>\
%endif_new_version_unavailable%\
        </div>\
        " FOOTER_PAGE "\
    </body>\
</html>"
