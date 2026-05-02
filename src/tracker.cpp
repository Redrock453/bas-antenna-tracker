#include "tracker.h"
#include <math.h>

AntennaTracker::AntennaTracker()
    : targetAzimuth(0), targetElevation(0),
      stepSize(1.0f), trackingEnabled(false), homeSet(false) {}

bool AntennaTracker::begin() {
    telemetry.begin();

    if (!compass.begin()) {
        return false;
    }

    servos.begin();
    servos.setPanLimits(0, 180);
    servos.setTiltLimits(0, 90);
    return true;
}

bool AntennaTracker::setHomePosition(float lat, float lon, float alt) {
    if (!validateCoordinates(lat, lon, alt)) {
        return false;
    }

    home = Position(lat, lon, alt);
    homeSet = true;
    return true;
}

bool AntennaTracker::validateCoordinates(float lat, float lon, float alt) {
    if (lat < MIN_LAT || lat > MAX_LAT) return false;
    if (lon < MIN_LON || lon > MAX_LON) return false;
    if (alt < MIN_ALT || alt > MAX_ALT) return false;
    if (!isfinite(lat) || !isfinite(lon) || !isfinite(alt)) return false;
    return true;
}

void AntennaTracker::update() {
    if (!homeSet) return;

    if (telemetry.update()) {
        drone = Position(
            telemetry.getData().latitude,
            telemetry.getData().longitude,
            telemetry.getData().altitude
        );

        if (validateCoordinates(drone.lat, drone.lon, drone.alt)) {
            calculateAngles();

            if (trackingEnabled) {
                servos.setPanAngle(targetAzimuth);
                servos.setTiltAngle(targetElevation);
            }
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

    float dLat = (drone.lat - home.lat) * PI / 180.0f;
    float dLon = (drone.lon - home.lon) * PI / 180.0f;
    float lat1 = home.lat * PI / 180.0f;
    float lat2 = drone.lat * PI / 180.0f;

    float sinDLat = sin(dLat / 2.0f);
    float sinDLon = sin(dLon / 2.0f);

    float a = sinDLat * sinDLat + cos(lat1) * cos(lat2) * sinDLon * sinDLon;
    float c = 2.0f * atan2(sqrt(a), sqrt(1.0f - a));
    float distance = EARTH_RADIUS_M * c;

    float altDiff = drone.alt - home.alt;
    targetElevation = calculateElevation(distance, altDiff);
}

float AntennaTracker::haversineAzimuth(float lat1, float lon1, float lat2, float lon2) {
    float dLon = (lon2 - lon1) * PI / 180.0f;
    lat1 *= PI / 180.0f;
    lat2 *= PI / 180.0f;

    float x = sin(dLon) * cos(lat2);
    float y = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);

    float azimuth = atan2(x, y) * 180.0f / PI;
    if (azimuth < 0) azimuth += 360.0f;
    return azimuth;
}

float AntennaTracker::calculateElevation(float dist, float altDiff) {
    if (dist < MIN_DISTANCE_M) return 0.0f;
    float elevation = atan2(altDiff, dist) * 180.0f / PI;
    return constrain(elevation, 0.0f, 90.0f);
}