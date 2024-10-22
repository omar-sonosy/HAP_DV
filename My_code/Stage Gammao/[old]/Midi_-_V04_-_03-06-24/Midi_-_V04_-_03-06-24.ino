#include <MIDI.h>

// https://www.youtube.com/watch?v=Twx0kzxXvp4&list=PL4_gPbvyebyH2xfPXePHtx8gK5zPBrVkg&index=4
 
#define LED 13

int threshold = 200; // threshold of knocking of the tribo
const int maxcurent = 1023; // max current read of the tribo

int val0;
int val1;
int val2; 
int val3; 
int val4; 
int val5;

// define the analog input pins for the sensors
const int sensorPin0 = 0;
const int sensorPin1 = 1;
const int sensorPin2 = 2;
const int sensorPin3 = 3;
const int sensorPin4 = 4;
const int sensorPin5 = 5;

int threshold0 = threshold;    // threshold of knocking of the tribo
unsigned long startTime0 = 0;  // record the start time of the knock
boolean active0 = 0;           // represent whether it's in the "knocking" period
boolean firstHit0 = 1;         // represet wether it's the first time to knock
boolean setTime0 = 0;          // represent wether it has the starting time

int threshold1 = threshold;    // threshold of knocking of the tribo
unsigned long startTime1 = 0;  // record the start time of the knock
boolean active1 = 0;           // represent whether it's in the "knocking" period
boolean firstHit1 = 1;         // represet wether it's the first time to knock
boolean setTime1 = 0;          // represent wether it has the starting time

int threshold2 = threshold;    // threshold of knocking of the tribo
unsigned long startTime2 = 0;  // record the start time of the knock
boolean active2 = 0;           // represent whether it's in the "knocking" period
boolean firstHit2 = 1;         // represet wether it's the first time to knock
boolean setTime2 = 0;          // represent wether it has the starting time

int threshold3 = threshold;    // threshold of knocking of the tribo
unsigned long startTime3 = 0;  // record the start time of the knock
boolean active3 = 0;           // represent whether it's in the "knocking" period
boolean firstHit3 = 1;         // represet wether it's the first time to knock
boolean setTime3 = 0;          // represent wether it has the starting time

int threshold4 = threshold;    // threshold of knocking of the tribo
unsigned long startTime4 = 0;  // record the start time of the knock
boolean active4 = 0;           // represent whether it's in the "knocking" period
boolean firstHit4 = 1;         // represet wether it's the first time to knock
boolean setTime4 = 0;          // represent wether it has the starting time

int threshold5 = threshold;    // threshold of knocking of the tribo
unsigned long startTime5 = 0;  // record the start time of the knock
boolean active5 = 0;           // represent whether it's in the "knocking" period
boolean firstHit5 = 1;         // represet wether it's the first time to knock
boolean setTime5 = 0;          // represent wether it has the starting time

unsigned long offTime = 150;  // represent the ending time of the knocking
unsigned long duree = 800;
const int note0 = 80;
const int note1 = 40;
const int note2 = 50;
const int note3 = 60;
const int note4 = 70;
const int note5 = 80;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() 
{
  pinMode (LED, OUTPUT);
  MIDI.begin(MIDI_CHANNEL_OFF);
  // Serial.begin(9600);
  
  // Setup - MIDI IN
  MIDI.setHandleNoteOn(MyHandleNoteOn);
  MIDI.setHandleNoteOff(MyHandleNoteOff);
}

void MyHandleNoteOn(byte channel, byte pitch, byte velocity)
{
  digitalWrite(LED,HIGH); //turn LED ON 
}

void MyHandleNoteOff(byte channel, byte pitch, byte velocity)
{
  digitalWrite(LED,LOW); //turn LED OFF 
}

