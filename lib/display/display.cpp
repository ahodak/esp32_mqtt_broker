#include "display.hpp"

Display::Display() {
    ESP_LOGV(_logTAG, "IN: Display::Display() constructor");
    #ifdef TTGO_T_DISPLAY
    this->_tft = new TFT_eSPI(TFT_WIDTH, TFT_HEIGHT);
    #endif
    ESP_LOGV(_logTAG, "OUT: Display::Display() constructor");
}

void Display::init() {
    ESP_LOGV(_logTAG, "IN: Display::init()");
    #ifdef TTGO_T_DISPLAY
    this->_tft->init();

    this->_tft->setRotation(1);
    this->_tft->setTextFont(2);
    ESP_LOGI("Main", "Display initialized");
    #endif

    this->clear();
    ESP_LOGV(_logTAG, "OUT: Display::init()");
}

void Display::clear() {
    ESP_LOGV(_logTAG, "IN: Display::clear()");
    this->_currentLine = 0;
    this->_currentColumn = 0;
    this->_isFirstAppear = true;

    Serial.println("");
    #ifdef TTGO_T_DISPLAY
    this->_tft->fillScreen(TFT_BLACK);
    ESP_LOGI("Main", "Display cleared");
    #endif

    ESP_LOGV(_logTAG, "OUT: Display::clear()");
}

void Display::error(String message) {
    ESP_LOGV(_logTAG, "IN: Display::error(%s)", message.c_str());
    this->clear();

    ESP_LOGE("Main", "%s", message.c_str());

    #ifdef TTGO_T_DISPLAY
    this->_tft->drawString(message, DISPLAY_BORDER, DISPLAY_BORDER);
    ESP_LOGV(_logTAG, "Error displayed");
    #endif

    ESP_LOGV(_logTAG, "OUT: Display::error()");
}

void Display::line(String message) {
    ESP_LOGV(_logTAG, "IN: Display::line(%s)", message.c_str());
    ESP_LOGI("Main", "%s", message.c_str());

    #ifdef TTGO_T_DISPLAY
    this->_tft->drawString(message, DISPLAY_BORDER, DISPLAY_BORDER + this->_currentLine * LINE_HEIGHT);
    #endif

    this->_currentLine += 1;
    if (this->_currentLine > MAX_LINE_COUNT) {
        this->_currentLine = 0;
        this->_currentColumn = 0;
    }

    this->_isFirstAppear = true;

    ESP_LOGV(_logTAG, "OUT: Display::line()");
}

void Display::progress(int startFromLine) {
    ESP_LOGV(_logTAG, "IN: Display::progress(startFromLine=%d)", startFromLine);
    Serial.print(DOT);

    if (this->_isFirstAppear) {
        this->_currentLine = startFromLine;
        this->_isFirstAppear = false;
    }

    #ifdef TTGO_T_DISPLAY
    this->_tft->drawString(DOT, DISPLAY_BORDER + this->_currentColumn * COLUMN_WIDTH, DISPLAY_BORDER + this->_currentLine * LINE_HEIGHT);
    #endif

    this->_currentColumn += 1;
    if (this->_currentColumn > MAX_COLUMN_COUNT) {
        this->_currentColumn = 0;
        if (this->_currentLine > MAX_LINE_COUNT) {
            this->_currentLine = startFromLine;
        }
        else {
            this->_currentLine += 1;
        }
    }

    ESP_LOGV(_logTAG, "OUT: Display::progress()");
}
