// declare constants
const int GREEN_LED = 7;
const int BUTTON = 2;

// declare variables
int greenLEDState = LOW; 
int buttonState = LOW;
int lastButtonState = LOW; // to store the button state for the next loop

void setup() {
  Serial.begin(9600); 
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void loop() {
  lastButtonState = buttonState;  // store the button state before we  update it  
  buttonState = digitalRead(BUTTON); 
  
  if(buttonState == HIGH && lastButtonState == LOW){  // now check previous button state
    greenLEDState = !greenLEDState; 
    Serial.print("toggled LED State to: "); 
    Serial.println(greenLEDState); 
  }

  digitalWrite(GREEN_LED, greenLEDState); 
}

