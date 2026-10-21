# Firmware Folder

Place the actual ESP32 firmware source files here.

Suggested modules:
- `DroneStabilization.ino` — setup and top-level scheduling
- `imu.cpp` / `imu.h` — MPU6050 reads and calibration
- `attitude.cpp` / `attitude.h` — complementary filter
- `pid.cpp` / `pid.h` — PID calculation and anti-windup
- `motor_mixer.cpp` — motor mixing and output limits
- `safety.cpp` — arming, disarming, sensor timeout, and failsafe handling

These are suggested names only; no flight-ready firmware is included in this starter package. Test with propellers removed and validate the control signs and failsafe before flight.
