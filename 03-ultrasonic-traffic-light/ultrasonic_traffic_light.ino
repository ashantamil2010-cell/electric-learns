// ============================================================
// Ultrasonic Distance Traffic Light
// ------------------------------------------------------------
// Uses an HC-SR04 ultrasonic sensor to measure distance, then
// lights one of three LEDs like a traffic light:
//   - RED    = object closer than 10 cm  (too close!)
//   - YELLOW = object between 10 and 20 cm (getting close)
//   - GREEN  = object 20 cm or farther away (all clear)
// ============================================================

// ---- Pin assignments ----
// The HC-SR04 has two signal pins:
//   TRIG (trigger) - we send a pulse OUT on this pin to tell
//                    the sensor to fire an ultrasonic chirp
//   ECHO           - the sensor sends a pulse back IN on this
//                    pin; its length = round-trip travel time
int trig = 9;      // TRIG pin -> Arduino digital pin 9
int echo = 8;      // ECHO pin -> Arduino digital pin 8

// Each LED (with a resistor!) connects to its own digital pin
int redLED = 4;    // lights when object is very close
int yellowLED = 3; // lights when object is medium distance
int greenLED = 2;  // lights when path is clear

void setup() {
  // setup() runs ONCE when the Arduino powers on or resets.
  // Here we tell the Arduino which pins are inputs vs outputs.

  pinMode(trig, OUTPUT);  // we WRITE to trig (send the pulse)
  pinMode(echo, INPUT);   // we READ from echo (time the reply)

  // All three LED pins are outputs since we control them
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  // Start with the trigger pin LOW so our first measurement
  // begins with a clean rising edge
  digitalWrite(trig, LOW);
}

void loop() {
  // loop() repeats forever. Each pass = one distance
  // measurement + one LED update.

  // ---- Step 1: Fire the sensor ----
  // Holding TRIG high for 10 microseconds tells the HC-SR04
  // to send out a burst of 8 ultrasonic pulses (40 kHz sound,
  // too high-pitched for humans to hear).
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // ---- Step 2: Time the echo ----
  // The sensor raises ECHO high when the sound leaves and
  // drops it low when the reflection comes back. pulseIn()
  // measures how long ECHO stayed high, in microseconds.
  //
  // We use "long" (not "int") because on an Arduino Nano an
  // int only holds up to 32,767 -- but pulseIn can return up
  // to 1,000,000 us if no echo comes back. long holds it fine.
  long duration = pulseIn(echo, HIGH);

  // ---- Step 3: Convert time to distance ----
  // Sound travels about 0.034 cm per microsecond.
  // duration * 0.034 = TOTAL distance the sound traveled,
  // but that's a round trip (out and back), so divide by 2
  // to get the one-way distance to the object.
  int distance = duration * 0.034 / 2;

  // ---- Step 4: Traffic light logic ----
  // Only ONE branch of this if/else-if/else chain runs each
  // loop. Each branch turns its LED on and the other two off,
  // so exactly one LED is lit at a time.

  if (distance < 10) {
    // Object is closer than 10 cm -> RED (danger zone)
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
  } else if (distance < 20) {
    // We only reach here if distance is NOT < 10, so this
    // branch means 10 to 19 cm -> YELLOW (warning zone)
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(greenLED, LOW);
  } else {
    // Anything 20 cm or more -> GREEN (all clear)
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, HIGH);
  }

  // ---- Step 5: Small pause ----
  // 5 ms between readings. Short enough that the LEDs react
  // almost instantly, long enough to let old echoes fade so
  // one measurement doesn't interfere with the next.
  delay(5);
}
