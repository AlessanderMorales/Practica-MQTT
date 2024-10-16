#include <DHTSensor.h>
#include "Light.h"
#include "MQTTClientHandler.h"
#include "WiFiManager.h"

// Configuración de la red WiFi
const char* WIFI_SSID = "TIGO 2";
const char* WIFI_PASS = "SARAI246";

// Configuración del broker MQTT
const char* MQTT_BROKER = "broker.hivemq.com";
const int MQTT_PORT = 1883;
const char* CLIENT_ID = "ESP32-DHT11";

// Instanciar las clases
Light light(4);                // El foco está conectado al pin 4
DHTSensor dhtSensor(5, DHT11); // DHT11 conectado al pin 5
WiFiManager wifiManager(WIFI_SSID, WIFI_PASS);
MQTTClientHandler mqttClient(MQTT_BROKER, MQTT_PORT, CLIENT_ID);

// Función callback para procesar mensajes MQTT entrantes
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

void setup() {
    Serial.begin(115200);
    wifiManager.connect();       // Conectar a WiFi
    mqttClient.setCallback(callback);
    mqttClient.setup();          // Configurar MQTT
}

void loop() {
    mqttClient.loop();           // Ejecutar el loop de MQTT

    // Publicar datos del DHT11 cada 10 segundos
    static unsigned long lastPublish = 0;
    if (millis() - lastPublish > 10000) {  // 10 segundos
        lastPublish = millis();
        publishSensorData();
    }
}

// Publicar datos del sensor DHT11
void publishSensorData() {
    float temperature = dhtSensor.readTemperature();
    float humidity = dhtSensor.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Error al leer el DHT11");
        return;
    }

    // Publicar datos en sus respectivos tópicos
    mqttClient.publish("ucb/test/temperature", String(temperature, 2).c_str());
    mqttClient.publish("ucb/test/humidity", String(humidity, 2).c_str());

    Serial.println("Datos publicados:");
    Serial.println("Temperatura: " + String(temperature, 2));
    Serial.println("Humedad: " + String(humidity, 2));
}
