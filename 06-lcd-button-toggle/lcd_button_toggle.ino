/*
 * ============================================================
 *  Day 6 — LCD Button Toggle
 *  Road to Electronics  ·  @electroniclearns
 * ------------------------------------------------------------
 *  A push button toggles an LED, and a 16x2 LCD reports what's
 *  happening: how many times the button has been pressed, and
 *  whether the LED is currently on or off.
 *
 *  New concept vs. Day 5: an actual SCREEN. Up to now the only
 *  way to see what the Arduino was thinking was the Serial
 *  Monitor, which means being tethered to a laptop. An LCD makes
 *  the project standalone — it can tell you what it's doing on
 *  its own.
 *
 *  This is Day 2's pushbutton toggle, but now it can talk.
 *
 *  Built by Aya S.T. — learning electronics without AI.
 * ============================================================
 */

#include <LiquidCrystal.h>

// ---- Pins ----
const int ledRED    = 9;   // LED through a 220 ohm resistor
const int buttonPin = 8;   // Button to GND, using the internal pull-up

// LiquidCrystal(RS, E, D4, D5, D6, D7)
// This is the standard wiring from Arduino's own LCD example.
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// ---- State ----
int  buttonState;
int  lastButtonState = HIGH;  // INPUT_PULLUP idles HIGH (not pressed)
int  pushAmount      = 0;     // how many presses so far
bool ledOn           = false; // current LED state

void setup() {
  pinMode(ledRED, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  lcd.begin(16, 2);           // 16 columns, 2 rows

  // The "Presses:" label never changes, so print it ONCE here
  // instead of re-printing it every loop. Less work, no flicker.
  lcd.setCursor(0, 0);
  lcd.print("Presses:");

  updateDisplay();
}

void loop() {
  buttonState = digitalRead(buttonPin);

  // EDGE DETECTION.
  // We don't ask "is the button down?" — we ask "did it JUST go
  // down?", by comparing this reading to the previous one.
  // HIGH -> LOW is the exact moment of the press.
  // Without this, holding the button would toggle over and over.
  if (buttonState == LOW && lastButtonState == HIGH) {
    delay(50);                // debounce: ride out the contact bounce

    pushAmount++;
    ledOn = !ledOn;           // flip it: false->true, true->false
    digitalWrite(ledRED, ledOn ? HIGH : LOW);

    updateDisplay();
  }

  lastButtonState = buttonState;   // remember for next time around
}

// Only called when something actually CHANGED. That's the trick to
// a flicker-free display: never redraw a screen that's already correct.
void updateDisplay() {
  lcd.setCursor(9, 0);        // just after "Presses:"
  lcd.print(pushAmount);
  lcd.print("      ");        // trailing spaces wipe leftover digits
                              // (so 10 -> 9 doesn't leave "90")

  lcd.setCursor(0, 1);
  // Both strings are padded to the SAME width (8 characters), so the
  // new one fully covers the old one. This is why we never need
  // lcd.clear() — and clear() is what caused the flicker.
  lcd.print(ledOn ? "LED: ON " : "LED: OFF");
}
