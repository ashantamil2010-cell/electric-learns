# Day 8 — Joystick Control in 30 Seconds 🕹️

An analog joystick drives a servo on one axis and a two-LED status indicator on the other. Push it toward one axis and the servo sweeps to follow; push it toward the other axis and a green or red LED tells you which way you're leaning. Press it down and both LEDs flash to confirm.

> **Road to Electronics** — learning electronics from scratch, no AI, just docs and datasheets.

---

## 🎯 What it does

- Push **X left** → servo rotates toward 0°.
- Push **X right** → servo rotates toward 180°.
- Push **Y up** → green LED on.
- Push **Y down** → red LED on.
- Center → servo holds, both LEDs off.
- Press the joystick button → both LEDs double-blink as a confirm flash.
- It starts centered at 90° and can't be driven past its 0–180° limits.

It reuses **Day 7's servo** as the output, but the input is brand new: one analog stick standing in for two potentiometers and a button.

---

## ⚙️ What is a joystick module, actually?

Strip away the plastic cap and a **KY-023 joystick** is just two potentiometers and a button sharing one shaft. Tilting the stick spins the wiper on each pot at the same time — one pot tracks the X tilt, the other tracks Y — so both axes are read as ordinary analog voltages, exactly like the single pot from **Day 4**. Pushing the stick straight down clicks a momentary switch underneath, wired to GND just like **Day 7's** buttons.

So a joystick isn't really a new component — it's two Day 4 potentiometers and one Day 7 button, mechanically ganged into a single stick.

### Reading two axes at once

Each axis comes back from `analogRead()` as a value 0–1023, centered around ~512 when the stick is untouched. Rather than reacting to the exact center number (which drifts a little from unit to unit), this project uses a **deadzone**:

- Below 300 → treated as "pushed low"
- Above 700 → treated as "pushed high"
- 300–700 → treated as centered, ignored

That 400-point buffer zone is what keeps the servo from twitching and the LEDs from flickering when the stick is just resting.

---

## 🧰 Parts

| Part | Qty | Notes |
|------|-----|-------|
| Arduino Nano | 1 | |
| KY-023 joystick module | 1 | X, Y, and click button |
| Hobby servo (SG90 / MG90S etc.) | 1 | The 3-wire kind |
| LED | 2 | Green + red |
| 330 Ω resistor | 2 | One per LED |
| Breadboard + jumper wires | — | Mix of M–M and M–F |

---

## 🔌 Wiring

### Joystick (5 wires)

| Joystick pin | Connects to |
|--------------|-------------|
| VCC | **5V** |
| GND | **GND** |
| VRx (HOR) | Analog **A2** |
| VRy (VERT) | Analog **A1** |
| SW (button) | Digital **7** (`INPUT_PULLUP`) |

### LEDs

| LED | Anode (through 330 Ω resistor) | Cathode |
|-----|----------------------------------|---------|
| Green | Digital **2** | GND |
| Red | Digital **3** | GND |

### Servo (3 wires)

| Servo wire | Connects to |
|------------|-------------|
| Brown or Black | **GND** |
| Red | **5V** |
| Orange or Yellow (signal) | Digital **9** |

Same servo wiring as Day 7 — this project just swaps the two-button input for a joystick.

> ⚡ **Power warning — carried over from Day 7.** A servo idling draws almost nothing, but movement (especially against resistance) can spike it to hundreds of mA. If the Arduino resets or the LEDs flicker when the servo moves, the servo is browning out the board — give it a separate 5V supply with the ground tied back to the Arduino.

---

## ⬆️ INPUT_PULLUP recap

Same idea as Day 6 and Day 7: a floating input pin picks up noise and reads randomly. `INPUT_PULLUP` switches on a resistor *inside* the chip that quietly holds the pin HIGH, so no external resistor is needed on the joystick's SW pin.

Clicking the stick down connects the pin to GND, which wins:

- **Not pressed → HIGH**
- **Pressed → LOW**

That's why the code checks `== 0` (LOW) to detect a press.

---

## 💻 The code, explained

### Setup

```cpp
int yPin = A1;
int xPin = A2;
int buttonPin = 7;
```

Two analog pins for the two axes, one digital pin for the click button — same `INPUT_PULLUP` pattern as Day 7's buttons.

```cpp
int servoPos = 90;
```

Just like Day 7's `pos`, this tracks the current commanded angle and starts centered.

```cpp
myservo.attach(servoPin);
```

Connects the servo object to pin 9, same as Day 7.

### The loop — X-axis (servo)

```cpp
if (xVal <= 300) {
  servoPos--;
} else if (xVal >= 700) {
  servoPos++;
}
```

This is the exact same held-input pattern as Day 7's buttons — while the stick sits past the threshold, the angle keeps ticking up or down every loop. The difference is what triggers it: two discrete button states on Day 7, one continuous analog reading here.

```cpp
if (servoPos > 180) servoPos = 180;
if (servoPos < 0) servoPos = 0;
```

Same **clamping** as Day 7, protecting the servo from straining against its own end-stop.

### The loop — Y-axis (LEDs)

```cpp
if (yVal <= 300) {
  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED, LOW);
} else if (yVal >= 700) {
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, HIGH);
} else {
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);
}
```

Straightforward direction indicator — one axis, two LEDs, three states (up / down / centered).

### The loop — button press

```cpp
if (buttonState == 0) {
  // flash both LEDs twice
}
```

Reads LOW when the stick is clicked in (same `INPUT_PULLUP` logic as Day 7), then runs a short blocking blink sequence as a visual confirm.

---

## 🛠️ Troubleshooting

| Problem | Likely cause / fix |
|---------|--------------------|
| Servo jitters near center | Deadzone too tight — widen the 300–700 range |
| Axes feel reversed | Swap which `analogRead()` feeds `xVal` vs `yVal`, or swap the wires at VRx/VRy |
| LEDs never light up | Check resistor + cathode-to-GND wiring, same as any LED |
| Button does nothing | Confirm `INPUT_PULLUP` and that SW is wired to a digital pin, not analog |
| Servo resets the board when moving | Underpowered — separate 5V supply, same fix as Day 7 |
| Servo won't move at all | Check it's on D9 and `myservo.attach()` ran in `setup()` |

---

## 🚀 Level up

1. **Speed control** — scale how much `servoPos` changes per loop based on how far the stick is pushed, instead of a fixed ±1.
2. **Two servos** — use both axes to drive pan and tilt at once instead of one axis to a servo and one to LEDs.
3. **Kill the `delay()`** — swap the button's blink sequence for a `millis()` timer, same non-blocking upgrade flagged on Day 6 and Day 7.
4. **Combine with Day 6** — bring back the LCD and print the live `servoPos` and joystick direction as it moves.

---

## 📚 Official documentation

- **Arduino — AnalogRead reference:** https://docs.arduino.cc/language-reference/en/functions/analog-io/analogRead/
- **Servo library reference:** https://docs.arduino.cc/libraries/servo/
- **KY-023 joystick module overview:** https://docs.arduino.cc/retired/getting-started-guides/joystick-module/

---

## 🏃 How to run

1. Wire it up as above.
2. Open `joystick_servo_led.ino` in the Arduino IDE.
3. `Servo` is a built-in library — nothing to install.
4. Pick your board and port under **Tools**.
5. **Upload**, then move the stick and watch the servo and LEDs react.

---

*Built by Aya S.T. · Road to Electronics · [@electroniclearns](https://instagram.com/electroniclearns)*
