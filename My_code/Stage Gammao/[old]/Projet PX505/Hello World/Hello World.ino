#include <MIDI.h>

// Set up MIDI over USB
MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  // Start the MIDI interface
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop() {
  // Send an aftertouch message on channel 1, note 60 (middle C), pressure 64, and cable 0
  MIDI.sendNoteOn(60, 64, 1);
  MIDI.sendControlChange(1, 127, 1);
  delay(500);
  MIDI.sendControlChange(1, 0, 1);
  delay(500);
  MIDI.sendControlChange(1, 64, 1);
  delay(500);
  MIDI.sendNoteOff(60, 0, 1);
  delay(1000);
}
