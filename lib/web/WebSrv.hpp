/*
 *  Веб-сервер
 *  -----------------------------------------------------------------------------------------------------------------------
 *  (c) 2025 Ходак Андрей | Andrey Khodak
 *  andrey@khodak.ru | https://khodak.ru | tg: @akhodak
*/
#pragma once

#include <Arduino.h>
#include <WebServer.h>
#include <Preferences.h>
#include <esp_log.h>

#include "format.hpp"
#include "app_version.hpp"
#include "network_params.hpp"
#include "default_values.hpp"
#include <version.hpp>

#include "views/styles.tpl"
#include "views/scripts.tpl"
#include "views/index_page.tpl"
#include "views/publish_page.tpl"
#include "views/subscribe_page.tpl"
#include "views/reboot_page.tpl"
#include "views/setup_page.tpl"
#include "views/firmware_page.tpl"
#include "views/not_found_page.tpl"
#include "views/alert_page.tpl"

#define WEB_SERVER_PORT 80

extern const uint8_t binary_lib_web_bootstrap_min_css_gz_start[] asm("_binary_lib_web_bootstrap_min_css_gz_start");
extern const uint8_t binary_lib_web_bootstrap_min_css_gz_end[] asm("_binary_lib_web_bootstrap_min_css_gz_end");
extern const uint8_t binary_lib_web_bootstrap_min_js_gz_start[] asm("_binary_lib_web_bootstrap_min_js_gz_start");
extern const uint8_t binary_lib_web_bootstrap_min_js_gz_end[] asm("_binary_lib_web_bootstrap_min_js_gz_end");
extern const uint8_t binary_lib_web_favicon_ico_gz_start[] asm("_binary_lib_web_favicon_ico_gz_start");
extern const uint8_t binary_lib_web_favicon_ico_gz_end[] asm("_binary_lib_web_favicon_ico_gz_end");

class WebSrv
{
    private:
        // Тег для логирования
        const char* _logTAG = "WebServer";

        // Веб-сервер
        WebServer* _server;

        // Параметры сети
        NetworkParams* _networkParams;

        void handleStyles();
        void handleBootstrapCss();
        void handleBootstrapJs();
        void handleFavicon();
        void handleScripts();
        void handleShowIndex();
        void handleShowPublish(String message);
        void handlePublish();
        void handleShowSubscribe(String message);
        void handleSubscribe();
        void handleUnsubscribe();
        void handleShowSettings();
        void handleSaveSettings();
        void handleShowFirmwareUpdate();
        void handleFirmwareUpgrade();
        void handleReboot();
        void handleGetModuleParams();
        void handleHealthCheck();
        void handleNotFound();
        void processConditionalBlock(String& html, const String& conditionName, bool keepBlock);
        void replaceCommonTemplateVars(String& html);
        void showMessage(String& html, String message);
        void sendContentGzip(const unsigned char *content, size_t length, const char *mime_type);
        String getAppVersion();

    public:
        typedef std::function<void(void)> TOtaFunction;
        typedef std::function<String(void)> TGetAppVersionFunction;
        typedef std::function<void(void)> TRebootFunction;
        typedef std::function<void(String topic, String payload)> TPublishMessageFunction;
        typedef std::function<void(String topic_filter)> TSubscribeFunction;
        typedef std::function<void(String topic_filter)> TUnsubscribeFunction;
        void init(WebServer* server,
                NetworkParams* networkParams,
                TOtaFunction otaFunction,
                TGetAppVersionFunction getAppVersionFunction,
                TRebootFunction rebootFunction,
                TPublishMessageFunction publishMessageFunction,
                TSubscribeFunction subscribeFunction,
                TUnsubscribeFunction unsubscribeFunction);
        void loop();

    protected:
        TOtaFunction otaFunction;
        TGetAppVersionFunction getAppVersionFunction;
        TRebootFunction rebootFunction;
        TPublishMessageFunction publishMessageFunction;
        TSubscribeFunction subscribeFunction;
        TUnsubscribeFunction unsubscribeFunction;
};
