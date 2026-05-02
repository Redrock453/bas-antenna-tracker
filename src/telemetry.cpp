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
        size_t available = Serial2.available();
        size_t toRead = min(available, TELEMETRY_BUFFER_SIZE - 1);
        uint8_t buffer[TELEMETRY_BUFFER_SIZE];
        size_t len = Serial2.readBytes(buffer, toRead);

        if (len > 2) {
            if (buffer[0] == CRSF_SYNC_BYTE) {
                parseCRSF(buffer, len);
            } else if (buffer[0] == MAVLINK_SYNC_BYTE) {
                parseMAVLink(buffer, len);
            }
        }
    }
    return data.valid && !isDataStale();
}

bool TelemetryReceiver::isDataStale() const {
    if (!data.valid) return true;
    uint32_t age = millis() - data.timestamp;
    return age > TELEMETRY_TIMEOUT_MS;
}

void TelemetryReceiver::parseCRSF(const uint8_t* buffer, size_t len) {
    if (len < 10) return;

    uint8_t frameLen = buffer[1];
    uint8_t frameType = buffer[2];

    if (frameType != CRSF_FRAME_TYPE_GPS) return;
    if (len < frameLen + 2) return;

    memcpy(&data.latitude, &buffer[3], sizeof(int32_t));
    memcpy(&data.longitude, &buffer[7], sizeof(int32_t));

    data.latitude /= 10000000.0f;
    data.longitude /= 10000000.0f;

    data.altitude = buffer[11];
    data.heading = buffer[12] * 360.0f / 255.0f;
    data.timestamp = millis();
    data.valid = true;
}

void TelemetryReceiver::parseMAVLink(const uint8_t* buffer, size_t len) {
    if (len < 30) return;

    memcpy(&data.latitude, &buffer[6], sizeof(float));
    memcpy(&data.longitude, &buffer[10], sizeof(float));
    memcpy(&data.altitude, &buffer[14], sizeof(float));
    memcpy(&data.heading, &buffer[18], sizeof(float));

    data.timestamp = millis();
    data.valid = true;
}