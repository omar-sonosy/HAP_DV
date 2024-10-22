//----------------------------------------------------------------------------------------------------------//
// Nom du fichier     : Version_finale.ino
// Auteur             : ELOY Ulrich
// Date de création   : 26 août 2024
// Description        : Ce script est programme Arduino permettant d'échantilloner 
//                      un ou plusieurs signaux analogiques, de le(s) traiter avec 
//                      un maping, un filtre passe-bas et un cutoff puis une transmission MIDI
//
//
// Historique des modifications :
//  - 26/08/2024 : Création du script (Auteur)
// - 30/09/2024 : Ajout de nouvelles fonctionnalités (Auteur)
//
//----------------------------------------------------------------------------------------------------------//


#include <MIDI.h>

//----------------------------------------------------------------------------------------------------------//
// Rentrer les variables :

const unsigned numSensorsToDisplay = 1; // nombre de capteurs qu'on veut afficher
const int analogPins[] = {A0, A1, A2, A3, A4, A5}; // Broche de A0 à A5
const unsigned sampleInterval = 1; // période d'échantillonnage (en ms)
const unsigned measurementInterval = 1000; // période de mesure (en ms)
const unsigned peakTimeout = 12; // temps entre 2 peak (en ms)
const unsigned lowPass = 72; // fréquence de coupure filtre passe-bas (en Hz)
const float alpha = 0.311;
const unsigned Cutoff[] = {10, 20, 0, 0, 0, 0}; // Valeur seuil pour ne pas détecter le bruit (en mV)
const unsigned bit = 1023; // Résolution ADC (2^bit-1)
const unsigned tensionMaxADC = 5000; // Tension max de l'ADC (mv)

//----------------------------------------------------------------------------------------------------------//

unsigned long lastSampleTime = 0; // Temps du dernier échantillon
unsigned long lastMeasurementTime = 0; // Temps de la dernière mesure
int lastVoltage[numSensorsToDisplay] = {0}; // Dernière valeur de tension mesurée

int maxVoltage[numSensorsToDisplay] = {0}; // Valeur maximale mesurée du signal
int frequency[numSensorsToDisplay] = {0}; // Fréquence calculée

int peakValue[numSensorsToDisplay] = {0}; // Variable pour stocker la valeur de crête
int numPeak[numSensorsToDisplay] = {0}; // Nombre de peak 
unsigned long lastPeakTime = 0; // Temps du dernier peak
float filteredValue = 0; // Valeur filtrer

int val0; // valeur transmis en MIDI
int active = 0; // Condition scénario 1

//----------------------------------------------------------------------------------------------------------//

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  MIDI.begin(MIDI_CHANNEL_OFF);
}

//----------------------------------------------------------------------------------------------------------//

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastSampleTime >= sampleInterval){
    readSensors(); // Lecture et filtrage de la valeur analogique des capteurs
    lastSampleTime = currentTime;
  }
  
  if (currentTime - lastMeasurementTime >= measurementInterval) {

    for(int i = 0; i < numSensorsToDisplay; i++){
      frequency[i] = numPeak[i] * (1000/measurementInterval);  // La fréquence est le nombre de peak détectés en 1 seconde
    }


    // Sénario de test 1
    val0 = map(maxVoltage[0], 0, 5000, 100, 127);

    if(frequency[0] < 25 && frequency[0] >2){
      if(active == 2){
        MIDI.sendNoteOff(90,0,1);
      }
      MIDI.sendNoteOn(80,val0,2);
      active = 1;
    }
    if(frequency[0] > 25){
      if(active == 1){
        MIDI.sendNoteOff(80,0,2);
      }
      MIDI.sendNoteOn(90,val0,1);
      active = 2; 
    }
    if(frequency[0] < 2){
      MIDI.sendNoteOff(80,0,2);
      MIDI.sendNoteOff(90,0,1);
    }

    resetMeasurements(); // Réinitialisation des variables
    lastMeasurementTime = currentTime;
  }
}

//----------------------------------------------------------------------------------------------------------//

void readSensors(){
  for(int i = 0; i < numSensorsToDisplay; i++){
    int readValue = analogRead(analogPins[i]); // Lecture de la valeur analogique
    filteredValue = alpha * readValue + (1 - alpha) * filteredValue; // Filtrage de la valeur analogique
    int currentVoltage = map(filteredValue, 0, bit, 0, tensionMaxADC); // Conversion de la valeur lue en tension (mV)

    if (currentVoltage <= Cutoff[i]) currentVoltage = 0; // Cutoff pour les faibles signaux

    detectPeak(i, currentVoltage); // Détecteur de peak avec incrémentation du nombre de peak
    
    lastVoltage[i] = currentVoltage; // Mettre à jour la dernière tension mesurée
  }
}

//----------------------------------------------------------------------------------------------------------//

void detectPeak(int sensorIndex, int currentVoltage){
  if(currentVoltage < lastVoltage[sensorIndex] && millis() - lastPeakTime > peakTimeout){
    peakValue[sensorIndex] = lastVoltage[sensorIndex];
    numPeak[sensorIndex]++;
    lastPeakTime = millis();
  }

  if (peakValue[sensorIndex] > maxVoltage[sensorIndex]){
    maxVoltage[sensorIndex] = peakValue[sensorIndex];
  }
}

//----------------------------------------------------------------------------------------------------------//

void resetMeasurements(){
  for(int i = 0; i < numSensorsToDisplay; i++){
    numPeak[i] = 0; // Réinitialise le nombre de peak pour la prochaine période
    peakValue[i] = 0; // Réinitialise la valeur du dernier peak pour la prochaine période
    maxVoltage[i] = 0; // Réinitialise maxVoltage pour la prochaine période
  }
}