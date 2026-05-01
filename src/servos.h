#ifndef SERVOS_H
#define SERVOS_H

#include <Arduino.h>
#include <ESP32Servo.h>

class ServoController {
public:
    ServoController();
    void begin();
    void setPanAngle(float angle);
    void setTiltAngle(float angle);
    void setPanLimits(float min, float max);
    void setTiltLimits(float min, float max);
    float getPanAngle() const { return currentPan; }
    float getTiltAngle() const { return currentTilt; }

private:
    Servo panServo;
    Servo tiltServo;
    float currentPan;
    float currentTilt;
    float panMin, panMax;
    float tiltMin, tiltMax;
};

#endif