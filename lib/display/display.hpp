/*
 *  Отображение информации на дисплее.
 *  -----------------------------------------------------------------------------------------------------------------------
 *  (c) 2025 Ходак Андрей | Andrey Khodak
 *  andrey@khodak.ru | https://khodak.ru | tg: @akhodak
*/
#pragma once

#include <Arduino.h>
#include <esp_log.h>

#ifdef TTGO_T_DISPLAY
// В файле TFT_eSPI/User_Setup_Select.h раскомментировать строку
// #include <User_Setups/Setup25_TTGO_T_Display.h>
#include <TFT_eSPI.h>
#endif

#define DISPLAY_BORDER 10

#define LINE_HEIGHT 21
#define COLUMN_WIDTH 8

#define MAX_LINE_COUNT 6 
#define MAX_COLUMN_COUNT 27

#define DOT "." 

// Класс для работы с дисплеем
class Display {
private:
    const char* _logTAG = "Display";

#ifdef TTGO_T_DISPLAY
    TFT_eSPI* _tft;
#endif

    int _currentLine = 0;
    int _currentColumn = 0;
    bool _isFirstAppear = true;

public:
    // Конструктор
    Display();

    // Инициализация дисплея
    void init();
    
    // Очистка дисплея
    void clear();
    
    // Отображение ошибки
    void error(String message);
    
    // Отображение строки
    void line(String message);
    
    // Отображение прогресса
    void progress(int startFromLine);
};
