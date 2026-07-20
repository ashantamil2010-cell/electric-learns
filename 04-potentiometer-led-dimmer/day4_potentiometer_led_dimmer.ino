/*
 * Day 4 — Potentiometer LED Dimmer
 * Road to Electronics series
 *
 * Controls LED brightness with a potentiometer using PWM.
 * The pot's wiper voltage (0–5V) is read as 0–1023, mapped
 * to 0–255, and written to a PWM pin to dim the LED.
 *
 * Board:  Arduino Nano (works on Uno too)
 * Author: Aya (@electroniclearns)
 * Repo:   arduino-beginner-projects
 *
 * Wiring:
 *   LED (+) -> 220Ω resistor -> D3 (PWM pin, marked with ~)
 *   LED (-) -> GND
 *   Pot outer pins -> 5V and GND
 *   Pot middle pin (wiper) -> A1
 */

const int LED_PIN = 3;   // Must be a PWM pin: 3, 5, 6, 9, 10, or 11
const int POT_PIN = A1;  // Analog input (no pinMode needed)

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Read knob position: 0 (0V) to 1023 (5V)
  int sensorValue = analogRead(POT_PIN);

  // Rescale to PWM range: 0 (off) to 255 (full brightness)
  int brightness = map(sensorValue, 0, 1023, 0, 255);

  // Print for Serial Monitor / Serial Plotter
  Serial.println(brightness);

  // PWM: pin toggles ~490x/sec; duty cycle sets perceived brightness
  analogWrite(LED_PIN, brightness);

  delay(10);  // Small delay keeps Serial Plotter readable
}
