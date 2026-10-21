# ESP32-Drone-Stabilization
Developed an ESP32-based quadcopter stabilization system using an MPU6050 IMU, complementary-filter sensor fusion, and independent PID control loops for roll, pitch, and yaw. Integrated ESC-controlled brushless motors, debugged ESC timing inconsistencies, and evaluated hover stability through multiple flight tests.
# ESP32-Based Quadcopter Drone Stabilization System

**Embedded Systems | Flight Control | PID Control | Sensor Fusion | Robotics**

## Project Overview

This project focuses on developing a quadcopter stabilization system using an ESP32 microcontroller, MPU6050 inertial measurement unit (IMU), electronic speed controllers (ESCs), and brushless DC motors.

The system uses real-time sensor measurements, complementary-filter-based attitude estimation, and independent PID control loops to stabilize the drone along its roll, pitch, and yaw axes.

The project emphasizes embedded control, sensor calibration, motor control, timing consistency, and iterative stability testing.

> **Project status:** Rebuild and configuration documentation. The hardware configuration and numerical settings below are proposed starting points unless explicitly identified as confirmed project details. Final flight performance must be validated on the assembled hardware.

## Objectives

- Develop a real-time quadcopter attitude-stabilization system.
- Interface an MPU6050 IMU with an ESP32 using I²C communication.
- Estimate roll and pitch using accelerometer and gyroscope measurements.
- Implement PID-based rotational stabilization.
- Generate ESC control signals for four brushless motors.
- Investigate ESC timing inconsistencies and control-loop timing.
- Evaluate stability through staged bench testing and controlled flight tests.
- Document hardware configuration, firmware parameters, and test results.

## System Architecture

```text
                  MPU6050 IMU
          Accelerometer + Gyroscope
                       |
                       v
              ESP32 Microcontroller
                       |
                       v
             Sensor Calibration
                       |
                       v
             Complementary Filter
                       |
                       v
              Attitude Estimation
                       |
                       v
          Roll / Pitch / Yaw Controllers
                       |
                       v
                 Motor Mixer
                       |
                       v
               Four ESC Outputs
                       |
                       v
             Four Brushless Motors
                       |
                       v
                Quadcopter Motion
                       |
                       +---- Feedback to IMU
```

The controller continuously measures the drone's motion and adjusts motor commands to reduce rotational error.

## Hardware Components

| Component | Quantity | Proposed specification |
|---|---:|---|
| Microcontroller | 1 | ESP32-WROOM-32 development board |
| IMU | 1 | MPU6050 / GY-521 |
| Quadcopter frame | 1 | 450 mm X-frame |
| Brushless motors | 4 | 2212, approximately 920 KV |
| ESCs | 4 | Candidate 30 A ESCs, subject to motor-current verification |
| Propellers | 4 | Candidate 10 × 4.5 inch CW/CCW-compatible set |
| Battery | 1 | Candidate 3S LiPo, 11.1 V nominal |
| Battery capacity | 1 | Candidate 2200–3000 mAh |
| Power distribution | 1 | Current-rated power distribution board or harness |
| Controller power supply | 1 | Regulated supply compatible with the ESP32 board |
| Radio control system | 1 set | Compatible receiver and transmitter with failsafe support |

**Important:** These are proposed rebuild components, not verified specifications of the original drone. Motor, propeller, battery, and ESC compatibility must be confirmed before assembly or flight.

## Hardware Connections

### MPU6050 to ESP32

| MPU6050 pin | ESP32 connection |
|---|---|
| SDA | GPIO 21 |
| SCL | GPIO 22 |
| GND | Common ground |
| VCC | Supply compatible with the exact breakout board |

The typical MPU6050 I²C address is `0x68` when the AD0 pin is low. It can be `0x69` when AD0 is high.

### ESC signal connections

| Motor channel | Example ESP32 GPIO |
|---|---|
| Motor 1 | GPIO 25 |
| Motor 2 | GPIO 26 |
| Motor 3 | GPIO 27 |
| Motor 4 | GPIO 33 |

These are example GPIO assignments and must be checked against the selected ESP32 board and firmware.

ESC power connections must use an appropriately rated battery and power-distribution system. Do not power brushless motors directly from the ESP32.

