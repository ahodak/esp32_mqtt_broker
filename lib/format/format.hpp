/*
 *  Форматирование данных
 *  -----------------------------------------------------------------------------------------------------------------------
 *  (c) 2025 Ходак Андрей | Andrey Khodak
 *  andrey@khodak.ru | https://khodak.ru | tg: @akhodak
*/
#pragma once

#include <Arduino.h>

String format_zero_padding(int value, int width);
String format_value(double value);
String format_si(double value, int decimal_places = 2);

String format_memory(size_t bytes, int decimal_places = 2);
String format_duration(time_t seconds);