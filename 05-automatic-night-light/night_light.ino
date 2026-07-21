/*
 * ============================================================
 *  Day 5 — Automatic Night Light
 *  Road to Electronics  ·  @electroniclearns
 * ------------------------------------------------------------
 *  An LED that turns itself on when the room gets dark, using a
 *  photoresistor (LDR) to sense light. No buttons, no knobs —
 *  the environment makes the decision.
 *
 *  New concept vs. Day 4: instead of a potentiometer that YOU
 *  turn, a light sensor changes the analog value on its own.
 *  Then an `if` statement makes the board react on its own.
 *
 *  Built by Aya S.T. — learning electronics without AI.
 * ============================================================
 */

int ledRED = 3;   // LED output. Pin 3 is a PWM (~) pin — handy if you
                  // later want to FADE the LED instead of just on/off.
int ldr = A1;     // Analog input: the junction of the LDR and the 10k resistor.

void setup() {
  pinMode(ledRED, OUTPUT);
  pinMode(ldr, INPUT);
  Serial.begin(9600);   // Open the Serial Monitor to calibrate the threshold.
}

void loop() {
  int res = analogRead(ldr);   // Read the light level: 0 (dark) .. 1023 (bright).

  // If it's darker than our threshold, turn the light ON. Otherwise OFF.
  // 300 was chosen by watching real values in MY room — see the README
  // for how to find the right number for yours.
  if (res < 300) {
    digitalWrite(ledRED, HIGH);
  }
  else {
    digitalWrite(ledRED, LOW);
  }
}
