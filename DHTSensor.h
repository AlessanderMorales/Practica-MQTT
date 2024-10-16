#ifndef DHTSENSOR_H
#define DHTSENSOR_H

#include <DHT.h>

class DHTSensor {
private:
    DHT dht;

public:
    DHTSensor(int pin, int type);
    float readTemperature();
    float readHumidity();
};

#endif // DHTSENSOR_H
