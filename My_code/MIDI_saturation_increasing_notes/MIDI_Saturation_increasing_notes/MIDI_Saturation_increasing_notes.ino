
#define NOTE_A0  21
#define NOTE_AS0 22
#define NOTE_B0  23
#define NOTE_C1  24
#define NOTE_CS1 25
#define NOTE_D1  26
#define NOTE_DS1 27
#define NOTE_E1  28
#define NOTE_F1  29
#define NOTE_FS1 30
#define NOTE_G1  31
#define NOTE_GS1 32
#define NOTE_A1  33
#define NOTE_AS1 34
#define NOTE_B1  35
#define NOTE_C2  36
#define NOTE_CS2 37
#define NOTE_D2  38
#define NOTE_DS2 39
#define NOTE_E2  40
#define NOTE_F2  41
#define NOTE_FS2 42
#define NOTE_G2  43
#define NOTE_GS2 44
#define NOTE_A2  45
#define NOTE_AS2 46
#define NOTE_B2  47
#define NOTE_C3  48
#define NOTE_CS3 49
#define NOTE_D3  50
#define NOTE_DS3 51
#define NOTE_E3  52
#define NOTE_F3  53
#define NOTE_FS3 54
#define NOTE_G3  55
#define NOTE_GS3 56
#define NOTE_A3  57
#define NOTE_AS3 58
#define NOTE_B3  59
#define NOTE_C4  60
#define NOTE_CS4 61
#define NOTE_D4  62
#define NOTE_DS4 63
#define NOTE_E4  64
#define NOTE_F4  65
#define NOTE_FS4 66
#define NOTE_G4  67
#define NOTE_GS4 68
#define NOTE_A4  69
#define NOTE_AS4 70
#define NOTE_B4  71
#define NOTE_C5  72
#define NOTE_CS5 73
#define NOTE_D5  74
#define NOTE_DS5 75
#define NOTE_E5  76
#define NOTE_F5  77
#define NOTE_FS5 78
#define NOTE_G5  79
#define NOTE_GS5 80
#define NOTE_A5  81
#define NOTE_AS5 82
#define NOTE_B5  83
#define NOTE_C6  84
#define NOTE_CS6 85
#define NOTE_D6  86
#define NOTE_DS6 87
#define NOTE_E6  88
#define NOTE_F6  89
#define NOTE_FS6 90
#define NOTE_G6  91
#define NOTE_GS6 92
#define NOTE_A6  93
#define NOTE_AS6 94
#define NOTE_B6  95
#define NOTE_C7  96
#define NOTE_CS7 97
#define NOTE_D7  98
#define NOTE_DS7 99
#define NOTE_E7  100
#define NOTE_F7  101
#define NOTE_FS7 102
#define NOTE_G7  103
#define NOTE_GS7 104
#define NOTE_A7  105
#define NOTE_AS7 106
#define NOTE_B7  107
#define NOTE_C8  108



#define SIXTEENTH QUARTER/4
#define EIGHTH QUARTER/2
#define QUARTER 526
#define DOT_QUARTER QUARTER * 1.5
#define HALF QUARTER*2
#define WHOLE QUARTER*4

//----------------------------------------------------------------------------------------------------------//
// Rentrer les variables :
const int numSensorsToDisplay=1;
const int analogPins[] = {A0, A1, A2, A3, A4, A5}; // Broche de A0 à A5
const unsigned sampleInterval = 1; // période d'échantillonnage (en ms)
const unsigned bit = 1023; // Résolution ADC (2^bit-1)
const unsigned tensionMaxADC = 5000; // Tension max de l'ADC (mv)
bool last_state=true;
bool current_state=true;
int Noise_threshold= map(5, 0, bit, 0, tensionMaxADC);
int middle_level_voltage= map(100,0,bit,0,tensionMaxADC);
int note=60;

//----------------------------------------------------------------------------------------------------------//

unsigned long lastSampleTime = 0; // Temps du dernier échantillon
unsigned long start_measurment_time=0;
unsigned long lastMeasurementTime=0;
int lastVoltage[numSensorsToDisplay] = {0}; // Dernière valeur de tension mesurée
int frequency[numSensorsToDisplay] = {0}; // Fréquence calculée
int numPeak[numSensorsToDisplay] = {0}; // Nombre de peak  // Temps du dernier peak
int active = 0; // Condition scénario
int note_increase_index=0;

int get_next_scale_note(int intial_note){
  int temp_note;
  int note_increase[7]={0,2,2,1,2,2,1};

  if (active==1){
    temp_note=intial_note+note_increase[note_increase_index];
    if (note_increase_index<6){
      note_increase_index++;
      return temp_note;
    }
    else{
      note_increase_index=0;
      return 60;
    }
  }else{
    return intial_note;
  }
}
void setup() {
  // setup serial port at 115200 to communicate via MIDI
  Serial.begin(115200);
}


