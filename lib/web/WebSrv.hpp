#pragma once

#include <Arduino.h>
#ifdef ESP32
#include <WebServer.h>
#elif ESP8266
#include <ESP8266WebServer.h>
#endif

#include "version.hpp"
#include "format.hpp"

#include "wifi_config.hpp"
#include "common_config.hpp"

#define WEB_SERVER_PORT 80

extern const uint8_t binary_lib_web_bootstrap_min_css_gz_start[] asm("_binary_lib_web_bootstrap_min_css_gz_start");
extern const uint8_t binary_lib_web_bootstrap_min_css_gz_end[] asm("_binary_lib_web_bootstrap_min_css_gz_end");
extern const uint8_t binary_lib_web_bootstrap_min_js_gz_start[] asm("_binary_lib_web_bootstrap_min_js_gz_start");
extern const uint8_t binary_lib_web_bootstrap_min_js_gz_end[] asm("_binary_lib_web_bootstrap_min_js_gz_end");

class WebSrv
{
    private:
#ifdef ESP32
        WebServer* _server;
#elif ESP8266
        ESP8266WebServer* _server;
#endif

        WiFiConfig* _wifiConfig;
        CommonConfig* _commonConfig;

        bool _isAP;

        void handleStyles();
        void handleBootstrapCss();
        void handleBootstrapJs();
        void handleScripts();
        void handleShowIndex();
        void handleShowPublish(String message);
        void handlePublish();
        void handleShowSubscribe(String message);
        void handleSubscribe();
        void handleUnsubscribe();
        void handleShowSettings();
        void handleSaveSettings();
        void handleReboot();
        void handleGetModuleParams();
        void handleNotFound();
        void processConditionalBlock(String& html, const String& conditionName, bool keepBlock);
        void replaceCommonTemplateVars(String& html);
        void showMessage(String& html, String message);
        String renderParameterRow(String paramName,
                                 String paramId,
                                 String paramValue,
                                 int maxLength,
                                 bool isReadonly,
                                 bool isPassword);
        void sendContentGzip(const unsigned char *content, size_t length, const char *mime_type);

    public:
        typedef std::function<void(void)> TRebootFunction;
        typedef std::function<void(String topic, String payload)> TPublishMessageFunction;
        typedef std::function<void(String topic_filter)> TSubscribeFunction;
        typedef std::function<void(String topic_filter)> TUnsubscribeFunction;
#ifdef ESP32
        void init(WebServer* server,
#elif ESP8266
        void init(ESP8266WebServer* server,
#endif
                WiFiConfig* wifiConfig,
                CommonConfig* commonConfig,
                bool isAP,
                TRebootFunction rebootFunction,
                TPublishMessageFunction publishMessageFunction,
                TSubscribeFunction subscribeFunction,
                TUnsubscribeFunction unsubscribeFunction);
        void loop();

    protected:
        TRebootFunction rebootFunction;
        TPublishMessageFunction publishMessageFunction;
        TSubscribeFunction subscribeFunction;
        TUnsubscribeFunction unsubscribeFunction;
};
