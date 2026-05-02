#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>

#define TELEMETRY_BUFFER_SIZE 256
#define TELEMETRY_TIMEOUT_MS 5000
#define CRSF_FRAME_TYPE_GPS 0x16
#define CRSF_SYNC_BYTE 0xC8
#define MAVLINK_SYNC_BYTE 0xFE

struct TelemetryData {
    float latitude;
    float longitude;
    float altitude;
    float heading;
    uint32_t timestamp;
    bool valid;
};

class TelemetryReceiver {
public:
    TelemetryReceiver();
    void begin();
    bool update();
    TelemetryData getData() const { return data; }
    bool isDataStale() const;

private:
    TelemetryData data;
    void parseCRSF(const uint8_t* buffer, size_t len);
    void parseMAVLink(const uint8_t* buffer, size_t len);
    uint8_t calculateCRSFCRC(const uint8_t* buffer, size_t len);
};

#endif