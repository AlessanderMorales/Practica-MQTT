#include "Light.h"

Light::Light(int pin) : pin(pin), state(false) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH); 
}

void Light::turnOn() {
    state = true;
    digitalWrite(pin, LOW); 
    Serial.println("Foco encendido");
}

void Light::turnOff() {
    state = false;
    digitalWrite(pin, HIGH);
    Serial.println("Foco apagado");
}

bool Light::getState() const {
    return state;
}
