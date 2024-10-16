#include "MQTTClientHandler.h"

MQTTClientHandler::MQTTClientHandler(const char* broker, int port, const char* clientId)
    : broker(broker), port(port), client(wiFiClient) {
    client.setServer(broker, port);
}

void MQTTClientHandler::setup() {
    // Configurar el cliente MQTT
}

void MQTTClientHandler::loop() {
    client.loop();
}

void MQTTClientHandler::reconnect() {
    while (!client.connected()) {
        Serial.print("Intentando conectar a MQTT...");
        if (client.connect(clientId)) {
            Serial.println("Conectado");
        } else {
            Serial.print("Fallo, rc=");
            Serial.print(client.state());
            Serial.println(" intentando de nuevo en 5 segundos");
            delay(5000);
        }
    }
}

void MQTTClientHandler::subscribe(const char* topic) {
    client.subscribe(topic);
}

void MQTTClientHandler::publish(const char* topic, const char* message) {
    client.publish(topic, message);
}

void MQTTClientHandler::setCallback(MQTTCallback callback) {
    client.setCallback(callback);
}
