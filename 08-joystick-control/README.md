# Day 8 — Joystick Control in 30 Seconds

Part of the **Road to Electronics** series.

A KY-023 analog joystick module controls a servo motor's position on its X-axis, and lights up a green or red LED depending on Y-axis direction. Pressing the joystick button flashes both LEDs as a confirm signal.

## Parts List

- Arduino Nano
- KY-023 joystick module
- Servo motor
- Green LED
- Red LED
- 330 Ω resistors ×2 (for LEDs)
- Breadboard
- Male-to-male jumper wires
- Male-to-female jumper wires

## Wiring

| Component            | Pin / Wire          | Arduino Nano Pin |
|-----------------------|----------------------|-------------------|
| Joystick VRx (HOR)    | X-axis signal        | A2                |
| Joystick VRy (VERT)   | Y-axis signal         | A1                |
| Joystick SW (button)  | Button signal         | D7 (INPUT_PULLUP) |
| Joystick VCC          | Power                 | 5V                |
| Joystick GND          | Ground                | GND               |
| Green LED (+ resistor)| Anode                 | D2                |
| Red LED (+ resistor)  | Anode                 | D3                |
| Servo signal wire     | PWM control           | D9                |
| Servo power/ground    | Power / Ground        | 5V / GND          |

**Notes:**
- Both LED cathodes (through their 330 Ω resistors) tie into the breadboard ground rail, which connects back to the Nano's GND.
- The joystick button pin uses `INPUT_PULLUP`, so it reads LOW when pressed and HIGH when released.
- X/Y analog readings range 0–1023; a deadzone of 300–700 is used on both axes so the servo/LEDs don't jitter near center.

## How It Works

- **X-axis** — pushing the joystick left/right increments or decrements the servo's angle, clamped between 0° and 180°.
- **Y-axis** — pushing up lights the green LED, pushing down lights the red LED, and centered turns both off.
- **Button press** — flashes both LEDs in a quick double-blink as a visual confirm signal.

## Code

See [`joystick_servo_led.ino`](./joystick_servo_led.ino).

---
🔗 [@electroniclearns](https://instagram.com/electroniclearns) — Road to Electronics
