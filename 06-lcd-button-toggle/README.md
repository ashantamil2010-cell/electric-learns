# Day 6 — LCD Button Toggle 🖥️

A push button toggles an LED, and a 16×2 LCD shows what's going on: how many times you've pressed it, and whether the LED is currently on or off.

Up to now, the only way to see what the Arduino was thinking was the Serial Monitor — which means being tied to a laptop. This is the day the project starts talking on its own.

> **Road to Electronics** — learning electronics from scratch, no AI, just docs and datasheets.

---

## 🎯 What it does

- Press the button → the LED toggles on or off.
- The LCD's top line counts total presses.
- The LCD's bottom line reports the LED's current state.
- Release and press again → it toggles once, not repeatedly.

This is **Day 2's pushbutton toggle with a screen bolted on** — same core idea, but now the circuit can report on itself.

---

## 🖥️ What is an LCD display?

**LCD** stands for **Liquid Crystal Display**. The kind used here is a **16×2 character LCD** — 16 columns by 2 rows, 32 characters total. It can't draw pictures or arbitrary graphics; it draws *characters* from a built-in font, in fixed grid cells.

### The HD44780 — the chip that matters

Almost every text LCD you'll meet is built around the **Hitachi HD44780** controller chip (or a clone of it). This is one of those parts that became so standard it outlived its own company — it's been in production since the 1980s and it's still what's in the module in your starter kit.

You can usually spot one by its **16-pin interface** along the top edge.

Why this matters: because they all speak the same protocol, **one library drives all of them**. That's what `LiquidCrystal.h` is — Arduino's built-in library for HD44780-compatible displays. Buy a different brand of 16×2, and the same code just works.

The display has its own small brain and its own memory. You're not constantly redrawing the screen — you send a character once, the HD44780 stores it in its display memory (DDRAM) and keeps showing it forever until you overwrite it. **That fact is the key to the flicker fix later in this README.**

### The 16 pins

| Pin | Name | What it's for |
|-----|------|---------------|
| 1 | VSS | Ground |
| 2 | VDD | +5V power |
| 3 | V0 | **Contrast** — this is the one that gets everyone |
| 4 | RS | Register Select |
| 5 | R/W | Read / Write select |
| 6 | E | Enable |
| 7–14 | D0–D7 | The eight data lines |
| 15 | A | Backlight + (anode) |
| 16 | K | Backlight – (cathode) |

Three of those need explaining properly.

### RS — Register Select

The LCD has to be able to tell the difference between **"display this letter"** and **"move the cursor"**. Both arrive on the same data wires, so how does it know which is which?

That's the RS pin. It selects which internal register your data is headed for:

- **RS = LOW → instruction register.** The display treats the incoming byte as a *command* — clear the screen, move the cursor, turn the display off.
- **RS = HIGH → data register.** The display treats the byte as a *character* to draw.

So `lcd.print("Hi")` pulls RS high and sends characters. `lcd.setCursor(0,1)` pulls RS low and sends a command. Same wires, different meaning, one pin deciding.

### E — Enable

The data lines are just sitting there with voltages on them all the time. The Enable pin is the "**now**" signal — the LCD only actually reads the data lines on the moment E pulses. It's the shutter click. Set up your data, pulse E, the display latches it in.

### R/W — Read/Write

You *can* read data back out of the LCD. Almost nobody does. So this pin gets **wired straight to GND**, permanently selecting write mode, and that frees up an Arduino pin. Arduino's official example does exactly this — which is why the pin never appears in your code.

---

## 🔢 Why only 6 wires? (4-bit mode)

Look at the pin table again: there are **eight** data lines, D0–D7. But the sketch only uses four of them, and the constructor only lists six pins total:

```cpp
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//                RS   E  D4 D5 D6 D7
```

The LiquidCrystal library can run in either **8-bit mode** or **4-bit mode**:

- **8-bit mode** — all eight data lines wired up. A byte goes across in one shot. Costs you 8 Arduino pins (+2 control) = **10 pins**.
- **4-bit mode** — only D4–D7 wired. Each byte gets split in half and sent as two nibbles, high half first, then low half. Costs **6 pins**.

The tradeoff: 4-bit mode takes twice as many transfers per character. But "twice as slow" here means microseconds, and a human staring at a text display cannot possibly notice. Meanwhile you just got **four Arduino pins back** — which on an Uno with 14 digital pins is a huge deal. You're already spending 6 on the screen; spending 10 would leave you almost nothing for the actual project.

**This is a real engineering tradeoff, and it's the first one in this series where the "worse" option is obviously the right one.** Nearly everyone uses 4-bit mode. D0–D3 just get left unconnected.