void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastSampleTime >= sampleInterval){
    readSensors(); // Lecture et filtrage de la valeur analogique des capteurs
    lastSampleTime = currentTime;
  }

  if((currentTime-lastMeasurementTime)>1000){
    if(active==1){
      noteOff(note);
      active=0;
    }
    // int readValue = analogRead(analogPins[0]); // Lecture de la valeur analogique
    // int currentVoltage = map(readValue, 0, bit, 0, tensionMaxADC);
    // Serial.println(currentVoltage);
    // Serial.println(lastVoltage[0]);
     resetMeasurements(); // Réinitialisation des variables
     lastMeasurementTime = currentTime;
  }

  if (numPeak[0]>=5) {

    for(int i = 0; i < numSensorsToDisplay; i++){
      frequency[i] = numPeak[i] * (10000/(currentTime-start_measurment_time));  // La fréquence est le nombre de peak détectés en 1 seconde
    }
    // Serial.println(numPeak[0]);
    // Serial.println(frequency[0]);
    // Serial.write("\n\r");
    noteOff(note);
    // if(active==1){
    //   note++;
    // }
    note=get_next_scale_note(note);
    noteOn(note, 0x3F);
    active=1;
    // Sénario de test 1

    // if( frequency[0]< 250 ){
    //   if(active !=0){
    //     sensor_off(0);
    //   }
    //   noteOn(NOTE_A2, 0x3F);
    //   active = 1;
    // }
    // if(frequency[0] >= 250 && frequency[0]< 400){
    //   if(active != 0){
    //     sensor_off(0);
    //   }
    //   noteOn(NOTE_G3, 0x3F);
    //   active = 1; 
    // }
    // if(frequency[0] >= 400 && frequency[0]< 650){
    //   if(active != 0){
    //     sensor_off(0);
    //   }
    //   noteOn(NOTE_F4, 0x3F);
    //   active = 1;
    // }
    // if(frequency[0] >= 650 ){
    //   if(active != 0){
    //     sensor_off(0);
    //   }
    //   noteOn(NOTE_D5, 0x3F);
    //   active = 1;
    // }

    resetMeasurements(); // Réinitialisation des variables
    lastMeasurementTime = currentTime;
  }
  }


bool detect_peak(int sensorIndex, int currentVoltage){
  if((currentVoltage - lastVoltage[sensorIndex])>middle_level_voltage ){
    return true;
  }else{
    return false;
  }
}

void readSensors(){
  for(int i = 0; i < numSensorsToDisplay; i++){
    int readValue = analogRead(analogPins[i]); // Lecture de la valeur analogique
    int currentVoltage = map(readValue, 0, bit, 0, tensionMaxADC); // Conversion de la valeur lue en tension (mV)

    if(detect_peak(i,currentVoltage)){
      //Serial.write("peak detected\n\r");
      if(numPeak[i]==0){
        start_measurment_time=millis();
      }
      numPeak[i]++;
    }
    lastVoltage[i] = currentVoltage; // Mettre à jour la dernière tension mesurée
  }
}



//----------------------------------------------------------------------------------------------------------//

void resetMeasurements(){
  for(int i = 0; i < numSensorsToDisplay; i++){
    numPeak[i] = 0; // Réinitialise le nombre de peak pour la prochaine période
  }
}
/*
MIDI Data message
Command: 0x90 (Note On command code)
Note: note number between 0 and 127 (piano notes are between 21-108)
Velocity: how forcefully not is played between 0 and 127

MIDI messages are oftentimes provided in hexadecimal format.
Notes and velocities range from 0x00 to 0x7F.
The Note On command code is 0x90.
*/

// turns note on at a specific velocity for a certain duration
void noteOn(int note, int velocity, int duration) {
  Serial.write(144);
  Serial.write(note);
  Serial.write(velocity);
  delay(duration);
  Serial.write(144);
  Serial.write(note);
  Serial.write(0);
}

// turns note on at a specific velocity indefinitely
void noteOn(int note, int velocity) {
  Serial.write(144);
  Serial.write(note);
  Serial.write(velocity);
}

// turns off a previously playing note
void noteOff(int note) {
  Serial.write(144);
  Serial.write(note);
  Serial.write(0);
}


void sensor_off(int sensor_index){
  if(sensor_index==0){
    noteOff(NOTE_A2);
    noteOff(NOTE_G3);
    noteOff(NOTE_F4);
    noteOff(NOTE_D5);
  }
  
}
// plays no note for specified duration
void noNote(int duration) {
  Serial.write(144);
  Serial.write(0);
  Serial.write(0);
  delay(duration);
  Serial.write(144);
  Serial.write(0);
  Serial.write(0);
}



