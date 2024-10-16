#include "MQTTManager.h"
#include <Arduino.h>

MQTTManager::MQTTManager(WiFiClient& wiFiClient, const char* broker, int port, const char* clientId)
    : client(wiFiClient), broker(broker), port(port), clientId(clientId) {
    client.setServer(broker, port);
}

void MQTTManager::connect() {
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

void MQTTManager::subscribe(const char* topic) {
    client.subscribe(topic);
}

void MQTTManager::loop() {
    client.loop();
}

void MQTTManager::setCallback(void (*callback)(char*, byte*, unsigned int)) {
    client.setCallback(callback);
}
