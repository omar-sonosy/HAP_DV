#include <MIDI.h>

// set up the MIDI connection
MIDI_CREATE_DEFAULT_INSTANCE();

// define the analog input pins for the sensors
const int sensorPin1 = A0;
const int sensorPin2 = A1;
const int sensorPin3 = A2;
const int sensorPin4 = A3;

// define the MIDI note and channel for each sensor
const int sensorNote1 = 60;
const int sensorNote2 = 62;
const int sensorNote3 = 64;
const int sensorNote4 = 65;
const int midiChannel = 1;

// define the aftertouch threshold value
const int noteOnThreshold = 100;

// define the time threshold for sending a note off message (in milliseconds)
const int noteOffThreshold = 1000;

void setup() {
  // set up the MIDI connection
  MIDI.begin(MIDI_CHANNEL_OMNI);

  // set up the analog input pins for the sensors
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin3, INPUT);
  pinMode(sensorPin4, INPUT);
}

void loop() {
  // read the sensor data
  int sensorValue1 = analogRead(sensorPin1);
  int sensorValue2 = analogRead(sensorPin2);
  int sensorValue3 = analogRead(sensorPin3);
  int sensorValue4 = analogRead(sensorPin4);

  // apply the low-pass and notch filters to the sensor data
  sensorValue1 = filterSensorData(sensorValue1);
  sensorValue2 = filterSensorData(sensorValue2);
  sensorValue3 = filterSensorData(sensorValue3);
  sensorValue4 = filterSensorData(sensorValue4);

  // send the filtered sensor data over MIDI as note on or aftertouch messages
  static unsigned long lastSensorActivation1 = 0;
  static unsigned long lastSensorActivation2 = 0;
  static unsigned long lastSensorActivation3 = 0;
  static unsigned long lastSensorActivation4 = 0;
  static bool sensorActive1 = false;
  static bool sensorActive2 = false;
  static bool sensorActive3 = false;
  static bool sensorActive4 = false;

  if (!sensorActive1) {
    if (sensorValue1 > noteOnThreshold) {
      MIDI.sendNoteOn(sensorNote1, 100, midiChannel);
      sensorActive1 = true;
    }
  } 
  else {
    if (sensorActive1 && millis() - lastSensorActivation1 > noteOffThreshold) {
        MIDI.sendNoteOff(sensorNote1, 0, midiChannel);
        sensorActive1 = false;
      }
    else {
      MIDI.sendAfterTouch(sensorNote1, sensorValue1, midiChannel);
     }
  }
  if (!sensorActive2) {
    if (sensorValue2 > noteOnThreshold) {
      MIDI.sendNoteOn(sensorNote2, 100, midiChannel);
      sensorActive2 = true;
    }
  } 
  else {
    if (sensorActive2 && millis() - lastSensorActivation2 > noteOffThreshold) {
        MIDI.sendNoteOff(sensorNote2, 0, midiChannel);
        sensorActive2 = false;
      }
    else {
      MIDI.sendAfterTouch(sensorNote2, sensorValue2, midiChannel);
     }
  }
  if (!sensorActive3) {
    if (sensorValue3 > noteOnThreshold) {
      MIDI.sendNoteOn(sensorNote3, 100, midiChannel);
      sensorActive3 = true;
    }
  } 
  else {
    if (sensorActive3 && millis() - lastSensorActivation3 > noteOffThreshold) {
        MIDI.sendNoteOff(sensorNote3, 0, midiChannel);
        sensorActive3 = false;
      }
    else {
      MIDI.sendAfterTouch(sensorNote3, sensorValue3, midiChannel);
     }  
  }
  if (!sensorActive4) {
    if (sensorValue4 > noteOnThreshold) {
      MIDI.sendNoteOn(sensorNote4, 100, midiChannel);
      sensorActive4 = true;
    }
  } 
  else {
    if (sensorActive4 && millis() - lastSensorActivation4 > noteOffThreshold) {
        MIDI.sendNoteOff(sensorNote4, 0, midiChannel);
        sensorActive4 = false;
      }
    else {
      MIDI.sendAfterTouch(sensorNote4, sensorValue4, midiChannel);
     }
  }
}


int filterSensorData(int sensorValue) {

  if (sensorValue < 0){
    sensorValue = -sensorValue;
  }

  // define the filter parameters
  const int lowPassCutoff = 100;
  const int notchFrequency = 50;
  const float notchQ = 0.707;

  // apply the low-pass filter using a simple moving average
  static int lowPassValues[10] = {0};
  static int lowPassIndex = 0;
  lowPassValues[lowPassIndex] = sensorValue;
  lowPassIndex = (lowPassIndex + 1) % 10;
  int lowPassSum = 0;
  for (int i = 0; i < 10; i++) {
    lowPassSum += lowPassValues[i];
  }
  int lowPassOutput = lowPassSum / 10;

  // apply the notch filter using a biquad filter
  static float notchX[3] = {0};
  static float notchY[3] = {0};
  float notchOutput = lowPassOutput;
  float w0 = 2 * M_PI * notchFrequency / 44100; // Audio sample rate
  float alpha = sin(w0) / (2 * notchQ);
  float a0 = 1 + alpha;
  float a1 = -2 * cos(w0);
  float a2 = 1 - alpha;
  float b0 = 1;
  float b1 = -2 * cos(w0);
  float b2 = 1;
  notchOutput = (b0/a0) * lowPassOutput + (b1/a0) * notchX[1] + (b2/a0) * notchX[2] - (a1/a0) * notchY[1] - (a2/a0) * notchY[2];
  notchX[2] = notchX[1];
  notchX[1] = lowPassOutput;
  notchY[2] = notchY[1];
  notchY[1] = notchOutput;

  return notchOutput;
}