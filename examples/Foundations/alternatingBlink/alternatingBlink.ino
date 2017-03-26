// declare constants
const int GREEN_LED = 7;
const int BLUE_LED = 8;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);  // set blue LED pin as output
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(GREEN_LED, HIGH); // green LED is on
  digitalWrite(BLUE_LED, LOW);  // blue LED is off
  delay(1000);              
  digitalWrite(GREEN_LED, LOW);  // green LED is off
  digitalWrite(BLUE_LED, HIGH);  // blue LED is on 
  delay(1000);             
}


