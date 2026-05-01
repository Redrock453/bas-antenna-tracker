#include <Arduino.h>
#include "tracker.h"

AntennaTracker tracker;

void setup() {
    Serial.begin(115200);
    Serial.println("BAS Antenna Tracker v1.0");

    tracker.begin();

    // Home position - задать через serial или hardcode
    // tracker.setHomePosition(50.1234, 8.5678, 300);
    tracker.enableTracking(true);

    Serial.println("Tracker initialized");
}

void loop() {
    tracker.update();

    // Serial commands for control
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();

        if (cmd.startsWith("HOME")) {
            float lat = cmd.substring(5, 15).toFloat();
            float lon = cmd.substring(16, 26).toFloat();
            float alt = cmd.substring(27).toFloat();
            tracker.setHomePosition(lat, lon, alt);
            Serial.printf("Home set: %.6f, %.6f, %.1fm\n", lat, lon, alt);
        }
        else if (cmd == "TRACK ON") {
            tracker.enableTracking(true);
            Serial.println("Tracking enabled");
        }
        else if (cmd == "TRACK OFF") {
            tracker.enableTracking(false);
            Serial.println("Tracking disabled");
        }
        else if (cmd == "STATUS") {
            Serial.printf("Az: %.1f, El: %.1f, Track: %s\n",
                tracker.getTargetAzimuth(),
                tracker.getTargetElevation(),
                tracker.isTracking() ? "ON" : "OFF");
        }
    }

    delay(10);
}