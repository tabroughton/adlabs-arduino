// declare constants
const int GREEN_LED = 7;
const int BUTTON = 2;

// declare variables
int greenLEDState = LOW; 
int buttonState = LOW;

void setup() {
  Serial.begin(9600); // begin serial communication
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void loop() {
  buttonState = digitalRead(BUTTON); 
  
  if(buttonState == HIGH){ 
    greenLEDState = !greenLEDState; 
    Serial.print("toggled Green LED State to: "); // print out some text
    Serial.println(greenLEDState); // print value toggled to and end the line
  }

  digitalWrite(GREEN_LED, greenLEDState); 
}

