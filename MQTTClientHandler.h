#ifndef MQTTCLIENTHANDLER_H
#define MQTTCLIENTHANDLER_H

#include <PubSubClient.h>
#include <WiFi.h>

class MQTTClientHandler {
private:
    WiFiClient wiFiClient;
    PubSubClient client;
    const char* broker;
    const int port;
    const char* clientId;

public:
    MQTTClientHandler(const char* broker, int port, const char* clientId);
    void setup();
    void loop();
    void reconnect();
    void subscribe(const char* topic);
    void publish(const char* topic, const char* message);
    void setCallback(MQTTCallback callback);
};

#endif // MQTTCLIENTHANDLER_H
