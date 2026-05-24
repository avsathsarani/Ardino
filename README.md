# Arc Discharge Controller

This project contains an Arduino sketch for controlling a stepper motor used in an arc discharge setup.

## Overview

The sketch provides:

- **Start/forward motion** with the `START` button
- **Manual reverse jog** with the `REVERSE` button
- **Emergency stop** using `ESTOP`
- **Automatic retraction** after the limit switch is triggered

## Hardware

### Pin assignments

| Signal | Arduino Pin | Direction | Description |
| --- | --- | --- | --- |
| `STEP_PIN` | `7` | Output | Step pulse output |
| `DIR_PIN` | `6` | Output | Direction control |
| `EN_PIN` | `10` | Output | Motor enable/disable |
| `START_BTN` | `9` | Input | Start/forward command |
| `REVERSE_BTN` | `8` | Input | Manual reverse jog command |
| `ESTOP_PIN` | `2` | Input | Emergency stop input |
| `LIMIT_PIN` | `3` | Input | Limit switch input |

### Important notes

- `EN_PIN` is driven **HIGH** to disable the motor and **LOW** to enable it.
- The `ESTOP_PIN` and `LIMIT_PIN` are configured as interrupt inputs.
- The sketch assumes the limit switch and E-stop are wired with a **pulled-down** or otherwise stable input logic.

## Behavior

### Normal operation

1. Press `START_BTN` to begin forward motion.
2. The motor runs continuously while `motorRunning` is `true`.
3. When the limit switch is triggered, the motor stops, retracts by `REVERSE_STEPS`, and then waits for the limit switch to be released.

### Manual reverse jog

- Press `REVERSE_BTN` to jog the motor backward by `REVERSE_JOG_STEPS`.
- The reverse jog is only allowed when the motor is not already running.

### Emergency stop

- Triggering `ESTOP_PIN` immediately disables the motor.
- The sketch waits until the E-stop input is released before resuming normal operation.

## Timing and motion parameters

| Parameter | Value | Description |
| --- | --- | --- |
| `STEP_DELAY_US` | `1000` | Delay between step pulse HIGH and LOW |
| `REVERSE_STEPS` | `600` | Automatic retract distance after limit hit |
| `REVERSE_JOG_STEPS` | `10000` | Manual reverse jog distance |

## File structure

- `Code_for_arc_discharge.ino` — Arduino sketch
- `README.md` — project documentation

## Safety

- Verify the motor driver and power supply wiring before powering the system.
- Test the E-stop function before using the machine in a live setup.
- Ensure the limit switch is correctly positioned so the automatic reverse does not overtravel.
- Do not rely on software alone for machine safety.

## Uploading to Arduino

1. Open `Code_for_arc_discharge.ino` in the Arduino IDE.
2. Select the correct board and serial port.
3. Upload the sketch.
4. Power the motor driver and confirm the wiring matches the pin map above.

## Troubleshooting

- If the motor does not move, confirm `EN_PIN` is being driven LOW during operation.
- If the system stalls after the limit switch, inspect the `LIMIT_PIN` wiring and the `limitTriggered` logic.
- If the E-stop does not stop the motor, verify the interrupt wiring and input state.

## License

This project is provided for educational and experimental use.