void loop() 
{
  // lecture entrée analogique

  
  // val0 = analogRead(sensorPin0);
  val1 = analogRead(sensorPin1);
  // val2 = analogRead(sensorPin2);
  // val3 = analogRead(sensorPin3);
  // val4 = analogRead(sensorPin4);
  val5 = analogRead(sensorPin5);
  
  // Serial.print(" Pin0 = ");
  // Serial.println(val0);
  // Serial.print(" Pin1 = ");
  // Serial.println(val1);
  // Serial.print(" Pin2 = ");
  // Serial.println(val2);
  // Serial.print(" Pin3 = ");
  // Serial.println(val3);
  // Serial.print(" Pin4 = ");
  // Serial.println(val4);
  // Serial.print(" Pin5 = ");
  // Serial.println(val5);
  // delay(1000);

  threshold = val5;
  threshold0 = val5;
  threshold1 = val5;
  threshold2 = val5;
  threshold3 = val5;
  threshold4 = val5;

  if(val0 > threshold0)
  {
    if (firstHit0 == 1) // if it's the first time
    {       
      if (setTime0 == 0)  // if there is no starting time
      {     
        startTime0 = micros();  // recoding the starting time
        setTime0 = 1;
      }
      
      if (micros() - startTime0 > duree) 
      {
        firstHit0 = 0;
        setTime0 = 0;
      }
    } 
    else 
    {
      if((active0 == 0)) 
      { 
        // val0 = map(val0, threshold0, maxcurent, 50, 127);
        val0 = map(val0, 0, maxcurent, 50, 127);
        digitalWrite(LED, HIGH);
        MIDI.sendNoteOn(note0,val0,2);
        active0 = 1;
        startTime0 = millis();
      }
    }
  }
  
  if(active0 == 1)
  {
    if(millis() - startTime0 > offTime)
    {
      active1 = 0;
      digitalWrite(LED,LOW);
      MIDI.sendNoteOff(note0,0,1); 
    }
  } 

  if(val1 > threshold1)
  {
    if (firstHit1 == 1) // if it's the first time
    {       
      if (setTime1 == 0)  // if there is no starting time
      {     
        startTime1 = micros();  // recoding the starting time
        setTime1 = 1;
      }
      
      if (micros() - startTime1 > duree) 
      {
        firstHit1 = 0;
        setTime1 = 0;
      }
    } 
    else 
    {
      if((active1 == 0)) 
      { 
        // val1 = map(val1, threshold1, maxcurent, 50, 127);
        val1 = map(val1, 0, maxcurent, 50, 127);
        digitalWrite(LED, HIGH);
        MIDI.sendNoteOn(note1,val1,1);
        active1 = 1;
        startTime1 = millis();
      }
    }
  }
  
  if(active1 == 1)
  {
    if(millis() - startTime1 > offTime)
    {
      active1 = 0;
      digitalWrite(LED,LOW);
      MIDI.sendNoteOff(note1,0,1); 
    }
  } 

  if(val2 > threshold2)
  {
    if (firstHit2 == 1) // if it's the first time
    {       
      if (setTime2 == 0)  // if there is no starting time
      {     
        startTime2 = micros();  // recoding the starting time
        setTime2 = 1;
      }
      
      if (micros() - startTime2 > duree) 
      {
        firstHit2 = 0;
        setTime2 = 0;
      }
    } 
    else 
    {
      if((active2 == 0)) 
      { 
        val2 = map(val2, threshold2, maxcurent, 50, 127);
        digitalWrite(LED, HIGH);
        MIDI.sendNoteOn(note2,val2,1);
        active2 = 1;
        startTime2 = millis();
      }
    }
  }
  
  if(active2 == 1)
  {
    if(millis() - startTime2 > offTime)
    {
      active2 = 0;
      digitalWrite(LED,LOW);
      MIDI.sendNoteOff(note2,0,1); 
    }
  } 

  if(val3 > threshold3)
  {
    if (firstHit3 == 1) // if it's the first time
    {       
      if (setTime3 == 0)  // if there is no starting time
      {     
        startTime3 = micros();  // recoding the starting time
        setTime3 = 1;
      }
      
      if (micros() - startTime3 > duree) 
      {
        firstHit3 = 0;
        setTime3 = 0;
      }
    } 
    else 
    {
      if((active3 == 0)) 
      { 
        val3 = map(val3, threshold1, maxcurent, 50, 127);
        digitalWrite(LED, HIGH);
        MIDI.sendNoteOn(note3,val3,2);
        active3 = 1;
        startTime3 = millis();
      }
    }
  }
  
  if(active3 == 1)
  {
    if(millis() - startTime3 > offTime)
    {
      active3 = 0;
      digitalWrite(LED,LOW);
      MIDI.sendNoteOff(note3,0,2); 
    }
  } 

  if(val4 > threshold4)
  {
    if (firstHit4 == 1) // if it's the first time
    {       
      if (setTime4 == 0)  // if there is no starting time
      {     
        startTime4 = micros();  // recoding the starting time
        setTime4 = 1;
      }
      
      if (micros() - startTime4 > duree) 
      {
        firstHit4 = 0;
        setTime4 = 0;
      }
    } 
    else 
    {
      if((active4 == 0)) 
      { 
        val4 = map(val4, threshold4, maxcurent, 50, 127);
        digitalWrite(LED, HIGH);
        MIDI.sendNoteOn(note4,val4,2);
        active4 = 1;
        startTime4 = millis();
      }
    }
  }
  
  if(active4 == 1)
  {
    if(millis() - startTime4 > offTime)
    {
      active4 = 0;
      digitalWrite(LED,LOW);
      MIDI.sendNoteOff(note4,0,2); 
    }
  } 

 
  
  // Partie code MIDI OUT  
  // digitalWrite(LED, HIGH);
  // MIDI.sendNoteOn(note0,127,1);
  // delay(1000);
  // MIDI.sendNoteOn(note1,127,1);
  // delay(1000);
  // MIDI.sendNoteOn(note2,127,1);
  // delay(1000);
  // MIDI.sendNoteOn(note3,127,2);
  // delay(1000);
  // MIDI.sendNoteOn(note4,127,2);
  // delay(1000);
  // MIDI.sendNoteOn(note5,127,3);
  // delay(1000);
  // MIDI.sendNoteOff(note2,0,1);
  // MIDI.sendNoteOff(note4,0,2);
  // MIDI.sendNoteOff(note5,0,3);
  // digitalWrite(LED,LOW);
  // delay(1000);

  // Partie Code MIDI IN
  // MIDI.read();
}
