# Day 5 — Automatic Night Light 💡

An LED that turns **itself** on when the room gets dark. No button to press, no knob to turn — a photoresistor watches the light and an `if` statement makes the decision. This is the first project in the series where the board reacts to the world on its own.

> **Road to Electronics** — learning electronics from scratch, no AI, just docs and datasheets.

---

## 🎯 What it does

- Reads how bright the room is using a photoresistor (LDR).
- When the light drops below a set level, the LED switches **on**.
- When the room brightens again, the LED switches **off**.

The classic demo: cup your hand over the sensor and watch the LED come on.

---

## 🔦 What is a photoresistor?

A **photoresistor** — also called an **LDR** (Light-Dependent Resistor) or **photocell** — is a resistor whose resistance *changes with light*. The more light hits it, the less it resists the flow of current.

- **Bright light → low resistance** (a few kΩ)
- **Darkness → high resistance** (hundreds of kΩ, up to ~1 MΩ)

The exact numbers vary by part; the common GL5528 in most starter kits sits around 10–20 kΩ in normal room light and climbs toward 1 MΩ in the dark.

### How does it actually work?

The squiggly sensing surface is made of a **semiconductor** — usually cadmium sulfide (CdS). Here's the chain of events:

1. In the dark, most of the material's electrons are locked in place. Few electrons are free to move, so current struggles to pass → **high resistance**.
2. When light (photons) hits the surface, the photons hand their energy to electrons. Electrons that absorb enough energy break free and become **charge carriers** that can move through the material.
3. More light → more free electrons → current flows more easily → **low resistance**.

This effect is called **photoconductivity**. The LDR doesn't generate power from light — it just gets *easier or harder* to push current through, depending on brightness. Our job is to turn that changing resistance into a number the Arduino can read.

---

## ⚡ The trick: turning resistance into voltage

Here's the catch — **the Arduino can't measure resistance directly.** Its analog pins only measure *voltage* (0 V to 5 V). So we need to convert the LDR's changing resistance into a changing voltage. We do that with a **voltage divider**.

A voltage divider is just two resistors in series between 5 V and GND. The voltage at the point *between* them depends on the ratio of the two resistances:

```
5V ──[ LDR ]──┬──[ 10kΩ ]── GND
              │
              └── to A1   (this midpoint voltage is what we read)
```

- **Bright room:** LDR resistance is low → most of the 5 V "drops" across the 10 kΩ resistor → the midpoint sits **high** → `analogRead` returns a **big** number.
- **Dark room:** LDR resistance is high → most of the 5 V drops across the LDR → the midpoint sits **low** → `analogRead` returns a **small** number.

So with this wiring, **dark = low value, bright = high value**. That's exactly why the code turns the LED on when the reading is *below* the threshold.

> 💡 The 10 kΩ resistor is the LDR's partner. It's chosen so the midpoint voltage swings across a wide, usable range instead of bunching up near 0 or 5 V. If your readings feel cramped, swapping this resistor is the first fix to try.

---

## 🧰 Parts

| Part | Qty | Notes |
|------|-----|-------|
| Arduino Uno / Nano | 1 | Whatever board you're using |
| Photoresistor (LDR, e.g. GL5528) | 1 | The light sensor |
| 10 kΩ resistor | 1 | Pairs with the LDR for the voltage divider |
| LED | 1 | Any color |
| 220 Ω resistor | 1 | Current-limiting resistor for the LED |
| Breadboard + jumper wires | — | ~6 wires |

Everything here is in a standard Arduino starter kit.

---

## 🔌 Wiring

**The voltage divider (sensor side)**
1. One leg of the **LDR** → **5V**.
2. The other leg of the LDR → a spot on the breadboard. Call this the **midpoint**.
3. From the midpoint, a **10 kΩ resistor** → **GND**.
4. Also from the midpoint, a jumper → **A1**.

**The LED (output side)**
5. Arduino **pin 3** → **220 Ω resistor** → LED **long leg (+, anode)**.
6. LED **short leg (–, cathode)** → **GND**.

