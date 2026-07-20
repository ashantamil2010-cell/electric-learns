# Day 4 — Potentiometer LED Dimmer

Part of the **Road to Electronics** series ([@electroniclearns](https://instagram.com/electroniclearns))

Turn a knob, dim an LED. This project introduces analog input (`analogRead`) and PWM output (`analogWrite`) — the jump from ON/OFF digital control to smooth analog-style control.

## How It Works

1. The potentiometer acts as a voltage divider — its middle pin (wiper) outputs 0–5V depending on knob position.
2. `analogRead(A1)` converts that voltage to a number from **0 to 1023**.
3. `map()` rescales it to **0 to 255**, the range `analogWrite` accepts.
4. `analogWrite(3, brightness)` uses **PWM** — the pin switches on/off ~490 times per second. The duty cycle (percent of time ON) determines how bright the LED *appears*. Your eyes can't see the flicker, so it just looks dimmer or brighter.

## Parts

| Part | Qty |
|------|-----|
| Arduino Nano (or Uno) | 1 |
| LED | 1 |
| 220Ω resistor | 1 |
| Potentiometer (10kΩ) | 1 |
| Jumper wires | ~5 |
| Breadboard | 1 |

## Wiring

| Component | Arduino Pin |
|-----------|-------------|
| LED (+) via 220Ω resistor | D3 (PWM ~) |
| LED (−) | GND |
| Pot outer pin 1 | 5V |
| Pot outer pin 2 | GND |
| Pot middle pin (wiper) | A1 |

> ⚠️ The LED **must** be on a PWM pin (marked with `~`): 3, 5, 6, 9, 10, or 11. On other pins, `analogWrite` just snaps the LED fully on or off.

## Try It

- Open **Serial Plotter** (Tools → Serial Plotter) and turn the knob — you'll see a live graph of the brightness value.
- Swap `map(sensorValue, 0, 1023, 0, 255)` to `map(sensorValue, 0, 1023, 255, 0)` to reverse the knob direction.
- Try controlling two LEDs in opposite directions with one knob.

## Series

| Day | Project |
|-----|---------|
| 1 | Blink |
| 2 | Pushbutton Toggle |
| 3 | Ultrasonic Traffic Light |
| **4** | **Potentiometer LED Dimmer** ← you are here |
