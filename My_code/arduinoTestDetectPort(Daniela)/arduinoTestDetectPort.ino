#include "notes.h"

// Timing Definitions
#define SIXTEENTH QUARTER / 4
#define EIGHTH QUARTER / 2
#define QUARTER 526
#define DOT_QUARTER QUARTER * 1.5
#define HALF QUARTER * 2
#define WHOLE QUARTER * 4

//----------------------------------------------------------------------------------------------------------//
// Variable Definitions
const int analogPins[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9}; // Numerical pin numbers
const int numSensorsToDisplay = sizeof(analogPins) / sizeof(analogPins[0]); // Number of pins
const float tensionMaxADC = 5.0; // Reference voltage (5V for most Arduino models)
const int bit = 1023; // ADC resolution (10-bit ADC)
const float targetVoltage = 3.5; // Target voltage to detect
const unsigned long peakTimeout = 500; // Timeout to detect peaks (in ms)
const unsigned long sampleInterval = 1; // Sampling time to read the sensors (in ms)

// Full classical piano range (white keys only)
const int pianoScale[] = {
    NOTE_C1, NOTE_D1, NOTE_E1, NOTE_F1, NOTE_G1, NOTE_A1, NOTE_B1,
    NOTE_C2, NOTE_D2, NOTE_E2, NOTE_F2, NOTE_G2, NOTE_A2, NOTE_B2,
    NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_A3, NOTE_B3,
    NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4,
    NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5,
    NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_A6, NOTE_B6,
    NOTE_C7, NOTE_D7, NOTE_E7, NOTE_F7, NOTE_G7, NOTE_A7, NOTE_B7,
    NOTE_C8
};
bool pianoScaleState[sizeof(pianoScale) / sizeof(pianoScale[0])] = {false};

const int totalNotes = sizeof(pianoScale) / sizeof(pianoScale[0]);

// Sensor Variables
int currentScaleStart = 0; // Index of the first note of the current scale (C major or equivalent)
const int notesPerScale = 7; // Number of notes in a scale
int numPeaks[numSensorsToDisplay] = {0}; // Array to count peaks for each analog input
int lastVoltage[numSensorsToDisplay] = {0};
unsigned long lastSampleIntervalTime[numSensorsToDisplay] = {0}; // Stores the last detected peak time for each analog input
unsigned long lastPeakTimeoutTime[numSensorsToDisplay] = {0};    // Stores the last timeout time for each analog input

// Square Wave Variables (Digital 9 pin to test the analog inputs)
const int squareWavePin = 9; // Pin to output the square wave
const int testFrequency = 10; // Frequency in Hz
const int halfPeriod = 1000 / (2 * testFrequency); // Half-period in milliseconds
unsigned long lastSquareWaveTime = 0; // Timing for square wave
bool squareWaveState = LOW;

//----------------------------------------------------------------------------------------------------------//
// Function Prototypes
void detectPeaks(unsigned long currentTime);
float adcToVoltage(int adcValue);
void changeScale(int direction);
void noteOn(int note, int velocity);
void noteOff(int note);
void generateSquareWave(unsigned long currentTime);

//----------------------------------------------------------------------------------------------------------//
// Main Functions

void setup() {
  pinMode(squareWavePin, OUTPUT); // Set square wave pin as output
  Serial.begin(115200);
}

void loop() {
  unsigned long currentTime = millis();
  generateSquareWave(currentTime); // Generate a square wave signal on pin 9
  detectPeaks(currentTime);        // Detect peaks on the analog inputs
}

//----------------------------------------------------------------------------------------------------------//
// Helper Functions

void generateSquareWave(unsigned long currentTime) {
  if (currentTime - lastSquareWaveTime >= halfPeriod) { // Check if it's time to toggle the signal
    squareWaveState = !squareWaveState; // Toggle the square wave state
    digitalWrite(squareWavePin, squareWaveState); // Write the square wave signal to the pin
    lastSquareWaveTime = currentTime; // Update the last toggle time
  }
}

void detectPeaks(unsigned long currentTime) {
  for (int i = 0; i < numSensorsToDisplay; i++) {
    if (currentTime - lastSampleIntervalTime[i] >= sampleInterval) {
      int readValue = analogRead(analogPins[i]);
      float currentVoltage = adcToVoltage(readValue);

      // Check if a new peak is detected
      if ((currentVoltage - lastVoltage[i]) > targetVoltage) {
        numPeaks[i]++;
        lastPeakTimeoutTime[i] = currentTime; // Reset the timeout timer
      }

      lastVoltage[i] = currentVoltage;
      lastSampleIntervalTime[i] = currentTime;
    }

    //Increase or decrease scale
    if (analogPins[i] == A8 && numPeaks[i] >= 5) {
      changeScale(-1); // Move down a scale
      numPeaks[i] = 0; // Reset peak count for A8
      continue; // Skip note-playing logic for this pin
    } else if (analogPins[i] == A9 && numPeaks[i] >= 5) {
      changeScale(1); // Move up a scale
      numPeaks[i] = 0; // Reset peak count for A9
      continue; // Skip note-playing logic for this pin
    }

    // If the timeout is reached and no peaks have occurred, turn off the note
    if ((currentTime - lastPeakTimeoutTime[i]) > peakTimeout) {
      if (pianoScaleState[currentScaleStart + i]) {
        noteOff(pianoScale[currentScaleStart + i]); // Turn off the note
        pianoScaleState[currentScaleStart + i] = false; // Mark the note as inactive
      }
    } else {
      
      // If the note is not already playing and peaks were detected, turn it on
      if (i<8 && !pianoScaleState[currentScaleStart + i] && numPeaks[i] > 3) {
        noteOn(pianoScale[currentScaleStart + i], 0x3F); // Play the note
        pianoScaleState[currentScaleStart + i] = true; // Mark the note as active
      }
      
    }
  }
}

float adcToVoltage(int adcValue) {
  return (adcValue / (float)bit) * tensionMaxADC;
}

void changeScale(int direction) {
  currentScaleStart += direction * notesPerScale;
  if (currentScaleStart < NOTE_C1) {
    currentScaleStart = NOTE_C1;
  } else if (currentScaleStart >= NOTE_C8) {
    currentScaleStart= NOTE_C8;
  }
}

void noteOn(int note, int velocity) {
  Serial.write(144);
  Serial.write(note);
  Serial.write(velocity);
}

void noteOff(int note) {
  Serial.write(144);
  Serial.write(note);
  Serial.write(0);
}
