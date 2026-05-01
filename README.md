# BAS Antenna Tracker

ESP32-based automatic antenna tracker for UAV long-range operations.

## Features

- Auto-tracking by GPS coordinates
- CRSF/MAVLink telemetry support
- Compass-based heading correction
- Pan-tilt servo control
- Remote configuration via serial/Wi-Fi

## Hardware

- **MCU:** ESP32
- **Servos:** Pan (GPIO12), Tilt (GPIO13)
- **Compass:** MPU6050 (I2C: 21/22)
- **Telemetry:** Serial2 (GPIO16/17)
- **Power:** 5-12V

## Installation

```bash
pio run --target upload
pio device monitor
```

## Commands

```
HOME 50.123456 8.765432 300    # Set home position
TRACK ON                         # Enable auto-tracking
TRACK OFF                        # Disable
STATUS                           # Get current angles
```

## Integration

Send UDP packets to port 4242:
```
{"lat": 50.1234, "lon": 8.5678, "alt": 150, "hdg": 45}
```

## License

MIT