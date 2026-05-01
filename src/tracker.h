#ifndef TRACKER_H
#define TRACKER_H

#include <Arduino.h>
#include "telemetry.h"
#include "compass.h"
#include "servos.h"

struct Position {
    float lat;
    float lon;
    float alt;
};

class AntennaTracker {
public:
    AntennaTracker();
    void begin();
    void update();
    void setHomePosition(float lat, float lon, float alt);
    void enableTracking(bool enable) { trackingEnabled = enable; }
    bool isTracking() const { return trackingEnabled; }
    void setAzimuthOffset(float offset);
    void setStepSize(float degrees) { stepSize = degrees; }

    float getTargetAzimuth() const { return targetAzimuth; }
    float getTargetElevation() const { return targetElevation; }

private:
    TelemetryReceiver telemetry;
    Compass compass;
    ServoController servos;

    Position home;
    Position drone;

    float targetAzimuth;
    float targetElevation;
    float stepSize;

    bool trackingEnabled;
    bool homeSet;

    void calculateAngles();
    float haversineAzimuth(float lat1, float lon1, float lat2, float lon2);
    float calculateElevation(float dist, float altDiff);
};

#endif