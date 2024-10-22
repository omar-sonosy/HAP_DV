const int analogPin = A0; // Broche analogique où le signal est connecté
const unsigned long samplingInterval = 1; // Intervalle d'échantillonnage en millisecondes (1ms = 1000Hz)
const unsigned long measurementInterval = 1000; // Intervalle de mesure en millisecondes (1 seconde)

unsigned long lastSampleTime = 0; // Temps du dernier échantillon
unsigned long lastMeasurementTime = 0; // Temps du dernier affichage
unsigned long pulseCount = 0; // Compteur d'impulsions détectées
float lastVoltage = 0; // Dernière valeur de tension mesurée
float maxVoltage = 0; // Valeur maximale mesurée du signal
float minVoltage = 5.0; // Valeur minimale mesurée du signal
float voltageThreshold = 0; // Seuil pour détecter les fronts
float frequency = 0; // Fréquence calculée

void setup() {
  Serial.begin(250000); // Initialisation de la communication série
}

void loop() {
  unsigned long currentTime = millis(); // Temps actuel
  
  // Prendre un échantillon si l'intervalle d'échantillonnage est écoulé
  if (currentTime - lastSampleTime >= samplingInterval) {
    float currentVoltage = analogRead(analogPin) * (5.0 / 1023.0); // Lire la valeur analogique et la convertir en tension
    
    // Mettre à jour les valeurs min et max
    if (currentVoltage > maxVoltage) {
      maxVoltage = currentVoltage;
    }
    if (currentVoltage < minVoltage) {
      minVoltage = currentVoltage;
    }
    
    // Calculer le seuil comme le milieu de l'intervalle min-max
    voltageThreshold = (maxVoltage + minVoltage) / 2.0;
    
    // Détecter un front montant
    if (lastVoltage < voltageThreshold && currentVoltage >= voltageThreshold) {
      pulseCount++; // Compter le front montant
    }
    
    // Mettre à jour la dernière tension mesurée
    lastVoltage = currentVoltage;
    lastSampleTime = currentTime;
  }
  
  // Afficher la fréquence toutes les secondes
  if (currentTime - lastMeasurementTime >= measurementInterval) {
    frequency = pulseCount; // La fréquence est le nombre de fronts détectés en 1 seconde
    Serial.print("Fréquence: ");
    Serial.print(frequency);
    Serial.print(" Hz /");
    Serial.print("Max: ");
    Serial.print(maxVoltage);
    Serial.print(" V /");
    Serial.print("Min: ");
    Serial.print(minVoltage);
    Serial.println(" V");
    
    // Réinitialiser le compteur et les valeurs min/max pour la prochaine période
    pulseCount = 0;
    maxVoltage = 0; // Réinitialiser maxVoltage pour la prochaine période
    minVoltage = 5.0; // Réinitialiser minVoltage pour la prochaine période
    lastMeasurementTime = currentTime;
  }
}
