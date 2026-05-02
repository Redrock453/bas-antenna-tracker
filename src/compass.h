#ifndef COMPASS_H
#define COMPASS_H

#include <Arduino.h>

#define MPU6050_ADDRESS 0x68
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_USER_CTRL 0x6A
#define MPU6050_FIFO_EN 0x23
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_GYRO_XOUT_H 0x43
#define COMPASS_READ_DELAY_MS 10

class Compass {
public:
    Compass();
    bool begin();
    float getHeading();
    bool calibrate();
    void setOffset(float offset) { headingOffset = offset; }
    bool isInitialized() const { return initialized; }
    bool hasError() const { return errorFlag; }

private:
    float headingOffset;
    bool initialized;
    bool errorFlag;
    float readRawHeading();
    bool writeRegister(uint8_t reg, uint8_t value);
    bool readRegister(uint8_t reg, uint8_t* value);
};

#endif