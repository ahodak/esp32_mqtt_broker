#pragma once

#define NOT_FOUND_PAGE "\
<!DOCTYPE html>\
<html lang='ru'>\
    <head>\
        <title>MQTT Broker - Страница не найдена</title>\
        <meta charset='utf-8'/>\
        <meta name='viewport' content='width=device-width, initial-scale=1'/>\
    </head>\
    <body>\
        <div class='text-center'>\
            <button class='btn btn-primary' onclick='window.location.href=\"/setup\"'>Настройка</button>\
            <button class='btn btn-danger' onclick='window.location.href=\"/reboot?delay=%delay%\"'>Перезагрузка</button>\
            <button class='btn btn-warning' onclick='window.location.href=\"/\"'>Назад</button>\
        </div>\
    </body>\
</html>"
