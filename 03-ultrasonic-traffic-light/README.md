# 03 — Ultrasonic Traffic Light

An HC-SR04 distance sensor drives three LEDs like a traffic light — basically a DIY parking sensor.

| Distance | LED |
|----------|-----|
| < 10 cm | 🔴 Red |
| 10–20 cm | 🟡 Yellow |
| ≥ 20 cm | 🟢 Green |

## Parts

| Part | Qty |
|------|-----|
| Arduino Uno/Nano | 1 |
| HC-SR04 ultrasonic sensor | 1 |
| Red, yellow, green LEDs | 1 each |
| 220Ω resistors | 3 |
| Breadboard + jumper wires | — |

## Wiring

```
HC-SR04 VCC  ──> 5V
HC-SR04 GND  ──> GND
HC-SR04 TRIG ──> Arduino Pin 9
HC-SR04 ECHO ──> Arduino Pin 8

Pin 4 ──> 220Ω ──> Red LED (+)    ──> GND
Pin 3 ──> 220Ω ──> Yellow LED (+) ──> GND
Pin 2 ──> 220Ω ──> Green LED (+)  ──> GND
```

## How It Works

1. A 10µs pulse on TRIG makes the sensor fire 8 ultrasonic chirps at 40kHz
2. The sensor holds ECHO high for exactly as long as the sound took to bounce back
3. `pulseIn()` measures that time in microseconds
4. Distance = time × 0.034 cm/µs ÷ 2 (divide by 2 because the sound travels out *and* back)
5. An `if / else if / else` chain lights exactly one LED based on the distance

## Notes

- The `duration` variable is a `long` because `pulseIn()` can return up to 1,000,000 — too big for an `int` on 8-bit boards
- Reliable range is about 2cm–400cm; soft or angled surfaces give flaky readings
- If readings seem stuck at 0, double-check TRIG/ECHO aren't swapped

## Try This Next

- Add a buzzer that beeps faster as objects get closer (real parking sensor!)
- Print the distance to Serial Monitor for debugging
- Use it to trigger a servo gate — the classic railway crossing build
