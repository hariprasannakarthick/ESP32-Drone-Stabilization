# MPU6050 Calibration Checklist

## Setup

- Secure the IMU to the frame in its intended orientation.
- Keep the aircraft completely stationary during gyro-bias calibration.
- Verify the I2C address; common values are `0x68` or `0x69`, depending on AD0.
- Confirm configured full-scale ranges before converting raw readings.

## Checks

- [ ] Sensor is detected reliably.
- [ ] Raw acceleration values are plausible at rest.
- [ ] Gyroscope readings are near zero after bias correction while stationary.
- [ ] Each axis responds in the expected direction when the frame is moved by hand.
- [ ] Roll and pitch estimates match the chosen coordinate convention.
- [ ] Filter output is stable at rest.
- [ ] Sensor disconnect / invalid-read behavior is handled safely.
- [ ] Sensor readings and timing are logged.

## Conversion Examples

For an accelerometer configured to ±2 g, nominal sensitivity is 16384 LSB/g.
For a gyroscope configured to ±250 degrees/second, nominal sensitivity is 131 LSB/(degree/second).

These divisors change with the configured range. Verify the MPU6050 datasheet and the actual firmware settings before using them.
