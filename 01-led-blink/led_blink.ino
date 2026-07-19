// ============================================================
// LED Blink
// ------------------------------------------------------------
// The "hello world" of Arduino: turns an LED on for one
// second, off for one second, and repeats forever.
// ============================================================

// ---- Pin assignment ----
// The LED's long leg (+) connects through a resistor to
// digital pin 7. The short leg (-) goes to GND.
int LED_PIN = 7;   // naming the LED's pin number

void setup() {
  // setup() runs ONCE when the Arduino powers on or resets.

  // Tell the Arduino this pin will be an OUTPUT -- meaning
  // we will be sending voltage OUT of it to power the LED,
  // not reading anything in from it.
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // loop() repeats forever, top to bottom, over and over.
  // One full pass = one blink cycle (on 1s + off 1s).

  // HIGH puts 5V on the pin -> current flows -> LED lights up
  digitalWrite(LED_PIN, HIGH);

  // delay() pauses the ENTIRE program. The number is in
  // milliseconds, so 1000 ms = 1 second of the LED staying on.
  delay(1000);

  // LOW puts 0V on the pin -> no current -> LED turns off
  digitalWrite(LED_PIN, LOW);

  // Wait another second with the LED off, then loop() starts
  // over from the top and the LED turns on again.
  delay(1000);
}
