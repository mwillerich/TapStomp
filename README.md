# TapStomp

Arduino Code, ToDo List and Documentation for a Tap Tempo/Metronome/Stomp Box Guitar Pedal.

## Decisions
- how many Buttons should the pedal be operated with? One (taps to determine speed, hold to stop), Two?
- should the display show the BPM as they are played? (yes, but it may be whole or partial)
- should the BPM be as exact as possible, or rounded?
- micros vs millis

## Done
- simple proof-of-concept hardware on breadboard: Arduino Uno, momentary push button, pull-down resistor, on-board LED
- simple tap: Every second tap sets the speed of an alternating LED
- amend too-quick and too-slow taps. Max/min range from 30 to 240 BPM

## ToDo
- average 3 taps
- timeout for aborted taps (max/min range)
- connect display
- connect output to play simple sound
- connect microSD to read out and play 8-bit mono wav
- output volume control
- burn bootloader on chip
- put on stripboard
- put into enclosure