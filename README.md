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
- timeout for aborted taps, removed too-slow taps. Min range 30 BPM
- connect display
- connect microSD to read out and play 8-bit mono wav
- connect output to play simple sound

## ToDo
- put LED on a different port, as port 13 is busy with SD
- be able to stop the stomp :-D
- better sound
- average 3 taps
- tidy display output, display decimal BPM
- output volume control
- improve sound (use 16-bit sound, build DAC)
- burn bootloader on chip
- put on stripboard
- put into enclosure