#pragma once

#define MENU_PAGE "\
<nav class='navbar navbar-expand-md navbar-dark bg-dark sticky-top'>\
  <div class='container-fluid'>\
    <a class='navbar-brand' href='/'>MQTT Broker</a>\
    <button class='navbar-toggler' type='button' data-bs-toggle='collapse' data-bs-target='#navbarToggler' aria-controls='navbarToggler' aria-expanded='false' aria-label='Toggle navigation'>\
      <span class='navbar-toggler-icon'></span>\
    </button>\
    <div class='collapse navbar-collapse' id='navbarToggler'>\
      <ul class='navbar-nav me-auto mb-2 mb-lg-0'>\
        <li class='nav-item'>\
          <a class='nav-link' href='/'>Главная</a>\
        </li>\
        <li class='nav-item dropdown'>\
          <a class='nav-link dropdown-toggle' id='navbarUtilitiesDropdown' role='button' data-bs-toggle='dropdown' aria-haspopup='true' aria-expanded='false'>\
            Тесты\
          </a>\
          <ul class='dropdown-menu dropdown-menu-dark' aria-labelledby='navbarUtilitiesDropdown'>\
            <li><a class='dropdown-item' href='/subscribe'>Подписаться</a></li>\
            <li><a class='dropdown-item' href='/publish'>Опубликовать</a></li>\
          </ul>\
        </li>\
        <li class='nav-item dropdown'>\
          <a class='nav-link dropdown-toggle' id='navbarSetupDropdown' role='button' data-bs-toggle='dropdown' aria-haspopup='true' aria-expanded='false'>\
            Настройка\
          </a>\
          <ul class='dropdown-menu dropdown-menu-dark' aria-labelledby='navbarSetupDropdown'>\
            <li><a class='dropdown-item' href='/setup'>Настройки</a></li>\
            <li><a class='dropdown-item' href='/firmware'>Прошивка</a></li>\
          </ul>\
        </li>\
        <li class='nav-item'>\
          <a class='nav-link reboot' href='/reboot?delay=%delay%'>Перезагрузка</a>\
        </li>\
      </ul>\
    </div>\
  </div>\
</nav>"
