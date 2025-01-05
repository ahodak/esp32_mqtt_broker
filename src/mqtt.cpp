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

void MQTT::username(const char* newUsername) {
    _username = newUsername;
}

void MQTT::password(const char* newPassword) {
    _password = newPassword;
}

PicoMQTT::ConnectReturnCode MQTT::auth(const char * client_id, const char * username, const char * password) {
    Serial.println("MQTT authentication. ClientId: " + String(client_id) + " Username: " + String(username) + " Password: " + String(password));
    // only accept client IDs which are 3 chars or longer
    if (String(client_id).length() < 3) {    // client_id is never NULL
        Serial.println("MQTT authentication rejected. ClientId length < 3");
        return PicoMQTT::CRC_IDENTIFIER_REJECTED;
    }

    // only accept connections if username and password are provided
    if (!username || !password) {  // username and password can be NULL
        // no username or password supplied
        Serial.println("MQTT authentication rejected. No username or password supplied");
        return PicoMQTT::CRC_NOT_AUTHORIZED;
    }

    // accept two user/password combinations
    if ((String(username) == _username) && (String(password) == _password)) {
        Serial.println("MQTT authentication accepted");
        return PicoMQTT::CRC_ACCEPTED;
    }

    // reject all other credentials
    Serial.println("MQTT authentication rejected. Wrong username or password");
    return PicoMQTT::CRC_BAD_USERNAME_OR_PASSWORD;
}
