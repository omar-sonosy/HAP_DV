#include <MIDI.h> // Include MIDI Library
#include <KickFilters.h>

const int sampleSize = 256; // Number of samples per block
const float samplingTime = 1; // s (time between two acquisitions)(how to determine it by taking into account the serialisation ?)          A ACTUALISER
const float samplingFrequency = 1/samplingTime; // Hz (self-explanatory)

// A REGLER
const int frequencyCutoffNotch = 50; // Hz
const int frequencyCutoffLowPass = 100; // Hz
const int frequencyCutoffHighPass = 0.1; // Hz
const int maxIntensity = 5; // V (the maximum possible velocity)
const int outputPin = 1;
const int inPin = 6;
const int note = 0x9E;

int * samples; // Stocke le bloc de samples en mémoire
int * filteredSamples; // Stocke le bloc de sample traité en mémoire



int * initializeSample(int sampleSize)// Initializes sample
{

  int * p = (int*) malloc(sampleSize * sizeof(int));

  for (int i = 0; i< sampleSize; i++) *p = 0;

  return p;
}

void destroySample(int * p){ // Used to free space in memory if needed
  free(* p);
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

// Non utilisé pour la version sérialisée
void playEnvelope(int * envelope, int sampleSize, int outputPin, int note) { // Plays the envelope on a selected channel
  for (int i = 0; i < sampleSize; i++){
    noteOn(outputPin, note, envelope[i] / maxIntensity);
  }
}

MIDI_CREATE_DEFAULT_INSTANCE(); // Create an instance of the midi library

// --------------------------------------------------------------------------//
//                  Tentative de sérialisation "à la main"
// --------------------------------------------------------------------------//

void setup() {
  
  Serial.begin(31250); // Baud rate of the MIDI

  MIDI.begin(MIDI_CHANNEL_OMNI); // Begin MIDI and listen to all channels

  * samples = initializeSample(sampleSize);
  * filteredSamples = initializeSample(sampleSize);

  * samples = samplesAcquisition(* samples, sampleSize, "A0"); // Fait une première acquisition d'un bloc de données

  // Crée un premier bloc de samples filtrés pour commencer à envoyer le message MIDI
  KickFilters<int8_t>::notch(* samples, * filteredSamples, sampleSize, frequencyCutoffNotch, samplingFrequency); // Filtre les 50 Hz (qui selon nos premières mesures pourrit le signal)
  * filteredSamples = absoluteValues(* filteredSamples, sampleSize); // Met tous les samples à leur valeur absolue en vue
  KickFilters<int16_t>::lowpass(* filteredSamples, * filteredSamples, sampleSize, frequencyCutoffLowPass, samplingFrequency); // Gros filtre passe-bas pour le détecteur d'enveloppe
  // KickFilters<int16_t>::highpass(* filteredSamples, * filteredSamples, sampleSize, frequencyCutoffHighPass, samplingFrequency); // Petit filtre passe-haut pour éviter les phénomènes trop statiques (optionnel ?)

}

void loop() {

  for (int i = 0; i < sampleSize; i++){ // Réalise l'acquisition tout en "parallélisant" la sortie du signal

    samples[i] = analogRead(inPin); // Acquiert un sample

    noteOn(outputPin, note, filteredSamples[i] / maxIntensity); // Joue un sample du bloc filtré précédent

  }

  // Réalise le filtrage sur le bloc acquit

  KickFilters<int8_t>::notch(* samples, * filteredSamples, sampleSize, frequencyCutoffNotch, samplingFrequency); // Filtre les 50 Hz (qui selon nos premières mesures pourrit le signal)

  // * filteredSamples = absoluteValues(* filteredSamples, sampleSize); // Met tous les samples à leur valeur absolue en vue (optionnel car les ADCs font que du 0 à 5 V ?)

  KickFilters<int16_t>::lowpass(* filteredSamples, * filteredSamples, sampleSize, frequencyCutoffLowPass, samplingFrequency); // Gros filtre passe-bas pour le détecteur d'enveloppe

  // KickFilters<int16_t>::highpass(* filteredSamples, * filteredSamples, sampleSize, frequencyCutoffHighPass, samplingFrequency); // Petit filtre passe-haut pour éviter les phénomènes trop statiques (optionnel ?)
}







