#include <WiFi.h>
#include <PubSubClient.h>
#include "Light.h"      
#include "DHTSensor.h"   


const char* WIFI_SSID = "TIGO 2";
const char* WIFI_PASS = "SARAI246";


const char* MQTT_BROKER = "broker.hivemq.com";
const int MQTT_PORT = 1883;
const char* CLIENT_ID = "ESP32-DHT11";


WiFiClient wiFiClient;
PubSubClient client(wiFiClient);
Light light(4);                
DHTSensor dhtSensor(5, DHT11); 


void callback(char* topic, byte* payload, unsigned int length) {
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.print("Mensaje recibido: ");
    Serial.println(message);
    if (message == "on") {
        light.turnOn();
    } else if (message == "off") {
        light.turnOff();
    }
}

void setupWiFi() {
    Serial.println();
    Serial.print("Conectando a ");
    Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Conectado a WiFi. Dirección IP: ");
    Serial.println(WiFi.localIP());
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Intentando conectar a MQTT...");
        if (client.connect(CLIENT_ID)) {
            Serial.println("Conectado");
            client.subscribe("ucb/test/focoControl");  
        } else {
            Serial.print("Fallo, rc=");
            Serial.print(client.state());
            Serial.println(" intentando de nuevo en 5 segundos");
            delay(5000);
        }
    }
}


void publishSensorData() {
    float temperature = dhtSensor.readTemperature();
    float humidity = dhtSensor.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Error al leer el DHT11");
        return;
    }
    String tempStr = String(temperature, 2);
    String humStr = String(humidity, 2);
    client.publish("ucb/test/temperature", tempStr.c_str());
    client.publish("ucb/test/humidity", humStr.c_str());
    Serial.println("Datos publicados:");
    Serial.println("Temperatura: " + tempStr);
    Serial.println("Humedad: " + humStr);
}

void setup() {
    Serial.begin(115200);
    setupWiFi();
    client.setServer(MQTT_BROKER, MQTT_PORT);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    static unsigned long lastPublish = 0;
    if (millis() - lastPublish > 10000) { 
        lastPublish = millis();
        publishSensorData();
    }
}
