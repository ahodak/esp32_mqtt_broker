/*
 *  MQTT-сервер
 *  -----------------------------------------------------------------------------------------------------------------------
 *  (c) 2025 Ходак Андрей | Andrey Khodak
 *  andrey@khodak.ru | https://khodak.ru | tg: @akhodak
*/
#pragma once

#include <esp_log.h>
#include <PicoMQTT.h>
#include <default_values.hpp>

#define MQTT_BROKER_PORT 1883

class MQTT : public PicoMQTT::Server {
    private:
        const char* _logTAG = "MQTT";

        const char* _username;
        const char* _password;

        static std::unique_ptr<PicoMQTT::ServerSocketInterface> createServer(uint16_t port, const char* serverType);

    public:
        MQTT(uint16_t port, const char* serverType);

        void init(const char* username, const char* password);

        void username(const char* newUsername);
        void password(const char* newPassword);

    protected:
        PicoMQTT::ConnectReturnCode auth(const char* client_id, const char* username, const char* password) override;
};
