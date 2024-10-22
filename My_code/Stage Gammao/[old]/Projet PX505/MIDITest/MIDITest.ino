/*
  MIDI note player

  This sketch shows how to use the serial transmit pin (pin 1) to send MIDI note data.
  If this circuit is connected to a MIDI synth, it will play the notes
  F#-0 (0x1E) to F#-5 (0x5A) in sequence.

  The circuit:
  - digital in 1 connected to MIDI jack pin 5
  - MIDI jack pin 2 connected to ground
  - MIDI jack pin 4 connected to +5V through 220 ohm resistor
  - Attach a MIDI cable to the jack, then to a MIDI synth, and play music.

  created 13 Jun 2006
  modified 13 Aug 2012
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Midi
*/

void setup() {
  // Set MIDI baud rate:
  Serial.begin(31250);

  // Set MIDI baud rate:
  //Serial.begin(115200);
  
  delay(2000);
}

void loop() {
  // Play notes from F#-0 (0x1E) to F#-5 (0x5A).
  // Each note will start at a velocity of 1, and then will slowly build up
  // using the aftertouch message. Then, the note is stopped, and the sequence
  // starts again.
  for (int note = 0x50; note < 0x5A; note++) {
    sendNote(0x90, note, 40);
    delay(300);
    sendAftertouch(0xD0, 60);
    delay(300);
    sendAftertouch(0xD0, 100);
    delay(300);
    sendAftertouch(0xD0, 127);
    delay(300);
    sendAftertouch(0xD0, 80);
    delay(300);
    sendAftertouch(0xD0, 30);
    delay(300);
    sendNote(0x90, note, 0);
  }
}

// plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that
// data values are less than 127:
void sendNote(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

// alters a MIDI note. Doesn't check to see that cmd is greater than 127, or that
// data values are less than 127:
void sendAftertouch(int cmd, int velocity) {
  Serial.write(cmd);
  Serial.write(velocity);
}

// alters a MIDI note. Doesn't check to see that cmd is greater than 127, or that
// data values are less than 127:
void sendBend(int cmd, int bend1, int bend2) {
  Serial.write(cmd);
  Serial.write(bend1);
  Serial.write(bend2);
}
