#ifndef COMPASS_H
#define COMPASS_H

#include <Arduino.h>

class Compass {
public:
    Compass();
    void begin();
    float getHeading();
    void calibrate();
    void setOffset(float offset) { headingOffset = offset; }

private:
    float headingOffset;
    float readRawHeading();
};

#endif