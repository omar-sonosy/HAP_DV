
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



#define CYCLE_NUM_PEAK 5

//----------------------------------------------------------------------------------------------------------//
// Used Global Variables:
const int numSensorsToDisplay=3;
const int analogPins[] = {A0, A1, A2, A3, A4, A5}; // Analog pins
const unsigned sampleInterval = 1; // sampling interval (in ms)
const unsigned bit = 1023; // ADC Resolution
const unsigned tensionMaxADC = 5000; // Maximum value of ADC (mv)
int middle_level_voltage= map(100,0,bit,0,tensionMaxADC); //Voltage value to detect a peak
unsigned long lastSampleTime = 0; // Time of last sample
unsigned long start_measurment_time[numSensorsToDisplay]={0};//Time of first peak detected for each sensor
unsigned long lastMeasurementTime[numSensorsToDisplay]={0};//Time of last measurment taken for each sensor
int lastVoltage[numSensorsToDisplay] = {0}; // Last voltage measured for each sensor
int numPeak[numSensorsToDisplay] = {0}; // Number of peaks detected for each sensor
bool sensor_active[numSensorsToDisplay]={false};// Array to detect if the sensor is active or not
int sensor_notes[numSensorsToDisplay]={0};//Array to save the playing note for each sesnor
int note_increase_index=0;// index to find the next note in the increasing array




void setup() {
  // setup serial port at 115200 to communicate via MIDI
  Serial.begin(115200);
}


void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastSampleTime >= sampleInterval){
    readSensors(); // Reading the sensor value and detecting if there's a peak or not
    lastSampleTime = currentTime;
  }

  for(int i=0; i<numSensorsToDisplay;i++){
// if a sensor stays idle for 1 seconds it cuts off the playing note
    if(currentTime-lastMeasurementTime[i]>1000){
      if(true==sensor_active[i]){
        sensor_off(i);
        sensor_active[i]=false;
      }
      lastMeasurementTime[i]=currentTime;
      resetMeasurements(i);
    }
    //if a sensor reading is more than specific value or more it activates the sensor
    if(numPeak[i]>=CYCLE_NUM_PEAK){
      sensor_on(i);
      sensor_active[i]=true;
      lastMeasurementTime[i]=currentTime;
      resetMeasurements(i);
    }

  }
}

//helping function to detect if there's a peak
bool detect_peak(int sensorIndex, int currentVoltage){
  if((currentVoltage - lastVoltage[sensorIndex])>middle_level_voltage ){
    return true;
  }else{
    return false;
  }
}
//function to read all sensors and detect if there's a peak
void readSensors(){
  for(int i = 0; i < numSensorsToDisplay; i++){
    int readValue = analogRead(analogPins[i]); // Lecture de la valeur analogique
    int currentVoltage = map(readValue, 0, bit, 0, tensionMaxADC); // Conversion de la valeur lue en tension (mV)

    if(detect_peak(i,currentVoltage)){
      if(numPeak[i]==0){
        start_measurment_time[i]=millis();
      }
      numPeak[i]++;
    }
    lastVoltage[i] = currentVoltage; // Mettre à jour la dernière tension mesurée
  }
}



//----------------------------------------------------------------------------------------------------------//
//Algorithms functions



//function to scale up the note in a musical way
int get_next_scale_note_musical(int intial_note, bool status){
  int temp_note;
  int note_increase[7]={0,2,2,1,2,2,1};

  if (status==true){
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
//function to scale up the note to control the volume of a song
int get_next_scale_note_control(int intial_note, bool status){
  int temp_note;
  int note_increase=12;

  if (status==true){
    temp_note=intial_note+note_increase;
    if (temp_note>105){
      return NOTE_A0;
    }else{
      return temp_note;
    }
  }else{
    if(intial_note==0){
      return NOTE_A0;
    }else{
      return intial_note;
    }
  }
}
//Function to activate each sensor in a different behavior
void sensor_on(int sensor_index){
  switch(sensor_index){
    case 0:
      noteOff(sensor_notes[sensor_index],sensor_index);
      sensor_notes[sensor_index]=get_next_scale_note_control(sensor_notes[sensor_index],sensor_active[sensor_index]);
      noteOn(sensor_notes[sensor_index], sensor_index);
      break;
    case 1:
      noteOff(sensor_notes[sensor_index],sensor_index);
      sensor_notes[sensor_index]=NOTE_G2;
      noteOn(sensor_notes[sensor_index], sensor_index);
      break;
    case 2:
      noteOff(sensor_notes[sensor_index],sensor_index);
      sensor_notes[sensor_index]=NOTE_G2;
      noteOn(sensor_notes[sensor_index], sensor_index);
      break;
    default:
      noteOff(sensor_notes[sensor_index],sensor_index);
  }
}

//Function to cut off each sensor
void sensor_off(int sensor_index){
  noteOff(sensor_notes[sensor_index],sensor_index);
}


void resetMeasurements(int index){
  numPeak[index] = 0; // Reinitialize the number of peak detected for all sensors
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
void noteOn(int note, int channel) {
  Serial.write(144+channel);
  Serial.write(note);
  Serial.write(0x3F);
}

// turns off a previously playing note
void noteOff(int note, int channel) {
  Serial.write(144+channel);
  Serial.write(note);
  Serial.write(0);
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



