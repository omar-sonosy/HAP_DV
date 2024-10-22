#include <MIDI.h> // Include MIDI Library
#include <KickFilters.h>

const int sampleSize = 256; // Number of samples per block
const float samplingTime = 1; // s (time between two acquisitions)          A ACTUALISER
const float samplingFrequency = 1/samplingTime; // Hz (self-explanatory)

// A REGLER
const int frequencyCutoffNotch = 50; // Hz
const int frequencyCutoffLowPass = 200; // Hz
const int frequencyCutoffHighPass = 0.1; // Hz
const int maxIntensity = 5; // V (the maximum possible velocity)
const int outputPin = 1;

int * samples; // Stocke le bloc de samples en mémoire
int * filteredSamples; // Stocke le bloc de sample traité en mémoire



int * initializeSample(int sampleSize)// Initializes sample
{

  int * p = (int*) malloc(sampleSize * sizeof(int));

  for (int i = 0; i< sampleSize; i++) *p = 0;

  return p;
}

void destroySample(int * p){ // Used to free space in memory if needed
  free(p);
}

int * samplesAcquisition(int * p, int sampleSize, char pin) { //Allows sampling sampleSize values
  
  for (int i = 0; i < sampleSize; i++) p[i] = analogRead(pin);
  
  return p;
}

int * absoluteValues(int * p, int sampleSize) { // Make the whole block go through an absolute value operation
  
  for (int i = 0; i < sampleSize; i++){ 
    if (p[i] < 0) {
      p[i] = -p[i];
    } 
  }
  return p;
}

void noteOn(int cmd, int pitch, int velocity) { // Write a Note On message on the MIDI canal
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

void playEnvelope(int * envelope, int sampleSize, int outputPin) { // Plays the envelope on a selected channel
  for (int i = 0; i < sampleSize; i++){
    noteOn(outputPin, 0x1E, envelope[i] / maxIntensity);
  }
}

MIDI_CREATE_DEFAULT_INSTANCE(); // Create an instance of the midi library

// --------------------------------------------------------------------------//

void setup() {
  
  Serial.begin(31250); // Baud rate of the MIDI

  MIDI.begin(MIDI_CHANNEL_OMNI); // Begin MIDI and listen to all channels

  * samples = initializeSample(sampleSize);
  * filteredSamples = initializeSample(sampleSize);

}


void loop() {

  * samples = samplesAcquisition(* samples, sampleSize, "A0"); // Acquiert un bloc de samples

  KickFilters<int8_t>::notch(* samples, * filteredSamples, sampleSize, frequencyCutoffNotch, samplingFrequency); // Filtre les 50 Hz (qui selon nos premières mesures pourrit le signal)

  * filteredSamples = absoluteValues(* filteredSamples, sampleSize); // Met tous les samples à leur valeur absolue en vue

  KickFilters<int16_t>::lowpass(* filteredSamples, * filteredSamples, sampleSize, frequencyCutoffLowPass, samplingFrequency); // Gros filtre passe-bas pour le détecteur d'enveloppe
  // KickFilters<int16_t>::highpass(* filteredSamples, * filteredSamples, sampleSize, frequencyCutoffHighPass, samplingFrequency); // Petit filtre passe-haut pour éviter les phénomènes trop statiques (optionnel ?)

  // Conversion en MIDI ?
  playEnvelope(* filteredSamples, sampleSize, outputPin);

}







