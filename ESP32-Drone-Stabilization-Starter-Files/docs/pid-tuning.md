# PID Tuning and Validation Notes

> This document is a procedure, not a set of proven gains. Do not copy arbitrary PID values into a flying drone.

## Recommended Sequence

1. Verify sensor axes, gyro bias, and attitude estimates with motors disabled.
2. Verify motor order and direction with propellers removed.
3. Test the mixer and PID calculations in simulation or with logged data.
4. Start with conservative rate-control gains and tune one axis at a time.
5. Increase proportional gain gradually; stop if oscillation or excessive heating appears.
6. Add derivative damping carefully and filter noisy derivative measurements.
7. Add a limited integral term only when needed; implement anti-windup.
8. Add the outer roll/pitch angle loop only after rate loops are stable.
9. Test failsafe and disarm behavior before any flight test.
10. Log every configuration change and the result.

## Record Your Actual Values

| Parameter | Roll | Pitch | Yaw | Notes |
|---|---:|---:|---:|---|
| Kp | TBD | TBD | TBD | Record units and controller type |
| Ki | TBD | TBD | TBD | Record units and anti-windup limits |
| Kd | TBD | TBD | TBD | Record derivative filtering |
| Output limit | TBD | TBD | TBD | Match motor mixer and ESC limits |
| Test date / firmware commit | TBD | TBD | TBD | Link to test log |

## Important

- Rate PID and angle PID are different controllers.
- PID gains depend on frame, mass, motor/propeller combination, sensor orientation, loop rate, and units.
- Use measured elapsed time in calculations.
- Validate mixer signs and motor rotation directions before flight.
- Do not conduct initial tests with propellers installed.
