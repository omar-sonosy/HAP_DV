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


void setup() {
  // setup serial port at 115200 to communicate via MIDI
  Serial.begin(115200);
}

int num=20;
void loop() {
  // play a specific note
  //noteOn(NOTE_C4, 0x3F, 500);  // play Middle C at half velocity for 500ms
  //noNote(500); // play no note, aka rest, for 500ms
  
  // play a random not
  noteOn(num, 100);
  //delay(5000);
  num++;
  delay(1000);
  if(num>31){
    //delay(200000);
    num=20;
  }

  
  //  delay(1000);  // play random note C at half velocity for 200-500ms
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
