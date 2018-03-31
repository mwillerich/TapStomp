/*
  TapStomp

  Turns on and off a light emitting diode(LED) connected to digital pin 13 in regular intervals,
  which are controlled by pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground
  - pushbutton attached to Digital pin 2 from +5V
  - 10K resistor attached to pin 2 from ground
  - alphanumeric display on ground, 2x+5V, DAT D on Analog 4, CLK C on Analog 5

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.

  - uses 
    - https://www.arduinolibraries.info/libraries/everytime
    - https://www.arduinolibraries.info/libraries/tm-rpcm
  
  created 2018
  by Matthias Willerich <http://gain-volume.com>

  https://github.com/mwillerich/TapStomp
*/

#include "everytime.h"
#include <avr/sleep.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#include <SD.h>                      // need to include the SD library
#define SD_ChipSelectPin 10
#include <TMRpcm.h>           //  also need to include this library...
#include <SPI.h>

TMRpcm tmrpcm;   // create an object for use in this sketch

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

//pin definitions
const int buttonPin = 2;     // the number of the pushbutton pin
//TODO pin13 is now busy with SD card
const int ledPin =  13;      // the number of the LED pin

const bool debug = true;

bool ledState = HIGH;         // the current state of the output pin
bool buttonState = 0;         // the current reading from the input pin
bool lastButtonState = LOW;   // the previous reading from the input pin

int interval = 500;          // current BPM in millis
int minInterval = 250;       // minimum interval = maximum BPM = 240
int maxInterval = 2000;      // maximum interval = minimum BPM = 30
float bpm;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
unsigned long firstMillis = 0;       // the last time the output pin was toggled
unsigned long secondMillis = 0;      // the last time the output pin was toggled
unsigned long currentMillis = 0;     // the last time the output pin was toggled

void displayBPM(int bpm) {
  
  String strBpm;
  char cBpm[4];
  strBpm = String(bpm);

  strBpm.toCharArray(cBpm,4);

  Serial.print("display out: ");
  Serial.println(cBpm);
  // set every digit to the buffer
  alpha4.writeDigitAscii(0, cBpm[0]);
  alpha4.writeDigitAscii(1, cBpm[1]);
  alpha4.writeDigitAscii(2, cBpm[2]);
  alpha4.writeDigitAscii(3, cBpm[3]);
 
  // write it out!
  //alpha4.clear();
  alpha4.writeDisplay();
}

int minMillis(int nextInterval) {
  if(nextInterval < minInterval) {
    Serial.print("Outside min: ");
    Serial.println(nextInterval);
    nextInterval = 250;
  }
  return nextInterval;
}

void countBPM() {
  currentMillis = millis();
  if(firstMillis == 0) {
    firstMillis = currentMillis;
    secondMillis = 0;
  } else if(secondMillis == 0) {
	//set BPM
    secondMillis = currentMillis;
    int diff = secondMillis - firstMillis;
    Serial.print("Button millis diff: ");
    Serial.println(diff);
    interval = minMillis(diff);
	bpm = (float)1000 / interval * (float)60;
	displayBPM(bpm);
    secondMillis = firstMillis = 0;
  }
}

void cancelBPMCount() {
  if(firstMillis != 0 and secondMillis == 0) {
    if(millis() - firstMillis > maxInterval) {
      Serial.print("Outside max, cancel firstMillis: ");
      Serial.println((millis() - firstMillis));
      secondMillis = firstMillis = 0;
    }
  }
}

void deBouncedButton(void (*function)()) {
  int currentButtonState = digitalRead(buttonPin);

  // If the switch changed, due to noise or pressing
  if (currentButtonState != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state

    // if the button state has changed
    if (currentButtonState != buttonState) {
      buttonState = currentButtonState;
      Serial.print("Button press: ");
      Serial.println(buttonState);
      if (buttonState == HIGH) {
        (*function)();
      }
    }
  }

  lastButtonState = currentButtonState;
}

void milliSecondSleep() {
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
  sleep_mode(); // put the device to sleep
  sleep_disable();
}

void executeBeat() {
    Serial.print("interval BPM: ");
    Serial.print(bpm);
    Serial.print(" Millis: ");
    Serial.println(millis());
    ledState = !ledState;
    digitalWrite(ledPin ,ledState);
    tmrpcm.play("bassdrum.wav");
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  if(debug) {
  	Serial.begin(9600);
  }
  alpha4.begin(0x70);  // pass in the address
  tmrpcm.speakerPin = 9;
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not
  }
}

void loop() {
  static bool ledState = false;

  every(interval) {
      executeBeat();
  }

  deBouncedButton(countBPM);
  cancelBPMCount();
  milliSecondSleep();
}
