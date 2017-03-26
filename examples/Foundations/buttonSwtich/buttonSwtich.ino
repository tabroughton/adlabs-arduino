// declare constants
const int GREEN_LED = 7;
const int BUTTON = 2;

// declare variables
int greenLEDState = LOW; // declare green led state to toggle
int buttonState = LOW;

void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void loop() {
  buttonState = digitalRead(BUTTON); // check the state of the LED button 
  
  if(buttonState == HIGH){ // if button state is pressed then
    greenLEDState = !greenLEDState; // toggle the state of the LED
  }

  digitalWrite(GREEN_LED, greenLEDState); // send the state to the actual LED  
}

