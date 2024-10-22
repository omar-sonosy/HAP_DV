#include <MIDI.h>

// set up the MIDI connection
MIDI_CREATE_DEFAULT_INSTANCE();

// define the analog input pins for the sensors
const int sensorPin1 = A0;
const int sensorPin2 = A1;
const int sensorPin3 = A2;
const int sensorPin4 = A3;

// define the MIDI note and channel for each sensor
const int sensorNote1 = 70;
const int midiChannel = 1;

// define the note on threshold value
float noteOnThreshold = 0.3; // threshold of 300 mV

// define the time threshold for sending a note off message (in milliseconds)
const int noteOffThreshold = 1000;

void setup() {
  // set up the MIDI connection
  MIDI.begin(MIDI_CHANNEL_OMNI);

  // set up the analog input pins for the sensors
  pinMode(sensorPin1, INPUT);
}

void loop() {
  // read the sensor data
  float sensorValue1 = analogRead(sensorPin1);

  // apply the low-pass and notch filters to the sensor data
  sensorValue1 = filterSensorData(sensorValue1);

  // send the filtered sensor data over MIDI as note on or aftertouch messages
  static unsigned long lastSensorActivation1 = 0;
  static bool sensorActive1 = false;

  if (!sensorActive1) {
    if (sensorValue1 > noteOnThreshold) {
      MIDI.sendNoteOn(sensorNote1, sensorValueInMIDIValue(sensorValue1), midiChannel);
      sensorActive1 = true;
    }
  } 
  else {
    if (sensorActive1 && millis() - lastSensorActivation1 > noteOffThreshold) {
        MIDI.sendNoteOff(sensorNote1, 0, midiChannel);
        sensorActive1 = false;
      }
    else {
      MIDI.sendPolyPressure(midiChannel, sensorNote1, sensorValueInMIDIValue(sensorValue1));
    }
  }

  MIDI.read(); // this reads the input (although we won't need it) just to empty the buffers.

}

int sensorValueInMIDIValue(float sensorValue) {
  // define a maximum value for the sensor (in volts)
  const float maxSensor = 5.0; // 2000 mV
  int MIDIValue = 0;

  if (sensorValue >= maxSensor) {
    MIDIValue = 127;
  }
  else {
    MIDIValue = 127 * sensorValue / maxSensor;
  }

  return MIDIValue; 
}

float filterSensorData(float sensorValue) {

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