## Operating Principle

### 1. IMU Data Acquisition

The MPU6050 provides six measurement channels:

- Three-axis acceleration: X, Y, and Z.
- Three-axis angular velocity: X, Y, and Z.

These measurements provide information about the drone's orientation and rotational motion.

### 2. Sensor Calibration

Gyroscope bias is measured while the drone is stationary. The measured offset is subtracted from subsequent readings to reduce integration drift.

Accelerometer readings are also checked for correct axis orientation and plausible gravity measurements.

### 3. Complementary Filter

The complementary filter combines the fast response of the gyroscope with the long-term reference provided by the accelerometer.

A typical roll estimate is:

\[
\phi_t =
\alpha(\phi_{t-1}+\omega_x\Delta t)
+(1-\alpha)\phi_{\text{acc}}
\]

Where:

- \(\phi_t\): estimated roll angle.
- \(\omega_x\): corrected roll-axis angular velocity.
- \(\Delta t\): elapsed sampling time.
- \(\alpha\): complementary-filter coefficient.
- \(\phi_{\text{acc}}\): accelerometer-derived roll estimate.

A starting coefficient of `0.98` may be evaluated at a 250 Hz control rate, but the correct value depends on the implementation and must be validated.

The MPU6050 has no magnetometer. Consequently, yaw angle obtained by integrating its gyroscope will drift over time. Yaw-rate stabilization is possible without a magnetometer.

### 4. PID Control

The PID controller computes a correction from the difference between the target and measured motion.

\[
u(t)=K_p e(t)+K_i\int e(t)\,dt
+K_d\frac{de(t)}{dt}
\]

Where:

- \(K_p\): proportional gain.
- \(K_i\): integral gain.
- \(K_d\): derivative gain.
- \(e(t)\): control error.
- \(u(t)\): controller output.

Separate control loops are used for roll, pitch, and yaw.

The initial implementation should prioritize gyroscope-based rate stabilization. An outer roll/pitch angle controller can be introduced after the inner rate loops have been verified.

Actual PID gains must be determined through controlled tuning on the completed hardware. No unverified gains are presented as tested values.

### 5. Motor Mixing

The motor mixer combines throttle and rotational corrections to generate individual motor commands.

For an X-frame quadcopter, the mixer must account for:

- Motor numbering.
- Motor rotation direction.
- Frame geometry.
- Roll and pitch correction signs.
- Yaw torque direction.
- ESC command limits.

Incorrect motor mapping or mixer signs can destabilize the drone. Verify motor order and rotation direction with propellers removed before attempting flight.

## Proposed Firmware Configuration

| Parameter | Initial candidate |
|---|---|
| Microcontroller | ESP32-WROOM-32 |
| IMU | MPU6050 |
| Communication | I²C |
| IMU address | `0x68`, subject to verification |
| Accelerometer range | ±4 g |
| Gyroscope range | ±500°/s |
| IMU sampling target | 500 Hz |
| Control-loop target | 250 Hz |
| Control-loop period | 4 ms |
| Attitude estimation | Complementary filter |
| Filter coefficient | 0.98, subject to validation |
| Initial controller | Gyroscope rate PID |
| ESC interface | Standard PWM, if supported |
| Candidate PWM range | 1000–2000 µs, subject to ESC documentation |
| Integral windup protection | Required |
| Derivative filtering | Recommended |
| Timing strategy | Fixed-rate scheduling with measured elapsed time |
| Sensor fault handling | Defined safe-state and disarm behavior |

These are starting configuration values, not verified measurements from the original build. Confirm the ESC protocol, sensor ranges, loop timing, and safe motor-command limits before enabling motor outputs.

## Firmware Structure

A modular implementation is recommended.

```text
ESP32-Drone-Stabilization/
├── README.md
├── firmware/
│   ├── DroneStabilization.ino
│   ├── imu.cpp
│   ├── imu.h
│   ├── attitude.cpp
│   ├── attitude.h
│   ├── pid.cpp
│   ├── pid.h
│   ├── motor_mixer.cpp
│   └── safety.cpp
├── docs/
│   ├── hardware-configuration.md
│   ├── wiring.md
│   ├── calibration.md
│   ├── pid-tuning.md
│   └── test-results.md
├── diagrams/
│   └── system-architecture.png
└── LICENSE
```

