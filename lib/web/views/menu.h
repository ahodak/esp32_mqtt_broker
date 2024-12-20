#pragma once

#define MENU_PAGE "\
<ul class='nav-menu'>\
  <li><a href='/'>Главная</a></li>\
  <li>\
    <a href='#'>Сообщения</a>\
    <ul>\
      <li><a href='/publish'>Публикация</a></li>\
      <li><a href='/subscribe'>Подписка</a></li>\
    </ul>\
  </li>\
  <li><a href='/setup'>Настройка</a></li>\
  <li class='danger'><a href='/reboot?delay=%delay%'>Перезагрузка</a></li>\
</ul>"
