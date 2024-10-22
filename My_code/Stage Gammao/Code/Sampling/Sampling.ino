const int analogPins[] = {A0, A1, A2, A3, A4, A5}; // Broche de A0 à A5 
const int totalSensors = sizeof(analogPins) / sizeof(analogPins[0]);
int numSensorsToDisplay = 2;

const int sampleRate = 1000;
const unsigned long sampleInterval = 1000000 / sampleRate;

void setup() {
  // Initialisation de la communication série pour l'affichage des données
  Serial.begin(9600);
  if(numSensorsToDisplay > totalSensors) numSensorsToDisplay = totalSensors;
}

void loop() {

  unsigned long startTime = micros();
  // Lecture de la valeur analogique des capteurs
  for(int i = 0; i < numSensorsToDisplay; i++){
    int sensorValue = analogRead(analogPins[i]);
    int tension = map(sensorValue, 0, 1023, 0, 5000); // Conversion de la valeur lue en tension (mV)
    Serial.print(tension);
    if (i < numSensorsToDisplay -1) Serial.print(",");
  }
  Serial.println();
  
  while(micros() - startTime < sampleInterval){

  }
}