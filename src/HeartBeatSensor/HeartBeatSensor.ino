//****************************************
//                LIBRARYS
//****************************************
//general
#include <stdlib.h>
//arduino
#include <Arduino.h>
#include <math.h>
//display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//project
#include "HeartBeatSensor.h"
 
//****************************************
//                TABLES
//****************************************


//****************************************
//                GLOBALS
//****************************************
const int i16BuzzerPin = DI_PIN_9;

// We'll set up an array with the notes we want to play
// change these values to make different songs!
// Length must equal the total number of notes and spaces 
const int i16SongLength = 18;

// Notes is an array of text characters corresponding to the notes
// in your song. A space represents a rest (no tone)

//char notes[] = "cdfda ag cdfdg gf "; // a space represents a rest
char u8NotesArr[] = "C C C C "; // a space represents a rest

// Beats is an array values for each note and rest.
// A "1" represents a quarter-note, 2 a half-note, etc.
// Don't forget that the rests (spaces) need a length as well.

//int beats[] = {1,1,1,1,1,1,4,4,2,1,1,1,1,1,1,4,4,2};
int i16BeatsArr[] = {2,1,2,1,2,1,2,1,1,1,1,1,1,1,1,1,1,1};

// The tempo is how fast to play the song.
// To make the song play faster, decrease this value.
int i16Tempo = 150;

// calculated heart rate from heart rate sensor
int i16HeartRate = 0;

//raw heart rate analog value
int i16SensorVal = 0;


//****************************************
//         FUNCTION DECLARATIONS
//****************************************
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// //****************************************
// //         FUNCTION DEFINITIONS
// //****************************************
void playTone(void) {
  int i, duration;
  
  for (i = 0; i < i16SongLength; i++) // step through the song arrays
  {
    duration = i16BeatsArr[i] * i16Tempo;  // length of note/rest in ms
    
    if (u8NotesArr[i] == ' ')          // is this a rest? 
    {
      delay(duration);            // then pause for a moment
    }
    else                          // otherwise, play the note
    {
      tone(i16BuzzerPin, frequency(u8NotesArr[i]), duration);
      delay(duration);            // wait for tone to finish
    }
    delay(i16Tempo/10);              // brief pause between notes
  }
}

int frequency(char u8Note) {
  // This function takes a note character (a-g), and returns the
  // corresponding frequency in Hz for the tone() function.
  
  int i16Idx;
  const int i16NumNotes = 8;  // number of notes we're storing  
  // The following arrays hold the note characters and their
  // corresponding frequencies. The last "C" note is uppercase
  // to separate it from the first lowercase "c". If you want to
  // add more notes, you'll need to use unique characters.

  // For the "char" (character) type, we put single characters
  // in single quotes.

  char u8NamesArr[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  const int i16FrequenciesArr[] = {262, 294, 330, 349, 392, 440, 494, 523};
  
  // Now we'll search through the letters in the array, and if
  // we find it, we'll return the frequency for that note.
  
  for (i16Idx = 0; i16Idx < i16NumNotes; i16Idx++)  // Step through the notes
  {
    if (u8NamesArr[i16Idx] == u8Note)         // Is this the one?
    {
      return(i16FrequenciesArr[i16Idx]);     // Yes! Return the frequency
    }
  }
  
  return(0);  // We looked through everything and didn't find it,
              // but we still need to return a value, so return 0.
}


void setup() 
{
  //begin serial communication
  Serial.begin(115200);
  //display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){ //initialize with the I2C addr 0x3C (128x64)
    Serial.println("Display (SSD1306) allocation failed");
  } 
  display.clearDisplay();
  //pin mode setup 
  pinMode(8, OUTPUT);
  //delay time
  delay(10);
  pinMode(i16BuzzerPin, OUTPUT);
}

void showStats(int heartRate) 
{ 
  char charVal[5]; 
  display.clearDisplay();   
  dtostrf(heartRate, 4, 0, charVal);

  //show cardiogramm
  //display.writeLine();
  display.drawLine(0, 0, 10, 10, WHITE); 

  //show beats per minute
  display.writeFillRect(0,50,128,16,BLACK);
  display.setCursor(10, 35);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("Heart Rate");  
  display.setCursor(30, 50);
  display.setTextSize(2);
  display.print(charVal);
  display.setTextSize(1);
  display.print("BPM");

  display.display();
}


void loop() 
{
  //show statistics on lcd terminal  
  showStats(60);

  //alarm if heart rate is to low
  if (i16HeartRate < 60) {
    playTone();
  }
  
  delay(10);

  // We only want to play the song once, so we'll pause forever:
  while(true){}
  // If you'd like your song to play over and over,
  // remove the above statement
}
