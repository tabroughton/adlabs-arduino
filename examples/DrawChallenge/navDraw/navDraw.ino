/*
* ADVNTURE LABS NAVIGATION DRAWING CHALLENGE
*
* In this program you will build a GPS device that
* can track you as you navigate.
*
* Follow the instructions on the requirements you're
* working with and activate the components in the
* setup() function where it says to do so.
*/

//include libraries we need to use
#include <TinyGPS++.h> //has all the functions needed for GPS
#include <SoftwareSerial.h> //allows us to use comms on other pins
#include <SPI.h> //adds serial peripheral interface (more comms)
#include <SD.h> //adds functionality required for SD card
#include <NavigationDraw.h>

//declare constants (these do not change)
const int RXPIN = 5; //pin for receiving GPS data
const int TXPIN = 6; //pin for transmitting to GPS device
const int CHIPSELECT = 4; //pin used by SD card reader
const long TRACK_DELAY = 1000; //write gps coordinates every second

//declare objects
TinyGPSPlus gps;
SoftwareSerial softSerial(RXPIN, TXPIN);
NavigationDraw navDraw;

// declare variables
int readyState = LOW;
int trackingState = LOW;
int buttonState = LOW;
int lastButtonState = LOW;
long trackTimer = 0;
String gpsString = "";

void setup() {
  Serial.begin(9600);
  Serial.println(F("Latitude, Longitude"));
  softSerial.begin(9600);

  //---- START ACTIVATING COMPONENTS -----
  navDraw.activateGPS();

  //---- STOP ACTIVATING COMPONENTS ------

  if(navDraw.isSDActivated()){
    if(!SD.begin(CHIPSELECT)) {
      displayError(F("Card failed, or not present"));
    }else{
      Serial.println(F("saving heading"));
      saveStringToFile(F("Latitude, Longitude"));
    }
  }
}


void loop() {
  //if we have activated the tracking button check to see if we should track
  if(navDraw.isTrackingButtonActivated()){
    lastButtonState = buttonState;
    buttonState = digitalRead(navDraw.getTrackingButtonPin());
    if(buttonState == HIGH && lastButtonState == LOW && readyState == HIGH){
      trackingState = !trackingState;
    }
  }else{
    trackingState = HIGH;
  }

  if(navDraw.isGPSActivated()) checkGPS();

  if(navDraw.isReadyLEDActivated()) digitalWrite(navDraw.getReadyLEDPin(), readyState);
  if(navDraw.isTrackingLEDActivated()) digitalWrite(navDraw.getTrackingLEDPin(), trackingState);
}


void checkGPS(){
  while(softSerial.available() > 0)//if we have a date from the GPS sensor
    gps.encode(softSerial.read()); //send the data to the gps object to interpret it

  if(gps.location.isValid()){
    readyState = HIGH;
    if(trackingState == HIGH && trackTimer + TRACK_DELAY < millis()){
      trackTimer = millis();
      gpsString = String(gps.location.lat(), 7);
      gpsString += ",";
      gpsString += String(gps.location.lng(), 7);
      // the coordinates to screen
      Serial.println(gpsString);
      //save the coordinates into the file
      if(navDraw.isSDActivated()) saveStringToFile(gpsString);
    }
  }else{
    readyState = LOW;
    trackingState = LOW;
  }
  if (millis() > 5000 && gps.charsProcessed() < 10) displayError(F("No GPS detected: check wiring."));
}

void saveStringToFile(String stringToSave){
  File dataFile = SD.open("gpslog.txt", FILE_WRITE);
  if(dataFile){
    dataFile.println(stringToSave);
    dataFile.close();
  }else{
    displayError(F("Error saving to file"));
  }
}

void displayError(String errString){
  Serial.println(errString);
  if(navDraw.isErrorLEDActivated()) digitalWrite(navDraw.getErrorLEDPin(), HIGH);
  while(true);
}
