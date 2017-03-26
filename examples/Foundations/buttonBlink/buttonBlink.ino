// declare constants
const int GREEN_LED = 7;
const int BUTTON = 2;

// declare variables
int buttonState = LOW;

void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void loop() {
  buttonState = digitalRead(BUTTON); // store the state of the LED button 

  // Read the value of the button and write it to the LED
  digitalWrite(GREEN_LED, buttonState);     
}

