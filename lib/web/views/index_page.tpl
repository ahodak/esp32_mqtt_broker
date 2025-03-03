#pragma once

#include "head.tpl"
#include "menu.tpl"
#include "footer.tpl"

#define INDEX_PAGE "\
<!DOCTYPE html>\
<html lang='ru'>\
    <head>\
        <title>MQTT Broker</title>\
        " PAGE_HEAD "\
    </head>\
    </head>\
    <body>\
        " MENU_PAGE "\
        <div class='container' id='main'>\
            <div class='row'>\
                <div class='col-md-6'>\
                    <div class='card bg-light mb-3'>\
                        <h5 class='card-header'>MQTT брокер</h5>\
                        <div class='card-body'>\
%if_ap_mode%\
                            <div class='row'>\
                                <div class='col-4'>MQTT:</div>\
                                <div class='col-8 text-danger'>не запущен</div>\
                            </div>\
%endif_ap_mode%\
%if_sta_mode%\
                            <div class='row'>\
                                <div class='col-4'>Версия протокола:</div>\
                                <div class='col-8'><strong>3.1.1</strong></div>\
                            </div>\
                            <div class='row'>\
                                <div class='col-4'>IP:</div>\
                                <div class='col-8'>%ip%</div>\
                            </div>\
                            <div class='row'>\
                                <div class='col-4'>Порт:</div>\
                                <div class='col-8'>1883</div>\
                            </div>\
                            <div class='row'>\
                                <div class='col-4'>Пользователь:</div>\
                                <div class='col-8'>%mqtt_user%</div>\
                            </div>\
                            <div class='row'>\
                                <div class='col-4'>Пароль:</div>\
                                <div class='col-8'>%mqtt_password%</div>\
                            </div>\
%endif_sta_mode%\
                        </div>\
                    </div>\
                </div>\
                <div class='col-md-6'>\
                    <div class='card bg-light mb-3'>\
                        <h5 class='card-header'>Модуль</h5>\
                        <div class='card-body'>\
                            <div class='row'>\
                            <div class='col-4'>Модель:</div>\
                                <div class='col-8'>%module_chip_model%%module_chip_revision%</div>\
                            </div>\
                            <div class='row'>\
                                <div class='col-4'>Частота:</div>\
                                <div class='col-8'>%module_chip_speed% MHz</div>\
                            </div>\
                            <div class='row'>\
                                <div class='col-4'>Ядер:</div>\
                                <div class='col-8'>%module_chip_cores%</div>\
                            </div>\
                            <div class='row'>\
                                <div class='col-4'>Размер ОЗУ:</div>\
                                <div class='col-8'>%module_heap_size%</div>\
                            </div>\
                            <div class='row'>\
                                <div class='col-4'>Размер Flash:</div>\
                                <div class='col-8'>%module_chip_flash_size%</div>\
                            </div>\
                        </div>\
                    </div>\
                </div>\
            </div>\
            <div class='row'>\
                <div class='col-md-6'>\
                    <div class='card bg-light mb-3'>\
                        <h5 class='card-header'>Сеть %network%</h5>\
                        <div class='card-body'>\
                            <div class='row'>\
                                <div class='col-4'>MAC:</div>\
                                <div class='col-8'>%mac_address%</div>\
                            </div>\
                            <div class='row'>\
                                <div class='col-4'>IP:</div>\
                                <div class='col-8'>%ip%</div>\
                            </div>\
                            <div class='row'>\
                                <div class='col-4'>Host:</div>\
                                <div class='col-8'>%hostname%</div>\
                            </div>\
%if_wifi_mode%\
                            <div class='row'>\
                                <div class='col-4'>SSID:</div>\
                                <div class='col-8'>%ssid%</div>\
                            </div>\
                            <div class='row'>\
                                <div class='col-4'>Уровень сигнала:</div>\
                                <div class='col-8'><span id='rssi'>%rssi%</span> дБм</div>\
                            </div>\
%endif_wifi_mode%\
                        </div>\
                    </div>\
                </div>\
                <div class='col-md-6'>\
                    <div class='card bg-light mb-3'>\
                        <h5 class='card-header'>Диагностика</h5>\
                        <div class='card-body'>\
                            <div class='row'>\
                                <div class='col-4'>Время работы:</div>\
                                <div class='col-8'><span id='uptime'>%uptime%</span> сек.</div>\
                            </div>\
                            <div class='row'>\
                                <div class='col-4'>Свободно ОЗУ:</div>\
                                <div class='col-8'><span id='heap_free'>%module_heap_free%</span> байт</div>\
                            </div>\
                            <div class='row'>\
                                <div class='col-4'>Макс. размер блока:</div>\
                                <div class='col-8'>%module_heap_max%</div>\
                            </div>\
%if_sensors_enabled%\
                            <div class='row'>\
                                <div class='col-4'>Датчик температуры:</div>\
                                <div class='col-8'>GPIO%one_wire_bus%</div>\
                            </div>\
%endif_sensors_enabled%\
                        </div>\
                    </div>\
                </div>\
            </div>\
        </div>\
        " FOOTER_PAGE "\
        <script>\
            setInterval(function()\
            {\
                getModuleParams();\
            }, 1000);\
            function getModuleParams() {\
                var xhttp = new XMLHttpRequest();\
                xhttp.onreadystatechange = function() {\
                    if (this.readyState == 4 && this.status == 200) {\
                        var params = this.responseText.split(',');\
                        document.getElementById('rssi').innerHTML = params[0];\
                        document.getElementById('uptime').innerHTML = params[1];\
                        document.getElementById('heap_free').innerHTML = params[2];\
                    }\
                };\
                xhttp.open('GET', 'module_params', true);\
                xhttp.send();\
            }\
        </script>\
    </body>\
</html>"
