/*
 * ============================================================
 *  Day 7 — Two-Button Servo Control
 *  Road to Electronics  ·  @electroniclearns
 * ------------------------------------------------------------
 *  Two push buttons drive a servo motor: hold LEFT and the arm
 *  sweeps one way, hold RIGHT and it sweeps back. Let go and it
 *  holds its position.
 *
 *  New concept vs. everything before: MOTION. Every project so
 *  far ended at a light or a screen. This is the first time the
 *  Arduino moves something in the physical world.
 *
 *  Reuses Day 2's buttons (INPUT_PULLUP) as the input, with a
 *  servo as the brand-new output.
 *
 *  Built by Aya S.T. — learning electronics without AI.
 * ============================================================
 */

#include <Servo.h>

Servo myServo;

int pos = 90;                    // start centered (0-180 range, 90 = middle)
const int servoPin = 9;          // servo signal wire

const int leftButton  = 3;       // hold to move toward 0
const int rightButton = 2;       // hold to move toward 180

void setup() {
  myServo.attach(servoPin);      // connect the servo object to pin 9
  myServo.write(pos);            // send it to the starting position

  // Buttons to GND using the chip's internal pull-ups:
  // not pressed = HIGH, pressed = LOW. No external resistors.
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
}

void loop() {
  int leftState  = digitalRead(leftButton);
  int rightState = digitalRead(rightButton);

  // NOTE: unlike Day 6, this is NOT edge detection — and that's on
  // purpose. Here we WANT continuous movement while the button is
  // held down, so we check "is it down?" every loop, not "did it
  // just go down?". Each pass nudges the position by one degree.

  // The "pos > 0" / "pos < 180" checks clamp the angle to the
  // servo's real range so we never command it past its limits.
  if (leftState == LOW && pos > 0) {
    pos--;
  }

  if (rightState == LOW && pos < 180) {
    pos++;
  }

  myServo.write(pos);            // push the (possibly updated) angle out

  delay(10);                     // 10 ms/step -> a full 0..180 sweep
                                 // takes ~1.8 s of holding. Bigger delay
                                 // = slower, more controllable movement.
}
