#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "rgb_lcd.h"

//Components:
const int RXPIN = 5; //pin for receiving GPS data
const int TXPIN = 6; //pin for transmitting to GPS device
int buzzerPin = 3;
//note to engineers: LCD Screen plugs into I2C socket.

//constants for conditional statements
const long TRACK_DELAY = 1000;
const long BUZZER_DELAY = 10000;

// lat long to test
float testLats = 53.318896;
float testLong = -1.9233584;
long iLat = (long)(testLats*10000);
long iLong = (long)(testLong*10000);

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
  displayMessage(100, 0, 0, "GPS Tester", "Starting");
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
      if(lattitude == iLat || lattitude == iLat+1 || lattitude == iLat-1){
        if(longitude == iLong || longitude == iLong+1 || longitude == iLong-1){
          displayMessage(0, 100, 0, "GPS Matched", "Test Complete");
          //sounds an alarm that the grid reference is displayed
          if(lastTimeBuzzed + BUZZER_DELAY < millis()){
            /***************************************
             *     REPLACE THIS BLOCK WITH
             *       MISSING BUZZER CODE
             **************************************/
            lastTimeBuzzed = millis();
          }
          lastState = deviceStates[2];
        }
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



