#pragma once

#include <Arduino.h>
#include <WebServer.h>

#include "wifi_config.h"
#include "common_config.h"

#define WEB_SERVER_PORT 80


class WebSrv
{
    private:
        WebServer* _server;

        WiFiConfig* _wifiConfig;
        CommonConfig* _commonConfig;

        bool _isAP;

        void handleStyles();
        void handleScripts();
        void handleRoot();
        void handleSetup();
        void handleSaveWiFiSetup();
        void handleSaveRebootSetup();
        void handleReboot();
        void handleNotFound();
        String renderParameterRow(String paramName, String paramId, String paramValue, int maxLength, bool isReadonly, bool isPassword);

    public:
        typedef std::function<void(void)> TRebootFunction;

        void init(WebServer* server, WiFiConfig* wifiConfig, CommonConfig* commonConfig, bool isAP, TRebootFunction rebootFunction);
        void loop();

    protected:
        TRebootFunction rebootFunction;
};
