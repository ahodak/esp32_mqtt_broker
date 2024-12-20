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
        void handleShowIndex();
        void handleShowPublish(String message);
        void handlePublish();
        void handleShowSubscribe(String message);
        void handleSubscribe();
        void handleUnsubscribe();
        void handleShowSettings();
        void handleSaveSettings();
        void handleReboot();
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

    public:
        typedef std::function<void(void)> TRebootFunction;
        typedef std::function<void(String topic, String payload, bool qos, bool retain, int message_id)> TPublishMessageFunction;
        typedef std::function<void(String topic_filter)> TSubscribeFunction;
        typedef std::function<void(String topic_filter)> TUnsubscribeFunction;
        void init(WebServer* server,
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
