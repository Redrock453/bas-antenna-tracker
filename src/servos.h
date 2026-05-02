#ifndef SERVOS_H
#define SERVOS_H

#include <Arduino.h>
#include <ESP32Servo.h>

#define SERVO_PAN_PIN 12
#define SERVO_TILT_PIN 13
#define SERVO_MIN_US 500
#define SERVO_MAX_US 2500
#define SERVO_HZ 50
#define SERVO_SMOOTHING_FACTOR 0.3f

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
    float targetPan;
    float targetTilt;
    float panMin, panMax;
    float tiltMin, tiltMax;
};

#endif