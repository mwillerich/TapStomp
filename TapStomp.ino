/*
  TapStomp

  Turns on and off a light emitting diode(LED) connected to digital pin 13 in regular intervals,
  which are controlled by pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.

  - uses https://www.arduinolibraries.info/libraries/everytime
  
  created 2018
  by Matthias Willerich <http://gain-volume.com>

  https://github.com/mwillerich/TapStomp
*/

#include "everytime.h"
#include <avr/sleep.h>

//pin definitions
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

const bool debug = TRUE;

int ledState = HIGH;         // the current state of the output pin
int buttonState = 0;         // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int interval = 500;          // current BPM in millis

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
unsigned long firstMillis = 0;  // the last time the output pin was toggled
unsigned long secondMillis = 0;  // the last time the output pin was toggled
unsigned long currentMillis = 0;  // the last time the output pin was toggled


void countBPM() {
  currentMillis = millis();
  if(firstMillis == 0) {
    firstMillis = currentMillis;
    secondMillis = 0;
  } else if(secondMillis == 0) {
    secondMillis = currentMillis;
    Serial.print("button millis diff: ");
    Serial.println(secondMillis - firstMillis);
    interval = secondMillis - firstMillis;
    secondMillis = firstMillis = 0;
  }
}

void buttonStuff() {
  // read the ledState of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current ledState

    // if the button ledState has changed
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        Serial.print("button millis: ");
        Serial.println(millis());
        countBPM();
      }
    }
  }

  lastButtonState = reading;
}

void sleepBlock() {   //sleepblock put if(0) in front if u do not want the AVR to sleep
  /*     set sleep mode
  *     SLEEP_MODE_IDLE
  *        -the least power savings keeps io-clk running -> timer 0 ->
  *         millis and micros keep working (wakeup about every 1ms)
  *     SLEEP_MODE_ADC
  *     SLEEP_MODE_PWR_SAVE
  *     SLEEP_MODE_STANDBY
  *     SLEEP_MODE_PWR_DOWN     -the most power savings
  */
  set_sleep_mode(SLEEP_MODE_IDLE); //sleeps for the rest of this millisecond or less if other trigger
  sleep_enable();
  sleep_mode();            // put the device to sleep
  sleep_disable();
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  if(debug) {
  	Serial.begin(9600);
  }
}

void loop() {
  static uint8_t ledState = 0;

  every(interval){
      // blink (change ledState and write to led)
      Serial.print("interval BPM: ");
      Serial.print(((float)1000 / interval * (float)60));
      Serial.print(" Millis: ");
      Serial.println(millis());
      ledState = ! ledState;
      digitalWrite(ledPin ,ledState);
  }

  buttonStuff();
  sleepBlock();
}
