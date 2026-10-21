# Hardware Configuration

> Status: starter template. Replace proposed values with verified component details before flight.

## Components

| Component | Quantity | Actual model / value | Verification status |
|---|---:|---|---|
| ESP32 development board | 1 | To be confirmed | Unverified |
| IMU | 1 | MPU6050 / GY-521 | Confirm module |
| Frame | 1 | To be confirmed | Unverified |
| Brushless motors | 4 | To be confirmed | Unverified |
| ESCs | 4 | To be confirmed | Unverified |
| Propellers | 4 | To be confirmed | Unverified |
| Battery | 1 | To be confirmed | Unverified |
| Power distribution | 1 | To be confirmed | Unverified |
| Radio receiver/transmitter | 1 set | To be confirmed | Unverified |

## Wiring Record

| Signal | Pin / connection | Confirmed? |
|---|---|---|
| MPU6050 SDA | ESP32 GPIO 21 (proposed) | No |
| MPU6050 SCL | ESP32 GPIO 22 (proposed) | No |
| ESC 1 signal | ESP32 GPIO 25 (proposed) | No |
| ESC 2 signal | ESP32 GPIO 26 (proposed) | No |
| ESC 3 signal | ESP32 GPIO 27 (proposed) | No |
| ESC 4 signal | ESP32 GPIO 33 (proposed) | No |
| Common signal ground | Controller and ESC signal reference | Verify wiring |

## Before Powering On

- Confirm the exact ESP32 board pinout.
- Confirm the MPU6050 module's supply and logic-level requirements.
- Confirm motor/propeller compatibility using manufacturer thrust and current data.
- Confirm ESC input protocol and command limits from its manual.
- Confirm battery voltage, current capability, connector, and wiring ratings.
- Remove all propellers during bench testing.
- Do not connect battery voltage directly to ESP32 GPIO pins.
