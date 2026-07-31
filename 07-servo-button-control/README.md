# Day 7 — Two-Button Servo Control 🦾

Two push buttons drive a servo motor. Hold the **left** button and the arm sweeps one way; hold the **right** button and it sweeps back; let go and it holds. This is the first project in the series that **moves** — every day before this ended at a light or a screen.

> **Road to Electronics** — learning electronics from scratch, no AI, just docs and datasheets.

---

## 🎯 What it does

- Hold **left** → servo rotates toward 0°.
- Hold **right** → servo rotates toward 180°.
- Release both → servo stays put.
- It starts centered at 90° and can't be driven past its 0–180° limits.

It reuses **Day 2's buttons** as the input, but the output is brand new: an actuator that turns electricity into physical motion.

---

## ⚙️ What is a servo motor?

A **servo** is a small motor that goes to a *specific angle* and holds it. That's the key difference from a plain DC motor, which just spins as fast as the voltage lets it. You don't tell a servo "spin" — you tell it "go to 47°," and it goes there and stays there, pushing back if something tries to move it.

Standard hobby servos rotate through about **0–180°** — half a turn.

### What's inside

A servo is actually four things packed into one case:

1. **A DC motor** — the thing that actually spins.
2. **A gearbox** — trades the motor's uselessly-fast spin for slow, strong movement (torque). This is why a tiny servo can push with surprising force.
3. **A potentiometer** — geared to the output shaft, so its resistance tells the servo *what angle the shaft is currently at*. This is the same component from Day 4, doing sensing.
4. **A control circuit** — the brain. It constantly compares "where am I?" (from the pot) against "where should I be?" (your command) and drives the motor to close the gap.

That last part is a **feedback loop**, and it's the whole idea behind a servo. Tell it 90°, and if the pot says it's at 85°, the circuit nudges the motor until the pot reads 90°. Try to force the arm off-target and it fights back. You'll build much more sophisticated versions of this exact loop later on in robotics — it's the foundation of how machines hold and correct position.

### How does it know the angle? (PWM)

The servo only has three wires, and one of them is the signal. So how do you send an angle down a single wire?

With a **pulse**, repeated every 20 ms. The *width* of that pulse — how long the signal stays HIGH — encodes the angle:

- ~1.0 ms pulse → 0°
- ~1.5 ms pulse → 90°
- ~2.0 ms pulse → 180°

The servo measures the pulse width and moves there. This is **PWM** (Pulse Width Modulation) again — the same trick that dimmed the LED on Day 4 — but used completely differently. On Day 4 the *average* voltage set brightness. Here the *precise timing* of each pulse sets an angle. Same tool, different job.

The best part: `Servo.h` handles all of that timing for you. You just call `myServo.write(90)` and the library generates the correct stream of pulses in the background.

---

## 🧰 Parts

| Part | Qty | Notes |
|------|-----|-------|
| Arduino Nano (or Uno) | 1 | |
| Hobby servo (SG90 / MG90S etc.) | 1 | The 3-wire kind |
| Push button | 2 | Momentary tactile switches |
| Breadboard + jumper wires | — | ~9 wires |

No resistors at all — the buttons use internal pull-ups, and the servo is a self-contained module.

---

## 🔌 Wiring

### Servo (3 wires)

Servo wire colors are fairly standard:

| Servo wire | Connects to |
|------------|-------------|
| Brown or Black | **GND** |
| Red | **5V** |
| Orange or Yellow (signal) | Digital **9** |

### Buttons

Each button connects a digital pin to GND. Because we use `INPUT_PULLUP`, no resistors are needed.

| Button | One leg | Other leg |
|--------|---------|-----------|
| Left | Digital **3** | GND |
| Right | Digital **2** | GND |

That's it — 5V and GND to the servo, signal to D9, and two buttons to D3 and D2.

> ⚡ **Power warning — read this one.** A servo standing still sips only a few mA, but the moment it *moves* — and especially if it stalls against something — it can pull hundreds of mA to over an amp. Powering a small unloaded servo from the Arduino's 5V pin is usually fine, but if your board keeps resetting, browning out, or the servo twitches randomly, that's the servo starving the Arduino. The fix is a **separate 5V supply** (battery pack or wall adapter) for the servo, with its **ground tied to the Arduino's ground**. For anything bigger than an SG90, or anything under load, power it separately from the start. Never run a metal-gear or high-torque servo off the Arduino's 5V pin.

---

## ⬆️ INPUT_PULLUP recap

