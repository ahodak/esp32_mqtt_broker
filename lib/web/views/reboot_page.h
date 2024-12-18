#pragma once

#define REBOOT_PAGE "\
<!DOCTYPE html>\
<html lang='ru'>\
    <head>\
        <title>MQTT Broker - Перезагрузка</title>\
        <meta charset='utf-8'/>\
        <meta name='viewport' content='width=device-width, initial-scale=1'/>\
        <link rel='stylesheet' href='/styles.css'/>\
        <script type='text/javascript'>\
        function getUrlVars() {\
            var vars = [], hash;\
            var hashes = window.location.href.slice(window.location.href.indexOf('?') + 1).split('&');\
            for(var i = 0; i < hashes.length; i++)\
            {\
                hash = hashes[i].split('=');\
                vars.push(hash[0]);\
                vars[hash[0]] = hash[1];\
            }\
            return vars;\
        } \
        window.onload = function() {\
            var params = getUrlVars();\
            var delay = parseInt(params['delay'], 10);\
            console.log(delay);\
            var txt = document.getElementById('info');\
            var counter = delay;\
            var id = setInterval(function () {\
                txt.innerHTML = 'Модуль будет перезагружен через ' + counter + ' секунд.';\
                counter--;\
                console.log(counter);\
                if (counter < 0) {\
                    clearInterval(id);\
                    txt.innerHTML = 'Модуль был перезагружен.';\
                    console.log('Reboot!');\
                }\
            }, 1000);\
        }\
        </script>\
    </head>\
    <body>\
        <div class='text-center'>\
            <button class='btn btn-primary' onclick='window.location.href=\"/setup\"'>Настройка</button>\
            <button class='btn btn-danger' onclick='window.location.href=\"/reboot?delay=%delay%\"'>Перезагрузка</button>\
            <button class='btn btn-warning' onclick='window.location.href=\"/\"'>Назад</button>\
        </div>\
        <div class='alert alert-info'>\
            <strong id='info'>Модуль будет перезагружен через %delay% секунд.</strong>\
        </div>\
    </body>\
</html>"
