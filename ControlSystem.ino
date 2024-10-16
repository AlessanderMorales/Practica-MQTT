#include <Arduino.h>
#include "Light.h"      
#include "DHTSensor.h"   
#include "WiFiManager.h"
#include "MQTTManager.h"

const char* WIFI_SSID = "TIGO 2";
const char* WIFI_PASS = "SARAI246";
const char* MQTT_BROKER = "broker.hivemq.com";
const int MQTT_PORT = 1883;
const char* CLIENT_ID = "ESP32-DHT11";

Light light(4);                
DHTSensor dhtSensor(5, DHT11);  

WiFiManager wifiManager(WIFI_SSID, WIFI_PASS);
WiFiClient wiFiClient;
MQTTManager mqttManager(wiFiClient, MQTT_BROKER, MQTT_PORT, CLIENT_ID);

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

void publishSensorData() {
    float temperature = dhtSensor.readTemperature();
    float humidity = dhtSensor.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Error al leer el DHT11");
        return;
    }
    String tempStr = String(temperature, 2);
    String humStr = String(humidity, 2);
    mqttManager.client.publish("ucb/test/temperature", tempStr.c_str());
    mqttManager.client.publish("ucb/test/humidity", humStr.c_str());
    Serial.println("Datos publicados:");
    Serial.println("Temperatura: " + tempStr);
    Serial.println("Humedad: " + humStr);
}

void setup() {
    Serial.begin(115200);
    wifiManager.connect();
    mqttManager.setCallback(callback);
}

void loop() {
    if (!mqttManager.client.connected()) {
        mqttManager.connect();
        mqttManager.subscribe("ucb/test/focoControl");  
    }
    mqttManager.loop();
    
    static unsigned long lastPublish = 0;
    if (millis() - lastPublish > 10000) { 
        lastPublish = millis();
        publishSensorData();
    }
}