That midpoint between the LDR and the 10 kΩ resistor is the whole point — it's the voltage that moves with the light, the same idea as the potentiometer's wiper from Day 4, just automated by the environment instead of your fingers.

---

## 💻 The code, explained

```cpp
int ledRED = 3;   // LED output (PWM pin)
int ldr = A1;     // Analog input from the voltage divider
```
Two variables name our pins so the code reads clearly. Pin 3 has a `~` next to it on the board, meaning it supports **PWM** — not needed for simple on/off, but it leaves the door open to *fading* the LED later.

```cpp
void setup() {
  pinMode(ledRED, OUTPUT);
  pinMode(ldr, INPUT);
  Serial.begin(9600);
}
```
`setup()` runs once. We tell the board pin 3 is an output (it sends signals *out* to the LED) and A1 is an input (it reads a value *in* from the sensor). `Serial.begin(9600)` opens a link to your computer so you can watch the sensor values — essential for calibration (below).

```cpp
void loop() {
  int res = analogRead(ldr);
```
`loop()` repeats forever. `analogRead()` measures the voltage at A1 and maps it to a whole number from **0 to 1023** (that's the Arduino's 10-bit ADC — 2¹⁰ = 1024 possible values). We store it in `res`.

```cpp
  if (res < 300) {
    digitalWrite(ledRED, HIGH);
  }
  else {
    digitalWrite(ledRED, LOW);
  }
}
```
This is the **decision** — the new skill for Day 5. If the light reading is below 300, it's dark enough, so drive the LED pin `HIGH` (on). Otherwise, `LOW` (off). `300` isn't magic — it's a threshold tuned to one specific room. Yours will differ.

---

## 🎚️ Calibrating your threshold (the important part)

The number `300` depends on **your** room, your LDR, and your resistor. Don't guess it — measure it. This "measure before you code" habit is the real lesson of Day 5.

Temporarily add one line inside `loop()`, right after the `analogRead`:

```cpp
  int res = analogRead(ldr);
  Serial.println(res);   // <-- add this line to see live values
```

Upload, then open **Tools → Serial Monitor** (baud 9600). Watch the number:

- **Room lights on / sensor uncovered** — note the value (say, ~600).
- **Cover the sensor with your hand** — note the value (say, ~120).

Pick a threshold **between** those two readings — around 300–350 in that example. Set it in the `if` statement, then delete the `Serial.println` line (or leave it; it's harmless). Done — your night light is tuned to your room.

---

## 🛠️ Troubleshooting

| Problem | Likely cause / fix |
|---------|--------------------|
| LED never turns on | Threshold too low — raise it, or check your dark reading in Serial |
| LED always on | Threshold too high, or LDR/10k swapped — check the wiring direction |
| Value barely changes | Voltage divider issue — confirm the midpoint goes to A1, try a different fixed resistor |
| LED flickers near the edge | The room is sitting right at the threshold — see "Level up" below |
| Nothing at all | LED backwards (long leg is +), or 220 Ω resistor missing |

---

## 🚀 Level up

Two upgrades once the basic version works:

**1. Stop the flicker (hysteresis).** Right at the threshold, tiny changes in light can make the LED flick on/off rapidly. Fix it with *two* thresholds — turn on below 300, turn off above 400 — so there's a "dead zone" it has to fully cross before switching back.

**2. Fade instead of snap.** Since the LED is on a PWM pin, you can `map()` the light level to a brightness (0–255) and `analogWrite()` it, so the light glows brighter as the room gets darker instead of a hard on/off. This reuses the exact `map()` idea from Day 4.

---

## 🏃 How to run

1. Wire the circuit as above.
2. Open `night_light.ino` in the Arduino IDE.
3. Select your board and port under **Tools**.
4. Hit **Upload**.
5. Open the Serial Monitor to calibrate, then cover the sensor and watch it work.

---

*Built by Ayagreeiv S.T. · Road to Electronics · [@electroniclearns](https://instagram.com/electroniclearns)*