This is a suggested repository structure. Add only source files, diagrams, and test records that actually exist in your implementation.

## Development and Testing Procedure

### Phase 1: Sensor Verification

- Verify I²C communication with the MPU6050.
- Confirm the sensor address and configured measurement ranges.
- Calibrate gyroscope offsets.
- Check the axis mapping and angle-estimation behavior.
- Record sensor readings and control-loop execution time.

### Phase 2: ESC and Motor Verification

- Remove all propellers before bench testing.
- Confirm ESC arming and minimum-command behavior.
- Verify the motor sequence and rotation directions.
- Check the PWM protocol and timing requirements.
- Confirm that the emergency disarm mechanism works.

### Phase 3: Controller Verification

- Test the PID calculations independently.
- Verify correction signs using simulated disturbances.
- Check output saturation and integral windup protection.
- Validate the motor mixer with a known motor layout.
- Check sensor-timeout and watchdog behavior.

### Phase 4: Stability Testing

- Begin with conservative rate-controller gains.
- Tune roll, pitch, and yaw separately.
- Introduce the outer roll/pitch angle loop after rate control is stable.
- Log attitude estimates, angular velocities, PID outputs, and motor commands.
- Conduct controlled flight tests only after all bench checks and failsafe tests pass.

## Performance Evaluation

The following metrics should be recorded during testing:

| Metric | Purpose | Actual result |
|---|---|---|
| Control-loop frequency | Timing consistency | Not recorded |
| Gyroscope bias | Calibration quality | Not recorded |
| Roll/pitch error | Attitude stability | Not recorded |
| Recovery time | Disturbance response | Not recorded |
| Overshoot | Transient response | Not recorded |
| Oscillation amplitude | PID tuning quality | Not recorded |
| Hover duration | Flight endurance | Not recorded |
| Battery voltage under load | Power-system behavior | Not recorded |

The project description includes debugging ESC timing inconsistencies and evaluating hover stability through multiple flight tests. Quantitative performance results should be added only after they have been measured or recovered from the original test logs.

## Challenges and Engineering Considerations

- Gyroscope drift and accelerometer noise.
- Vibration affecting IMU measurements.
- ESC signal timing inconsistencies.
- Control-loop scheduling and sampling-time variation.
- PID gain selection and oscillation.
- Correct motor numbering and mixer signs.
- Power delivery, battery current limits, and electrical noise.
- Failsafe behavior during sensor or receiver failure.

## Skills Demonstrated

- ESP32 embedded programming.
- MPU6050 sensor interfacing.
- I²C communication.
- IMU calibration and signal processing.
- Complementary-filter sensor fusion.
- PID feedback control.
- Brushless motor and ESC interfacing.
- Real-time control-loop timing.
- Embedded-system debugging.
- Experimental validation and technical documentation.

## Future Improvements

- Add an outer attitude controller with validated rate loops.
- Improve sensor filtering and vibration isolation.
- Implement comprehensive data logging and PID tuning tools.
- Evaluate a more capable IMU or a magnetometer for heading estimation.
- Add battery-voltage monitoring and robust failsafe handling.
- Compare measured performance across repeatable test conditions.
- Evaluate integration with a mature flight-control stack if reliability requirements exceed the scope of the educational prototype.

## Safety Notes

- Remove propellers during firmware, sensor, and motor-output bench tests.
- Use a battery and power-distribution system rated for the actual current.
- Validate motor order, rotation direction, and correction signs before flight.
- Verify disarming and failsafe behavior before enabling flight.
- Do not assume a software-only failsafe guarantees safety.
- Follow applicable aviation, battery-handling, and local flight requirements.

## Conclusion

This project explores the integration of embedded programming, inertial sensing, sensor fusion, PID control, and brushless motor actuation in a quadcopter stabilization system.

The rebuild process is intended to produce a documented and reproducible flight-control implementation, with verified hardware specifications, calibrated sensor measurements, validated controller settings, and recorded stability results.

**Note:** The proposed hardware configuration and firmware parameters are starting points. They must not be interpreted as verified results from the original drone or as a guarantee of stable flight.