> 📌 Coming in **Day 8**: the same screen over **I²C**, which does the whole job on **2 wires** using a little adapter chip. Going 6 → 2 will feel great after wiring this one by hand.

---

## 🎚️ The contrast pin (and why your screen might show nothing)

**Pin 3 (V0) sets the contrast, and it is not optional.** It needs a specific voltage, and the standard way to supply an adjustable one is a **10 kΩ potentiometer**:

- Outer two pins → **5V** and **GND**
- Middle pin (wiper) → **LCD pin 3 (V0)**

If you skip this, or leave V0 floating, you get the classic beginner experience: either a completely blank screen, or a row of solid white blocks. **The code is running perfectly. You just can't see it.** More beginners have given up on LCDs over this one pin than anything else.

The fix is to power it up and slowly turn the pot until the text appears at a comfortable darkness.

> 🔁 **Series callback:** that's the same potentiometer from Day 4 — but doing something completely different. On Day 4 the Arduino *read* the pot with `analogRead()`. Here the Arduino doesn't know the pot exists. It's a pure analog voltage divider feeding the display directly. Same part, totally different job.

---

## 🧰 Parts

| Part | Qty | Notes |
|------|-----|-------|
| Arduino Uno / Nano | 1 | |
| 16×2 LCD (HD44780) | 1 | The 16-pin one from any starter kit |
| 10 kΩ potentiometer | 1 | Contrast control for V0 |
| Push button | 1 | Momentary tactile switch |
| LED | 1 | Any color |
| 220 Ω resistor | 1 | Current limiting for the LED |
| Breadboard + jumper wires | — | ~16 wires — this is the biggest build yet |

Note there's **no resistor on the button** — that's on purpose, explained below.

---

## 🔌 Wiring

### The LCD (matches Arduino's official example)

| LCD pin | Goes to |
|---------|---------|
| 1 (VSS) | GND |
| 2 (VDD) | 5V |
| 3 (V0) | Potentiometer **wiper** (middle pin) |
| 4 (RS) | Digital **12** |
| 5 (R/W) | **GND** |
| 6 (E) | Digital **11** |
| 11 (D4) | Digital **5** |
| 12 (D5) | Digital **4** |
| 13 (D6) | Digital **3** |
| 14 (D7) | Digital **2** |
| 15 (A) | 5V |
| 16 (K) | GND |

D0–D3 (pins 7–10) stay **unconnected** — that's 4-bit mode.

> ⚠️ Some LCD modules don't have a backlight resistor built in. If yours doesn't, put a **220 Ω** resistor between 5V and pin 15 so you don't cook the backlight LED.

### Contrast potentiometer
- Outer pin → 5V
- Outer pin → GND
- Wiper (middle) → LCD pin 3

### The button
- One leg → Digital **8**
- Other leg → **GND**

No resistor needed — see below.

### The LED
- Digital **9** → 220 Ω resistor → LED **long leg (+)**
- LED **short leg (–)** → GND

---

## ⬆️ INPUT_PULLUP — why the button has no resistor

A floating input pin is a genuine problem: with nothing connected, it drifts and picks up electrical noise, so `digitalRead()` returns garbage. A button only connects the pin to something while it's being *pressed* — so the rest of the time, the pin is floating.

The classic fix is an external pull-up resistor holding the pin at 5V. But the ATmega chip has pull-up resistors **built in**, and one line switches them on:

```cpp
pinMode(buttonPin, INPUT_PULLUP);
```

Now the pin is quietly held HIGH from the inside. Press the button, and it connects the pin directly to GND, which wins — the pin reads LOW.

**This inverts the logic, and it trips people up constantly:**

- **Not pressed → HIGH**
- **Pressed → LOW**

Which is exactly why the code checks `if (buttonState == LOW)`.

---

## 💻 The code, explained

### Setting up

```cpp
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
```

Pull in the library, then create an `lcd` object, telling it which pins you used — **in the order RS, E, D4, D5, D6, D7**. Get this order wrong and you'll get garbage characters or nothing at all.

```cpp
lcd.begin(16, 2);
```

Tell the library the display's shape: 16 columns, 2 rows.

```cpp
lcd.setCursor(0, 0);
lcd.print("Presses:");
```

`setCursor(column, row)` moves the invisible write position. **Both are zero-indexed** — top-left is `(0, 0)`, and the bottom row is row **1**, not row 2.

This label is printed **once in `setup()`**, not in the loop, because it never changes. Remember the HD44780 has its own memory — once "Presses:" is on the screen it stays there with no further help. Reprinting it every loop is pure wasted work.

