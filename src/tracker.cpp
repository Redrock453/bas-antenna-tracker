#include "tracker.h"
#include <math.h>

AntennaTracker::AntennaTracker()
    : home{0, 0, 0}, drone{0, 0, 0}, targetAzimuth(0), targetElevation(0),
      stepSize(1.0f), trackingEnabled(false), homeSet(false) {}

void AntennaTracker::begin() {
    telemetry.begin();
    compass.begin();
    servos.begin();
    servos.setPanLimits(0, 180);
    servos.setTiltLimits(0, 90);
}

void AntennaTracker::setHomePosition(float lat, float lon, float alt) {
    home = {lat, lon, alt};
    homeSet = true;
}

void AntennaTracker::update() {
    if (!homeSet) return;

    if (telemetry.update()) {
        drone = {telemetry.getData().latitude, telemetry.getData().longitude, telemetry.getData().altitude};
        calculateAngles();

        if (trackingEnabled) {
            servos.setPanAngle(targetAzimuth);
            servos.setTiltAngle(targetElevation);
        }
    }
}

void AntennaTracker::setAzimuthOffset(float offset) {
    compass.setOffset(offset);
}

void AntennaTracker::calculateAngles() {
    targetAzimuth = haversineAzimuth(home.lat, home.lon, drone.lat, drone.lon);
    float heading = compass.getHeading();
    float relativeAzimuth = targetAzimuth - heading;

    if (relativeAzimuth < 0) relativeAzimuth += 360;
    if (relativeAzimuth > 180) relativeAzimuth -= 360;

    targetAzimuth = 90 + relativeAzimuth;

    float R = 6371000.0;
    float dLat = (drone.lat - home.lat) * PI / 180;
    float dLon = (drone.lon - home.lon) * PI / 180;
    float lat1 = home.lat * PI / 180;
    float lat2 = drone.lat * PI / 180;

    float a = sin(dLat/2) * sin(dLat/2) +
              cos(lat1) * cos(lat2) * sin(dLon/2) * sin(dLon/2);
    float c = 2 * atan2(sqrt(a), sqrt(1-a));
    float distance = R * c;

    float altDiff = drone.alt - home.alt;
    targetElevation = calculateElevation(distance, altDiff);
}

float AntennaTracker::haversineAzimuth(float lat1, float lon1, float lat2, float lon2) {
    float dLon = (lon2 - lon1) * PI / 180;
    lat1 *= PI / 180;
    lat2 *= PI / 180;

    float x = sin(dLon) * cos(lat2);
    float y = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);

    float azimuth = atan2(x, y) * 180 / PI;
    if (azimuth < 0) azimuth += 360;
    return azimuth;
}

float AntennaTracker::calculateElevation(float dist, float altDiff) {
    if (dist < 1.0f) return 0.0f;
    float elevation = atan2(altDiff, dist) * 180 / PI;
    return constrain(elevation, 0, 90);
}