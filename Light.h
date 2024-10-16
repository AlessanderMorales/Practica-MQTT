#ifndef LIGHT_H
#define LIGHT_H

#include <Arduino.h>

class Light {
private:
    int pin;        // Pin del relé
    bool state;     // Estado del foco

public:
    Light(int pin);
    void turnOn();
    void turnOff();
    bool getState() const;
};

#endif // LIGHT_H
