#include "mqtt.hpp"

std::unique_ptr<PicoMQTT::ServerSocketInterface> MQTT::createServer(uint16_t port, const char* serverType) {
    if (strcmp(serverType, "WiFiServer") == 0) {
        return std::make_unique<PicoMQTT::ServerSocket<::WiFiServer>>(port);
    }
    // Добавьте другие типы серверов, если необходимо
    // else if (strcmp(serverType, "ДругойТипСервера") == 0) {
    //     return std::make_unique<PicoMQTT::ServerSocket<ДругойТипСервера>>(port);
    // }
    else {
        // По умолчанию используем WiFiServer
        return std::make_unique<PicoMQTT::ServerSocket<::WiFiServer>>(port);
    }
}

MQTT::MQTT(uint16_t port, const char* serverType)
    : PicoMQTT::Server(createServer(port, serverType)), _username("admin"), _password("admin") {}

void MQTT::init(PicoMQTT::Server* server, const char* username, const char* password) {
    this->username(username);
    this->password(password);
    ESP_LOGD(_logTAG, "Server initialized");
    server->begin();
    ESP_LOGD(_logTAG, "Server started");
}

void MQTT::username(const char* newUsername) {
    _username = newUsername;
    ESP_LOGD(_logTAG, "Username set to %s", newUsername);
}

void MQTT::password(const char* newPassword) {
    _password = newPassword;
    ESP_LOGD(_logTAG, "Password set to %s", newPassword);
}

PicoMQTT::ConnectReturnCode MQTT::auth(const char * client_id, const char * username, const char * password) {
    ESP_LOGI(_logTAG, "Authentication. ClientId: %s Username: %s Password: %s", client_id, username, password);
    // only accept client IDs which are 3 chars or longer
    if (String(client_id).length() < 3) {    // client_id is never NULL
        ESP_LOGE(_logTAG, "Authentication rejected. ClientId length < 3");
        return PicoMQTT::CRC_IDENTIFIER_REJECTED;
    }

    if (String(_username).isEmpty()) {
        ESP_LOGI(_logTAG, "Authentication disabled. Admin username is not set");
        return PicoMQTT::CRC_ACCEPTED;
    }

    // Принимаем подключения только если username и password предоставлены
    if (!username || !password) {  // username и password могут быть NULL
        // Не предоставлены username или password
        ESP_LOGE(_logTAG, "Authentication rejected. No username or password supplied");
        return PicoMQTT::CRC_NOT_AUTHORIZED;
    }

    // Проверяем комбинацию username/password
    if ((String(username) == _username) && (String(password) == _password)) {
        ESP_LOGI(_logTAG, "Authentication accepted");
        return PicoMQTT::CRC_ACCEPTED;
    }

    // Отклоняем все остальные учетные данные
    ESP_LOGE(_logTAG, "Authentication rejected. Wrong username or password");
    return PicoMQTT::CRC_BAD_USERNAME_OR_PASSWORD;
}
