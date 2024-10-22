const int analogPins[] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Broche de A0 à A7 
const int numSensorsToDisplay = 1; // nombre de capteurs qu'on veut afficher

const unsigned sampleInterval = 1; // période d'échantillonnage (en ms)
const unsigned MeasurementInterval = 1000; // période de mesure (en ms)

unsigned long lastSampleTime = 0; // Temps du dernier échantillon
unsigned long lastMeasurementTime = 0; // Temps de la dernière mesure
int lastVoltage[numSensorsToDisplay] = {0}; // Dernière valeur de tension mesurée

int maxVoltage[numSensorsToDisplay] = {0}; // Valeur maximale mesurée du signal
int frequency[numSensorsToDisplay] = {0}; // Fréquence calculée

int peakValue[numSensorsToDisplay] = {0}; // Variable pour stocker la valeur de crête
int numPeak[numSensorsToDisplay] = {0}; // Nombre de peak 
unsigned long lastPeakTime = 0;
const unsigned long peakTimeout = 12;

const float alpha = 0.311;
float filteredValue = 0;

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
    filteredValue = alpha * readValue + (1 - alpha) * filteredValue;
    int currentVoltage = map(filteredValue, 0, 1023, 0, 5000); // Conversion de la valeur lue en tension (mV)

    if (currentVoltage <= 10) currentVoltage = 0;

    detectPeak(i, currentVoltage);
    // Mettre à jour la dernière tension mesurée
    lastVoltage[i] = currentVoltage;
  }
}

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

void displayResults(){
  for(int i = 0; i < numSensorsToDisplay; i++){
    frequency[i] = numPeak[i] * (1000/MeasurementInterval);  // La fréquence est le nombre de fronts détectés en 1 seconde
    Serial.print("Signal ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print("Fréquence: ");
    Serial.print(frequency[i]);
    Serial.print(" Hz / ");
    Serial.print("Max: ");
    Serial.print(maxVoltage[i]);
    Serial.println(" mV");
  }
  Serial.println();
  Serial.println();
}

void resetMeasurements(){
  for(int i = 0; i < numSensorsToDisplay; i++){
    // Réinitialiser le compteur et les valeurs min/max pour la prochaine période
    numPeak[i] = 0;
    peakValue[i] = 0;
    maxVoltage[i] = 0;     // Réinitialiser maxVoltage pour la prochaine période
  }
}