/*
 * Joystick Control in 30 Seconds
 * ------------------------------------------------------
 * X-axis of joystick sweeps a servo left/right.
 * Y-axis lights a green or red LED depending on direction.
 * Pressing the joystick button flashes both LEDs as a confirm signal.
 *
 * Hardware:
 *   - Arduino Nano
 *   - KY-023 joystick module (X, Y, button)
 *   - Servo motor
 *   - Green LED
 *   - Red LED
 *   - 330 ohm resistors (x2, for LEDs)
 *   - Breadboard
 *   - Male-to-male jumper wires
 *   - Male-to-female jumper wires
 *
 * Road to Electronics - Day 8
 * github.com/electric-learns
 */

#include <Servo.h>

Servo myservo;

// --- Pin assignments ---
int yPin = A1;         // Joystick Y-axis (analog)
int xPin = A2;         // Joystick X-axis (analog)
int buttonPin = 7;     // Joystick pushbutton (digital, active LOW)

int greenLED = 2;
int redLED = 3;
int servoPin = 9;

// --- Sensor readings ---
int yVal;
int xVal;
int buttonState;

// --- Servo position, starts centered ---
int servoPos = 90;

void setup() {
  Serial.begin(9600);

  myservo.attach(servoPin);

  pinMode(yPin, INPUT);
  pinMode(xPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);  // pressed = LOW

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
}

void loop() {
  // Read joystick + button state each cycle
  yVal = analogRead(yPin);
  xVal = analogRead(xPin);
  buttonState = digitalRead(buttonPin);

  // --- X-axis: sweep servo position ---
  // Deadzone is 300-700 so small drift near center doesn't cause drift
  if (xVal <= 300) {
    servoPos--;              // push left -> decrease angle
  } else if (xVal >= 700) {
    servoPos++;              // push right -> increase angle
  } else {
    servoPos = servoPos;     // inside deadzone, hold position
  }

  // Clamp to valid servo range
  if (servoPos > 180) {
    servoPos = 180;
  }
  if (servoPos < 0) {
    servoPos = 0;
  }

  myservo.write(servoPos);

  // --- Y-axis: green/red LED direction indicator ---
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

  // --- Button press: quick double-blink confirm flash ---
  if (buttonState == 0) {
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, LOW);
    delay(50);
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, HIGH);
    delay(50);
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, LOW);
    delay(50);
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, HIGH);
  }
}
