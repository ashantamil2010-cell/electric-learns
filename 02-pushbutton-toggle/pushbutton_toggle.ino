// ============================================================
// Pushbutton LED Toggle
// ------------------------------------------------------------
// Counts how many times a button has been pressed. If the
// count is EVEN, the LED is on; if it's ODD, the LED is off.
// The effect: each press toggles the LED on/off like a switch.
// ============================================================

// ---- Pin assignments ----
int BUTTON_PIN = 3;  // one button leg -> pin 3, other leg -> GND
int LED_PIN = 7;     // LED (+ resistor) -> pin 7

// ---- Variables that remember things between loops ----
int pressedAmount = 0;  // running count of button presses
int buttonState = 0;    // what the button reads right now

void setup() {
  // The LED pin sends voltage out, so it's an OUTPUT
  pinMode(LED_PIN, OUTPUT);

  // INPUT_PULLUP turns on the Arduino's built-in pull-up
  // resistor. This holds the pin at HIGH normally, and the
  // button connects it to GND when pressed. That means:
  //   button NOT pressed -> reads HIGH
  //   button pressed     -> reads LOW   (yes, it's flipped!)
  // Without a pull-up, the pin would "float" and give random
  // readings when the button isn't pressed.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // Read the button's current state (HIGH or LOW) each pass
  buttonState = digitalRead(BUTTON_PIN);

  // Remember: with INPUT_PULLUP, LOW means PRESSED
  if (buttonState == LOW) {
    // Add 1 to the press counter
    // (++ is shorthand for pressedAmount = pressedAmount + 1)
    pressedAmount++;
  }

  // The % symbol is "modulo" -- it gives the REMAINDER after
  // division. pressedAmount % 2 is 0 for even numbers and 1
  // for odd numbers. So this checks: "is the count even?"
  if (pressedAmount % 2 == 0) {
    digitalWrite(LED_PIN, HIGH);  // even count -> LED on
  }
  else {
    digitalWrite(LED_PIN, LOW);   // odd count -> LED off
  }
}

// ------------------------------------------------------------
// NOTE: loop() runs thousands of times per second, so holding
// the button down counts as MANY presses, not one. This makes
// the toggle feel random. The fix is to only count the moment
// the button CHANGES from released to pressed (plus a small
// delay for "debouncing"). Great follow-up lesson!
// ------------------------------------------------------------
