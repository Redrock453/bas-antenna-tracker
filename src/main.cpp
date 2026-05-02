#include <Arduino.h>
#include "tracker.h"
#include <string.h>

AntennaTracker tracker;

void setup() {
    Serial.begin(115200);
    Serial.println("BAS Antenna Tracker v1.1");
    Serial.println("Type HELP for commands");

    if (!tracker.begin()) {
        Serial.println("ERROR: Tracker initialization failed!");
        Serial.println("Check I2C connection and restart");
    } else {
        Serial.println("Tracker initialized successfully");
    }

    tracker.enableTracking(true);
}

void printHelp() {
    Serial.println("\n--- Commands ---");
    Serial.println("HOME <lat> <lon> <alt>  - Set home position");
    Serial.println("TRACK ON/OFF             - Enable/disable tracking");
    Serial.println("STATUS                    - Show current status");
    Serial.println("OFFSET <degrees>          - Set compass offset");
    Serial.println("CALIBRATE                 - Calibrate compass");
    Serial.println("HELP                      - Show this help");
}

bool parseHomeCommand(const String& cmd, float& lat, float& lon, float& alt) {
    int firstSpace = cmd.indexOf(' ');
    if (firstSpace == -1) return false;

    String params = cmd.substring(firstSpace + 1);
    params.trim();

    int space1 = params.indexOf(' ');
    int space2 = params.indexOf(' ', space1 + 1);

    if (space1 == -1 || space2 == -1) return false;

    String latStr = params.substring(0, space1);
    String lonStr = params.substring(space1 + 1, space2);
    String altStr = params.substring(space2 + 1);

    lat = latStr.toFloat();
    lon = lonStr.toFloat();
    alt = altStr.toFloat();

    return (lat != 0 || lon != 0) && altStr.length() > 0;
}

void loop() {
    tracker.update();

    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        cmd.toUpperCase();

        if (cmd == "HELP" || cmd == "?") {
            printHelp();
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
            Serial.println("\n--- Status ---");
            Serial.printf("Home set: %s\n", tracker.isHomeSet() ? "YES" : "NO");
            Serial.printf("Tracking: %s\n", tracker.isTracking() ? "ON" : "OFF");
            Serial.printf("Compass error: %s\n", tracker.hasCompassError() ? "YES" : "NO");
            Serial.printf("Telemetry stale: %s\n", tracker.isTelemetryStale() ? "YES" : "NO");
            Serial.printf("Target Azimuth: %.1f\n", tracker.getTargetAzimuth());
            Serial.printf("Target Elevation: %.1f\n", tracker.getTargetElevation());
        }
        else if (cmd == "CALIBRATE") {
            Serial.println("Calibrating compass... Rotate device slowly");
            if (tracker.hasCompassError()) {
                Serial.println("ERROR: Compass not responding!");
            } else {
                Serial.println("Calibration not implemented (needs compass library)");
            }
        }
        else if (cmd.startsWith("OFFSET ")) {
            float offset = cmd.substring(7).toFloat();
            tracker.setAzimuthOffset(offset);
            Serial.printf("Compass offset set to %.1f\n", offset);
        }
        else if (cmd.startsWith("HOME ")) {
            float lat, lon, alt;
            if (parseHomeCommand(cmd, lat, lon, alt)) {
                if (tracker.setHomePosition(lat, lon, alt)) {
                    Serial.printf("Home set: %.6f, %.6f, %.1fm\n", lat, lon, alt);
                } else {
                    Serial.println("ERROR: Invalid GPS coordinates");
                    Serial.println("Valid ranges: Lat [-90,90], Lon [-180,180], Alt [-1000,50000]");
                }
            } else {
                Serial.println("ERROR: Invalid HOME format");
                Serial.println("Usage: HOME 50.123456 8.765432 300");
            }
        }
        else if (cmd.length() > 0) {
            Serial.println("Unknown command. Type HELP");
        }
    }

    delay(10);
}