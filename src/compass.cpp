#include "compass.h"
#include <Wire.h>

Compass::Compass() : headingOffset(0.0f), initialized(false), errorFlag(false) {}

bool Compass::begin() {
    Wire.begin(21, 22);

    if (!writeRegister(MPU6050_PWR_MGMT_1, 0x00)) {
        errorFlag = true;
        return false;
    }

    initialized = true;
    errorFlag = false;
    return true;
}

float Compass::getHeading() {
    if (!initialized || errorFlag) {
        return 0.0f;
    }

    float raw = readRawHeading();
    float corrected = raw + headingOffset;

    if (corrected < 0) corrected += 360;
    if (corrected >= 360) corrected -= 360;
    return corrected;
}

float Compass::readRawHeading() {
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(MPU6050_ACCEL_XOUT_H);
    Wire.endTransmission(false);

    uint8_t bytesAvailable = Wire.requestFrom(MPU6050_ADDRESS, 6, true);
    if (bytesAvailable != 6) {
        errorFlag = true;
        return 0.0f;
    }

    int16_t mx = (Wire.read() << 8 | Wire.read());
    int16_t my = (Wire.read() << 8 | Wire.read());
    int16_t mz = (Wire.read() << 8 | Wire.read());

    if (mx == 0 && my == 0) {
        return 0.0f;
    }

    float heading = atan2(my, mx) * 180.0 / PI;
    if (heading < 0) heading += 360;
    return heading;
}

bool Compass::calibrate() {
    if (!initialized) return false;

    int16_t minX = 32767, maxX = -32768;
    int16_t minY = 32767, maxY = -32768;

    for (int i = 0; i < 1000; i++) {
        Wire.beginTransmission(MPU6050_ADDRESS);
        Wire.write(MPU6050_ACCEL_XOUT_H);
        Wire.endTransmission(false);

        uint8_t bytesAvailable = Wire.requestFrom(MPU6050_ADDRESS, 6, true);
        if (bytesAvailable != 6) {
            errorFlag = true;
            return false;
        }

        int16_t mx = Wire.read() << 8 | Wire.read();
        int16_t my = Wire.read() << 8 | Wire.read();
        Wire.read(); Wire.read();

        if (mx < minX) minX = mx;
        if (mx > maxX) maxX = mx;
        if (my < minY) minY = my;
        if (my > maxY) maxY = my;

        delay(COMPASS_READ_DELAY_MS);
    }

    errorFlag = false;
    return true;
}

bool Compass::writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    uint8_t status = Wire.endTransmission();
    return status == 0;
}

bool Compass::readRegister(uint8_t reg, uint8_t* value) {
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0) return false;

    uint8_t bytesAvailable = Wire.requestFrom(MPU6050_ADDRESS, 1, true);
    if (bytesAvailable != 1) return false;

    *value = Wire.read();
    return true;
}