#ifndef LIGHT_H
#define LIGHT_H

#include <Arduino.h>

class Light {
private:
    int pin;    
    bool state;     

public:
    Light(int pin);
    void turnOn();
    void turnOff();
    bool getState() const;
};

#endif // LIGHT_H
