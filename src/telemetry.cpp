#include "telemetry.h"
#include <cstring>

TelemetryReceiver::TelemetryReceiver() {
    data = {0, 0, 0, 0, 0, false};
}

void TelemetryReceiver::begin() {
    Serial2.begin(115200, SERIAL_8N1, 16, 17);
}

bool TelemetryReceiver::update() {
    if (Serial2.available()) {
        uint8_t buffer[256];
        size_t len = Serial2.readBytes(buffer, min(Serial2.available(), 256));

        if (len > 2) {
            if (buffer[0] == 0xC8) {
                parseCRSF(buffer, len);
            } else if (buffer[0] == 0xFE) {
                parseMAVLink(buffer, len);
            }
        }
    }
    return data.valid;
}

void TelemetryReceiver::parseCRSF(const uint8_t* buffer, size_t len) {
    if (len < 10 || buffer[1] != 0x16) return;

    data.latitude = *((int32_t*)&buffer[2]) / 10000000.0f;
    data.longitude = *((int32_t*)&buffer[6]) / 10000000.0f;
    data.altitude = buffer[10];
    data.heading = buffer[11] * 360.0f / 255.0f;
    data.timestamp = millis();
    data.valid = true;
}

void TelemetryReceiver::parseMAVLink(const uint8_t* buffer, size_t len) {
    if (len < 30) return;
    data.latitude = *((float*)&buffer[6]);
    data.longitude = *((float*)&buffer[10]);
    data.altitude = *((float*)&buffer[14]);
    data.heading = *((float*)&buffer[18]);
    data.timestamp = millis();
    data.valid = true;
}