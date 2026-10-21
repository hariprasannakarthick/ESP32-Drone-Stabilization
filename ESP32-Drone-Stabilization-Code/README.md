# ESP32 + MPU6050 Sensor Monitor

This Arduino sketch is a first-stage code file for the ESP32 drone-stabilization repository.

## What it does
- Initializes the MPU6050 over I²C.
- Applies example accelerometer and gyroscope ranges.
- Estimates gyroscope bias at startup.
- Calculates roll and pitch using a complementary filter.
- Prints sensor data over Serial for calibration.

## What it does not do
This is **not flight-control firmware**. It does not generate ESC commands, mix motors, arm a drone, or implement flight failsafes. Do not describe it as a working flight controller.

## Libraries
Install these through Arduino IDE Library Manager:
- Adafruit MPU6050
- Adafruit Unified Sensor
- Adafruit BusIO

## Example wiring
- SDA -> ESP32 GPIO 21
- SCL -> ESP32 GPIO 22
- GND -> GND
- VCC/VIN -> a voltage supported by the exact breakout board

If the sensor is not detected at address `0x68`, check the wiring and whether AD0 selects `0x69`.

## Run
1. Select your exact ESP32 board in Arduino IDE.
2. Install the libraries above.
3. Open `src/MPU6050_Sensor_Monitor.ino`.
4. Upload with the drone's motors disconnected.
5. Open Serial Monitor at **115200 baud**.
6. Keep the sensor still during startup calibration, then move it slowly and check axis signs.

The filter coefficient and ranges are example settings. Verify orientation and calibrate for your hardware. The angle equations assume a particular sensor-axis convention and may need adjustment for your mounting.