### The main loop

```cpp
buttonState = digitalRead(buttonPin);

if (buttonState == LOW && lastButtonState == HIGH) {
```

This is the heart of it: **edge detection**.

There's a big difference between *"is the button down?"* and *"did the button just go down?"* The loop runs thousands of times a second, so if you only ask the first question, a single half-second press looks like **hundreds** of presses in a row.

By storing the previous reading in `lastButtonState`, you can spot the exact transition from HIGH (not pressed) to LOW (pressed). That happens **once per physical press**, no matter how long you hold it.

```cpp
    delay(50);   // debounce
```

Button contacts are metal springs. When they touch, they physically bounce, making and breaking contact several times over a few milliseconds. To the Arduino that looks like several presses. Waiting 50 ms lets the bouncing settle.

```cpp
    pushAmount++;
    ledOn = !ledOn;
    digitalWrite(ledRED, ledOn ? HIGH : LOW);
    updateDisplay();
```

Count the press, flip the LED state with the `!` (NOT) operator, push that state to the pin, and refresh the screen.

```cpp
lastButtonState = buttonState;
```

Remember this reading so the next pass can compare against it. **Miss this line and edge detection silently stops working.**

### Updating the display

```cpp
lcd.setCursor(9, 0);
lcd.print(pushAmount);
lcd.print("      ");
```

Jump to column 9 (right after `Presses:`) and print the count. Those trailing spaces matter: the LCD only changes cells you write to. Going from `10` back to `9` would otherwise leave the old `0` sitting there, showing `90`.

```cpp
lcd.print(ledOn ? "LED: ON " : "LED: OFF");
```

Note `"LED: ON "` has a **trailing space** so both strings are exactly 8 characters. Equal width means the new text completely covers the old text — no leftovers.

**And that's the flicker fix.** `lcd.clear()` wipes the entire screen, so if you call it every loop the display spends part of every cycle blank — which your eye reads as flickering. It's also one of the slowest commands the HD44780 has (~1.5 ms). By padding strings to equal width, `clear()` becomes unnecessary. Combined with only redrawing when something actually changes, the screen becomes perfectly steady.

> **The rule worth remembering: never redraw a screen that's already correct.**

---

## 🛠️ Troubleshooting

| Problem | Likely cause / fix |
|---------|--------------------|
| Blank screen, backlight on | **Contrast.** Turn the 10 kΩ pot. This is the #1 issue |
| Row of solid blocks | Same — contrast pot needs adjusting |
| Nothing at all, no backlight | Check VDD/VSS (pins 1 & 2) and backlight pins 15/16 |
| Garbled/random characters | Pin order wrong in `LiquidCrystal(...)` — it's RS, E, D4, D5, D6, D7 |
| Text on wrong row | `setCursor` is zero-indexed — bottom row is `1` |
| Old digits linger | Not enough trailing spaces after the number |
| Screen flickers | You're calling `lcd.clear()` in the loop — don't |
| LED toggles while held | Edge detection missing, or you forgot `lastButtonState = buttonState;` |
| One press counts as several | Debounce delay too short |
| Button does nothing | Check `INPUT_PULLUP` is set and the button goes to **GND**, not 5V |

---

## 🚀 Level up

1. **Kill the `delay()`.** `delay(50)` freezes the whole board. Switch to a `millis()` timestamp comparison and the Arduino stays responsive — the standard non-blocking pattern, and the thing that makes multitasking possible.
2. **Add a reset button** that zeroes `pushAmount`.
3. **Custom characters.** The HD44780 lets you define up to 8 of your own 5×8 glyphs with `lcd.createChar()` — draw a little LED icon that fills in when it's on.
4. **Show live sensor data** — swap the press counter for your Day 5 LDR reading and watch it update in real time. That's your Day 8 project in miniature.

---

## 📚 Official documentation

- **Arduino — Liquid Crystal Displays (LCD):** https://docs.arduino.cc/learn/electronics/lcd-displays
- **LiquidCrystal library reference:** https://docs.arduino.cc/libraries/liquidcrystal/
- **Library source:** https://github.com/arduino-libraries/LiquidCrystal

---

## 🏃 How to run

1. Wire it up as above — take your time, this is ~16 connections.
2. Open `lcd_button_toggle.ino` in the Arduino IDE.
3. `LiquidCrystal` is built in, so there's nothing to install.
4. Pick your board and port under **Tools**.
5. **Upload.**
6. If the screen looks blank, **turn the contrast pot before assuming anything is broken.**

---

*Built by Aya S.T. · Road to Electronics · [@electroniclearns](https://instagram.com/electroniclearns)*
