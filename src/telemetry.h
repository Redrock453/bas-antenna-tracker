#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>

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

private:
    TelemetryData data;
    void parseCRSF(const uint8_t* buffer, size_t len);
    void parseMAVLink(const uint8_t* buffer, size_t len);
};

#endif