const int analogPins[] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Broche de A0 à A7 
const int numSensorsToDisplay = 1; // nombre de capteurs qu'on veut afficher

const unsigned sampleInterval = 1; // période d'échantillonnage (en ms)
const unsigned MeasurementInterval = 500; // période de mesure (en ms)

unsigned long lastSampleTime = 0; // Temps du dernier échantillon
unsigned long lastMeasurementTime = 0; // Temps de la dernière mesure

volatile unsigned long pulseCount[numSensorsToDisplay] = {0}; // Compteur d'impulsions détectées
int lastVoltage[numSensorsToDisplay] = {0}; // Dernière valeur de tension mesurée
int maxVoltage[numSensorsToDisplay] = {0}; // Valeur maximale mesurée du signal
int minVoltage[numSensorsToDisplay] = {5000}; // Valeur minimale mesurée du signal
int voltageThreshold[numSensorsToDisplay] = {0}; // Seuil pour détecter les fronts
int frequency[numSensorsToDisplay] = {0}; // Fréquence calculée

float alpha = 0.9;
float smoothedValue = 0;

void setup() {
  Serial.begin(250000);
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastSampleTime >= sampleInterval){
    readSensors();
    lastSampleTime = currentTime;
  }
  
  if (currentTime - lastMeasurementTime >= MeasurementInterval) {
    displayResults();
    resetMeasurements();
    lastMeasurementTime = currentTime;
  }
}

void readSensors(){
  // Lecture de la valeur analogique des capteurs
  for(int i = 0; i < numSensorsToDisplay; i++){
    int readValue = analogRead(analogPins[i]);
    smoothedValue = alpha * readValue + (1 - alpha) * smoothedValue;
    int currentVoltage = map(smoothedValue, 0, 1023, 0, 5000); // Conversion de la valeur lue en tension (mV)
    updateThresholds(i, currentVoltage);
    detectPulses(i, currentVoltage);
    // Mettre à jour la dernière tension mesurée
    lastVoltage[i] = currentVoltage;
  }
}

void updateThresholds(int sensorIndex, int currentVoltage){
  
  if (currentVoltage > maxVoltage[sensorIndex]) {
    maxVoltage[sensorIndex] = currentVoltage;
  }
  if (currentVoltage < minVoltage[sensorIndex]) {
    minVoltage[sensorIndex] = currentVoltage;
  }

  // Calculer le seuil comme le milieu de l'intervalle min-max
  voltageThreshold[sensorIndex] = (maxVoltage[sensorIndex] + minVoltage[sensorIndex]) / 2;
}

void detectPulses(int sensorIndex, int currentVoltage){
  float tolerance = 0.1; // Tolérance pour éviter les erreurs de détection
  if (lastVoltage[sensorIndex] < voltageThreshold[sensorIndex] - tolerance &&
      currentVoltage >= voltageThreshold[sensorIndex] + tolerance /*&&
      maxVoltage[sensorIndex] > 10*/){
    pulseCount[sensorIndex]++; // Compter les fronts montants
  }
}

void displayResults(){
  for(int i = 0; i < numSensorsToDisplay; i++){
    frequency[i] = (pulseCount[i] * 1000) / MeasurementInterval;  // La fréquence est le nombre de fronts détectés en 1 seconde
    Serial.print("Signal ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print("Fréquence: ");
    Serial.print(frequency[i]);
    Serial.print(" Hz / ");
    Serial.print("Max: ");
    Serial.print(maxVoltage[i]);
    Serial.print(" mV / ");
    Serial.print("Min: ");
    Serial.print(minVoltage[i]);
    Serial.println(" mV");
  }
}

void resetMeasurements(){
  for(int i = 0; i < numSensorsToDisplay; i++){
    // Réinitialiser le compteur et les valeurs min/max pour la prochaine période
    pulseCount[i] = 0;
    maxVoltage[i] = 0;     // Réinitialiser maxVoltage pour la prochaine période
    minVoltage[i] = 5000;  // Réinitialiser minVoltage pour la prochaine période
  }
}