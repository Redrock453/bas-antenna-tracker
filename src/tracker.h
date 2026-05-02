#ifndef TRACKER_H
#define TRACKER_H

#include <Arduino.h>
#include "telemetry.h"
#include "compass.h"
#include "servos.h"

#define EARTH_RADIUS_M 6371000.0f
#define MIN_DISTANCE_M 1.0f
#define MAX_LAT 90.0f
#define MIN_LAT -90.0f
#define MAX_LON 180.0f
#define MIN_LON -180.0f
#define MIN_ALT -1000.0f
#define MAX_ALT 50000.0f

struct Position {
    float lat;
    float lon;
    float alt;
    Position(float l = 0, float ln = 0, float a = 0) : lat(l), lon(ln), alt(a) {}
};

class AntennaTracker {
public:
    AntennaTracker();
    bool begin();
    void update();
    bool setHomePosition(float lat, float lon, float alt);
    void enableTracking(bool enable) { trackingEnabled = enable; }
    bool isTracking() const { return trackingEnabled; }
    void setAzimuthOffset(float offset);
    void setStepSize(float degrees) { stepSize = degrees; }

    float getTargetAzimuth() const { return targetAzimuth; }
    float getTargetElevation() const { return targetElevation; }
    bool isHomeSet() const { return homeSet; }
    bool hasCompassError() const { return compass.hasError(); }
    bool isTelemetryStale() const { return telemetry.isDataStale(); }

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
    bool validateCoordinates(float lat, float lon, float alt);
};

#endif