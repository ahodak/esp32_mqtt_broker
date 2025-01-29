#pragma once

#include "head.tpl"
#include "menu.tpl"
#include "footer.tpl"

#define REBOOT_PAGE "\
<!DOCTYPE html>\
<html lang='ru'>\
    <head>\
        <title>MQTT Broker - Перезагрузка</title>\
        " PAGE_HEAD "\
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
        " MENU_PAGE "\
        <div class='container'>\
            <div id='info' class='alert alert-info' role='alert'>\
                Модуль будет перезагружен через %delay% секунд.\
            </div>\
        </div>\
        " FOOTER_PAGE "\
    </body>\
</html>"
