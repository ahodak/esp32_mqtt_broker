#pragma once

#include <Arduino.h>
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

void initDisplay();
void clearDisplay();

void displayError(String message);
void displayLine(String message);
void displayProgress(int startFromLine);
