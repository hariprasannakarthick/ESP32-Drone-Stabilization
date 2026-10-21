/*
  ESP32 + MPU6050 Sensor Monitor and Complementary Filter
  -------------------------------------------------------
  Purpose:
    - Verify ESP32/MPU6050 wiring
    - Read accelerometer and gyroscope values
    - Estimate roll and pitch using a complementary filter
    - Print measurements for calibration and debugging

  This is a SENSOR-TEST sketch only. It does NOT control ESCs or motors
  and is NOT flight-control firmware.

  Libraries:
    - Adafruit MPU6050
    - Adafruit Unified Sensor
    - Adafruit BusIO

  Example wiring for a typical ESP32 DevKit:
    MPU6050 SDA -> GPIO 21
    MPU6050 SCL -> GPIO 22
    MPU6050 GND -> GND
    MPU6050 VIN/VCC -> only a voltage supported by your breakout board

  Verify the exact board and module pinout before powering.
*/

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

Adafruit_MPU6050 mpu;

constexpr int SDA_PIN = 21;
constexpr int SCL_PIN = 22;
constexpr uint32_t SERIAL_BAUD = 115200;

// Candidate filter coefficient; validate against the actual sampling rate.
constexpr float FILTER_ALPHA = 0.98f;

float rollDeg = 0.0f;
float pitchDeg = 0.0f;
float gyroBiasX = 0.0f;
float gyroBiasY = 0.0f;
float gyroBiasZ = 0.0f;

uint32_t lastMicros = 0;
uint32_t lastPrintMs = 0;

void calibrateGyroscope(uint16_t samples = 500) {
  Serial.println("Keep the sensor completely still: calibrating gyroscope...");
  double sumX = 0.0, sumY = 0.0, sumZ = 0.0;
  uint16_t validSamples = 0;

  for (uint16_t i = 0; i < samples; ++i) {
    sensors_event_t accel, gyro, temp;
    if (mpu.getEvent(&accel, &gyro, &temp)) {
      sumX += gyro.gyro.x;
      sumY += gyro.gyro.y;
      sumZ += gyro.gyro.z;
      ++validSamples;
    }
    delay(4);
  }

  if (validSamples == 0) {
    Serial.println("ERROR: no valid IMU samples received.");
    return;
  }

  gyroBiasX = sumX / validSamples;
  gyroBiasY = sumY / validSamples;
  gyroBiasZ = sumZ / validSamples;

  Serial.printf("Gyro bias (rad/s): X=%.5f Y=%.5f Z=%.5f\n",
                gyroBiasX, gyroBiasY, gyroBiasZ);
  Serial.println("Calibration done. Do not move the sensor during startup calibration.");
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(800);
  Serial.println("\nESP32 + MPU6050 sensor monitor (no motor outputs)");

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!mpu.begin(0x68, &Wire)) {
    Serial.println("MPU6050 not found at 0x68. Check wiring/address; AD0 may select 0x69.");
    while (true) {
      delay(1000);
    }
  }

  // These are test settings, not recovered original project settings.
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

  Serial.println("MPU6050 detected at 0x68.");
  calibrateGyroscope();
  lastMicros = micros();
}

void loop() {
  const uint32_t nowMicros = micros();
  float dt = (nowMicros - lastMicros) / 1000000.0f;
  lastMicros = nowMicros;

  // Avoid integrating with an unreasonable time step after a stall.
  if (dt <= 0.0f || dt > 0.1f) {
    dt = 0.004f;
  }

  sensors_event_t accel, gyro, temp;
  if (!mpu.getEvent(&accel, &gyro, &temp)) {
    Serial.println("IMU read failed.");
    delay(10);
    return;
  }

  const float gx = gyro.gyro.x - gyroBiasX; // rad/s
  const float gy = gyro.gyro.y - gyroBiasY; // rad/s
  const float gz = gyro.gyro.z - gyroBiasZ; // rad/s

  // Accelerometer-derived roll/pitch. Axis signs depend on mounting.
  const float rollAcc = atan2f(accel.acceleration.y,
                               accel.acceleration.z) * 180.0f / PI;
  const float pitchAcc = atan2f(-accel.acceleration.x,
                                sqrtf(accel.acceleration.y * accel.acceleration.y +
                                      accel.acceleration.z * accel.acceleration.z))
                         * 180.0f / PI;

  // Complementary filter. Gyro rates are rad/s; convert to deg/s.
  rollDeg = FILTER_ALPHA * (rollDeg + gx * 180.0f / PI * dt)
            + (1.0f - FILTER_ALPHA) * rollAcc;
  pitchDeg = FILTER_ALPHA * (pitchDeg + gy * 180.0f / PI * dt)
             + (1.0f - FILTER_ALPHA) * pitchAcc;

  // Print at about 10 Hz to avoid excessive serial overhead.
  if (millis() - lastPrintMs >= 100) {
    lastPrintMs = millis();
    Serial.printf(
      "acc[g] X=% .3f Y=% .3f Z=% .3f | "
      "gyro[deg/s] X=% .2f Y=% .2f Z=% .2f | "
      "roll=% .2f deg pitch=% .2f deg temp=% .1f C\n",
      accel.acceleration.x / 9.80665f,
      accel.acceleration.y / 9.80665f,
      accel.acceleration.z / 9.80665f,
      gx * 180.0f / PI, gy * 180.0f / PI, gz * 180.0f / PI,
      rollDeg, pitchDeg, temp.temperature
    );
  }
}
