#include "display.hpp"

#ifdef TTGO_T_DISPLAY
TFT_eSPI tft = TFT_eSPI(TFT_WIDTH, TFT_HEIGHT);
#endif

int currentLine = 0;
int currentColumn = 0;
bool isFirstAppear = true;

void clearDisplay() {
    currentLine = 0;
    currentColumn = 0;
    isFirstAppear = true;

    #ifdef TTGO_T_DISPLAY
    tft.fillScreen(TFT_BLACK);
    #endif
}

void initDisplay() {
    #ifdef TTGO_T_DISPLAY
    tft.init();
    tft.setRotation(1);
    tft.setTextFont(2);
    #endif

    clearDisplay();
}

void displayError(String message) {
    clearDisplay();

    Serial.println(message);

    #ifdef TTGO_T_DISPLAY
    tft.drawString(message, DISPLAY_BORDER, DISPLAY_BORDER);
    #endif
}

void displayLine(String message) {
    Serial.println(message);

    #ifdef TTGO_T_DISPLAY
    tft.drawString(message, DISPLAY_BORDER, DISPLAY_BORDER + currentLine * LINE_HEIGHT);
    #endif

    currentLine += 1;
    if (currentLine > MAX_LINE_COUNT) {
        currentLine = 0;
        currentColumn = 0;
    }

    isFirstAppear = true;
}

void displayProgress(int startFromLine) {
    Serial.print(DOT);

    if (isFirstAppear) {
        currentLine = startFromLine;
        isFirstAppear = false;
    }

    #ifdef TTGO_T_DISPLAY
    tft.drawString(DOT, DISPLAY_BORDER + currentColumn * COLUMN_WIDTH, DISPLAY_BORDER + currentLine * LINE_HEIGHT);
    #endif

    currentColumn += 1;
    if (currentColumn > MAX_COLUMN_COUNT) {
        currentColumn = 0;
        if (currentLine > MAX_LINE_COUNT) {
            currentLine = startFromLine;
        }
        else {
            currentLine += 1;
        }
    }
}
