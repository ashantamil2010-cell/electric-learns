# 02 — Pushbutton Toggle

Press a button to toggle an LED on and off. Introduces inputs, pull-up resistors, and the modulo operator.

## Parts

| Part | Qty |
|------|-----|
| Arduino Uno/Nano | 1 |
| Pushbutton | 1 |
| LED (any color) | 1 |
| 220Ω resistor | 1 |
| Breadboard + jumper wires | — |

## Wiring

```
Button leg A ──> Arduino Pin 3
Button leg B ──> GND
(no external resistor needed — we use the built-in pull-up)

Arduino Pin 7 ──> 220Ω resistor ──> LED long leg (+)
LED short leg (−) ──> GND
```

## How It Works

- `INPUT_PULLUP` enables the Arduino's internal pull-up resistor, so the pin reads **HIGH** normally and **LOW** when pressed (flipped from what you'd expect!)
- Every press increments a counter
- `pressedAmount % 2` checks if the count is even or odd — even means LED on, odd means LED off

## Known Limitation (On Purpose!)

`loop()` runs thousands of times per second, so holding the button counts as many presses and the toggle can feel random. This is left in as a teaching moment — the fix is **edge detection + debouncing**: only count when the reading *changes* from HIGH to LOW, and ignore changes faster than ~50ms. Try implementing it!

## Try This Next

- Add edge detection so one press = exactly one toggle
- Use the counter to cycle through 3 LEDs instead of toggling one