Same idea as Day 6: a floating input pin picks up noise and reads randomly. `INPUT_PULLUP` switches on a resistor *inside* the chip that quietly holds the pin HIGH, so you don't need to wire your own.

Pressing the button connects the pin to GND, which wins:

- **Not pressed → HIGH**
- **Pressed → LOW**

That's why the code checks `== LOW` to detect a press.

---

## 💻 The code, explained

### Setup

```cpp
#include <Servo.h>
Servo myServo;
```

Pull in the library and create a servo object to command.

```cpp
int pos = 90;
const int servoPin = 9;
const int leftButton  = 3;
const int rightButton = 2;
```

`pos` holds the current target angle — it starts at 90 (centered) and every button press will nudge this number. The `const` values name the pins so the code reads clearly.

```cpp
myServo.attach(servoPin);
myServo.write(pos);
```

`attach()` connects the servo object to pin 9 and starts generating the control pulses. `write(pos)` sends it to the 90° starting position so it doesn't jump on power-up.

### The loop

```cpp
int leftState  = digitalRead(leftButton);
int rightState = digitalRead(rightButton);
```

Read both buttons every pass.

```cpp
if (leftState == LOW && pos > 0) {
  pos--;
}
if (rightState == LOW && pos < 180) {
  pos++;
}
```

The heart of it. If the left button is held **and** there's room to move (`pos > 0`), drop the target angle by one. Same for the right button in the other direction, capped at 180.

**This is deliberately NOT edge detection.** On Day 6, holding the button was a problem to solve. Here it's the whole point — holding down = keep moving. Because the loop runs continuously, a held button ticks `pos` up or down one degree at a time, over and over, producing smooth motion.

The `pos > 0` and `pos < 180` guards are **clamping** — they keep the number inside the servo's physical range. Command a standard servo past its limits and it'll strain against its own end-stop, buzzing and drawing extra current. Clamping prevents that.

```cpp
myServo.write(pos);
delay(10);
```

Push the current angle to the servo, then wait 10 ms. That delay sets the **speed**: one degree every 10 ms means a full 0→180 sweep takes about 1.8 seconds of holding. Want slower, more precise control? Increase the delay. Want it snappier? Decrease it.

---

## 🛠️ Troubleshooting

| Problem | Likely cause / fix |
|---------|--------------------|
| Servo jitters / buzzes constantly | Underpowered — give it a separate 5V supply (grounds tied) |
| Arduino resets when servo moves | Same — the servo is browning out the board |
| Servo moves the wrong direction | Swap `pos--` / `pos++`, or flip which button is which |
| Only moves one way | Check that button's wiring — one leg to the pin, one to **GND** |
| Buttons do nothing | Confirm `INPUT_PULLUP` and that buttons go to GND, not 5V |
| Servo slams to one side on startup | Normal-ish — `write(90)` in setup centers it; make sure it's there |
| Movement too fast to control | Increase the `delay(10)` value |

> 📌 One quirk worth knowing: on an Uno/Nano, using the Servo library **disables `analogWrite()` (PWM) on pins 9 and 10**, whether or not a servo is attached there. So if you add an LED later and it won't dim, that's why — move it off pins 9 and 10.

---

## 🚀 Level up

1. **Add a potentiometer** to set the *speed* — read it with `analogRead()` (Day 4!) and use it to change the delay live.
2. **Position presets** — a third button that jumps straight to 0°, 90°, or 180°.
3. **Kill the `delay()`** — swap it for a `millis()` timer so the board stays responsive and could do other things while the servo moves. Same non-blocking upgrade flagged on Day 6.
4. **Show the angle** — bring back the Day 6 LCD and print `pos` live as it moves. Now two projects are combining.

---

## 📚 Official documentation

- **Arduino — Servo Motor Basics:** https://docs.arduino.cc/learn/electronics/servo-motors
- **Basic Servo Control tutorial:** https://docs.arduino.cc/tutorials/generic/basic-servo-control/
- **Servo library reference:** https://docs.arduino.cc/libraries/servo/
- **Library source:** https://github.com/arduino-libraries/Servo

---

## 🏃 How to run

1. Wire it up as above.
2. Open `servo_button_control.ino` in the Arduino IDE.
3. `Servo` is a built-in library — nothing to install.
4. Pick your board and port under **Tools**.
5. **Upload**, then hold a button and watch it move.

---

*Built by Aya S.T. · Road to Electronics · [@electroniclearns](https://instagram.com/electroniclearns)*
