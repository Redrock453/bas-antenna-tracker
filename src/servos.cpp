#include "servos.h"

ServoController::ServoController()
    : currentPan(90), currentTilt(0), targetPan(90), targetTilt(0),
      panMin(0), panMax(180), tiltMin(0), tiltMax(90) {}

void ServoController::begin() {
    panServo.setPeriodHertz(SERVO_HZ);
    tiltServo.setPeriodHertz(SERVO_HZ);
    panServo.attach(SERVO_PAN_PIN, SERVO_MIN_US, SERVO_MAX_US);
    tiltServo.attach(SERVO_TILT_PIN, SERVO_MIN_US, SERVO_MAX_US);
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
    targetPan = angle;

    currentPan += (targetPan - currentPan) * SERVO_SMOOTHING_FACTOR;
    panServo.write(currentPan);
}

void ServoController::setTiltAngle(float angle) {
    angle = constrain(angle, tiltMin, tiltMax);
    targetTilt = angle;

    currentTilt += (targetTilt - currentTilt) * SERVO_SMOOTHING_FACTOR;
    tiltServo.write(currentTilt);
}