#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include <Base64.h>

//Components:
const int RXPIN = 5; //pin for receiving GPS data
const int TXPIN = 6; //pin for transmitting to GPS device
int buzzerPin = 3;
//note to engineers: LCD Screen plugs into I2C socket.

//constants for conditional statements
const long TRACK_DELAY = 1000;
const long BUZZER_DELAY = 10000;
const int NUMPOINTS = 10;

// The magic numbers - flight recordings:
float lats[NUMPOINTS]{
    53.319675,
    53.317421,
    53.312203,
    53.307072,
    53.306023,
    53.303873,
    53.306551,
    53.308218,
    53.312911,
    53.314868
};

float longs[NUMPOINTS]{
    -1.9218376,
    -1.9189328,
    -1.9216257,
    -1.9316760,
    -1.9355223,
    -1.9392505,
    -1.9495060,
    -1.9518033,
    -1.9571154,
    -1.9511783
};

char messages[NUMPOINTS][13]=
{
  "U0swNTU4MDAA",
  "U0swNTM3OTUA",
  "U0swNDY3ODkA",
  "U0swNDQ3ODcA",
  "U0swNDI3ODUA",
  "U0swMzQ3ODgA",
  "U0swMzQ3OTEA",
  "U0swMjk3OTUA",
  "U0swMzQ3OTcA",
  "UkVTQ1VFRAAA"
};

char deviceStates[]{
  'S', //searching for GPS
  'N', //not matched
  'M' //matched
};


//declare objects
rgb_lcd lcd;
TinyGPSPlus gps;
SoftwareSerial softSerial(RXPIN, TXPIN);

//variables to keep track of application state
long trackTimer = 0;
long lastTimeBuzzed = 0;
char lastState;

//variables to store the current position
long lattitude = 0L;
long longitude = 0L;


void displayMessage(int r=0, int g=0, int b=0, String ln1="", String ln2=""){
    lcd.setRGB(r, g, b);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(ln1);
    lcd.setCursor(0, 1);
    lcd.print(ln2);
}

void setup() {
  softSerial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  displayMessage(100, 0, 0, "GPS Challenge", "Starting");
  //set up pins
  pinMode(buzzerPin, OUTPUT);
  delay(2000);
}

void loop() {
  while(softSerial.available() > 0)//if we have a date from the GPS sensor
    gps.encode(softSerial.read()); //send the data to the gps object to interpret it
  
  if(gps.location.isValid()){ 
    if(trackTimer + TRACK_DELAY < millis()){
      trackTimer = millis();
      lattitude = (long)(gps.location.lat()*10000);
      longitude = (long)(gps.location.lng()*10000);
      String currPos = String(lattitude) + ","+String(longitude);
      int matchedIndex = -1;
      for(int i=0;i<NUMPOINTS;i++){
        long iLat = (long)(lats[i]*10000);
        long iLong = (long)(longs[i]*10000);
        if(lattitude == iLat || lattitude == iLat+1 || lattitude == iLat-1){
          if(longitude == iLong || longitude == iLong+1 || longitude == iLong-1){
            matchedIndex = i;
          }
        }
      }

      //displays the stored grid reference
      if(matchedIndex > -1){
        if(lastState != deviceStates[2]){
          if(matchedIndex == NUMPOINTS - 1){
            displayMessage(0, 100, 0, "Transmitter", "location");
          }else{
            int input2Len = sizeof(messages[matchedIndex]);
            int decodedLen = base64_dec_len(messages[matchedIndex], input2Len);
            char decoded[decodedLen];
            base64_decode(decoded, messages[matchedIndex], input2Len);
            displayMessage(0, 100, 0, "next grid ref", String(decoded));
          }
        }

        //sounds an alarm that the grid reference is displayed
        if(lastTimeBuzzed + BUZZER_DELAY < millis()){

          /***************************************
           *     REPLACE THIS BLOCK WITH
           *       MISSING BUZZER CODE
           **************************************/
          
          lastTimeBuzzed = millis();
        }
        
        lastState = deviceStates[2];
      }else{
        displayMessage(0, 0, 0, "current pos:", currPos);
        lastState = deviceStates[1];
      }
    } 
  }else{
    if(lastState != deviceStates[0]) displayMessage(100, 0, 0, "searching for", "gps signal...");
    lastState = deviceStates[0];
  }

}



