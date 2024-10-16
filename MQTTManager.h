#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <PubSubClient.h>
#include <WiFi.h>

class MQTTManager {
private:
    PubSubClient client;
    const char* broker;
    int port;
    const char* clientId;

public:
    MQTTManager(WiFiClient& wiFiClient, const char* broker, int port, const char* clientId);
    void connect();
    void subscribe(const char* topic);
    void loop();
    void setCallback(void (*callback)(char*, byte*, unsigned int));
};

#endif // MQTTMANAGER_H
