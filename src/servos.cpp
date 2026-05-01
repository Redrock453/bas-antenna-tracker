#include "servos.h"

ServoController::ServoController()
    : currentPan(90), currentTilt(0), panMin(0), panMax(180), tiltMin(0), tiltMax(90) {}

void ServoController::begin() {
    panServo.setPeriodHertz(50);
    tiltServo.setPeriodHertz(50);
    panServo.attach(12, 500, 2500);
    tiltServo.attach(13, 500, 2500);
    setPanAngle(90);
    setTiltAngle(0);
}

void ServoController::setPanLimits(float min, float max) {
    panMin = min;
    panMax = max;
}

void ServoController::setTiltLimits(float min, float max) {
    tiltMin = min;
    tiltMax = max;
}

void ServoController::setPanAngle(float angle) {
    angle = constrain(angle, panMin, panMax);
    currentPan = angle;
    panServo.write(angle);
}

void ServoController::setTiltAngle(float angle) {
    angle = constrain(angle, tiltMin, tiltMax);
    currentTilt = angle;
    tiltServo.write(angle);
}