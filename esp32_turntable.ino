// 360° Photo Booth Turntable Controller
// Based on ESP32-S3 and a 28BYJ-48 Stepper Motor
// By FromFilamentToForm, August 2025

// Required library for stepper control
#include <AccelStepper.h>

// --- Pin Definitions ---
// Define the pins for the ULN2003 Driver
#define IN1 19
#define IN2 18
#define IN3 5
#define IN4 17

// Define the pin for the push button
const int BUTTON_PIN = 10;

// --- Global Variables ---
// Initialize the stepper for the 28BYJ-48 in half-step mode.
// NOTE: This pin order was found to work with this specific setup.
// Your motor might require a different sequence (e.g., IN1, IN2, IN3, IN4).
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4); // This is an example, use your working combination!

bool motorActive = false;
long lastButtonTime = 0;
bool lastButtonState = HIGH;

void setup() {
  // Start serial communication for debugging (optional for final version)
  Serial.begin(115200);

  // Set the button pin as an input with an internal pull-up resistor
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Set your desired speed and acceleration
  stepper.setMaxSpeed(400.0);
  stepper.setAcceleration(400.0);
}

void loop() {
  // --- Read button and toggle motor state ---
  bool currentButtonState = digitalRead(BUTTON_PIN);

  // Debounce the button
  if (millis() - lastButtonTime > 50) {
    if (currentButtonState == LOW && lastButtonState == HIGH) {
      motorActive = !motorActive;

      if (motorActive) {
        stepper.moveTo(2000000000);
      } else {
        stepper.stop();
      }
      lastButtonTime = millis();
    }
  }
  lastButtonState = currentButtonState;

  // --- Execute motor movement ---
  stepper.run();
}
