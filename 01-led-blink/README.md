# 01 — LED Blink

The classic first Arduino project: an LED that blinks on and off once per second.

## Parts

| Part | Qty |
|------|-----|
| Arduino Uno/Nano | 1 |
| LED (any color) | 1 |
| 220Ω resistor | 1 |
| Breadboard + jumper wires | — |

## Wiring

```
Arduino Pin 7 ──> 220Ω resistor ──> LED long leg (+)
LED short leg (−) ──> GND
```

## How It Works

- `pinMode(LED_PIN, OUTPUT)` tells the Arduino we'll be sending voltage *out* of pin 7
- `digitalWrite(LED_PIN, HIGH)` puts 5V on the pin → LED lights up
- `digitalWrite(LED_PIN, LOW)` puts 0V on the pin → LED turns off
- `delay(1000)` pauses everything for 1000 milliseconds (1 second)

`loop()` repeats forever, so the pattern is: on → wait 1s → off → wait 1s → repeat.

## Try This Next

- Change the delay values — can you make an SOS pattern (· · · — — — · · ·)?
- Add a second LED on another pin and make them alternate
