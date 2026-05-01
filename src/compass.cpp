#include "compass.h"
#include <Wire.h>

Compass::Compass() : headingOffset(0.0f) {}

void Compass::begin() {
    Wire.begin(21, 22);
    Wire.beginTransmission(0x68);
    Wire.write(0x37);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.beginTransmission(0x68);
    Wire.write(0x6B);
    Wire.write(0x00);
    Wire.endTransmission();
}

float Compass::getHeading() {
    float raw = readRawHeading();
    float corrected = raw + headingOffset;
    if (corrected < 0) corrected += 360;
    if (corrected >= 360) corrected -= 360;
    return corrected;
}

float Compass::readRawHeading() {
    Wire.beginTransmission(0x68);
    Wire.write(0x3C);
    Wire.endTransmission(false);
    Wire.requestFrom(0x68, 6, true);

    int16_t mx = (Wire.read() << 8 | Wire.read());
    int16_t my = (Wire.read() << 8 | Wire.read());
    int16_t mz = (Wire.read() << 8 | Wire.read());

    float heading = atan2(my, mx) * 180.0 / PI;
    if (heading < 0) heading += 360;
    return heading;
}

void Compass::calibrate() {
    int16_t minX = 32767, maxX = -32768;
    int16_t minY = 32767, maxY = -32768;

    for (int i = 0; i < 1000; i++) {
        Wire.beginTransmission(0x68);
        Wire.write(0x3C);
        Wire.endTransmission(false);
        Wire.requestFrom(0x68, 6, true);

        int16_t mx = Wire.read() << 8 | Wire.read();
        int16_t my = Wire.read() << 8 | Wire.read();
        Wire.read(); Wire.read();

        if (mx < minX) minX = mx;
        if (mx > maxX) maxX = mx;
        if (my < minY) minY = my;
        if (my > maxY) maxY = my;

        delay(10);
    }
}