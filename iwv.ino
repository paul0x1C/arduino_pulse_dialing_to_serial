const int pin = 12; //connect one wire of the phone to this pin, the other one to ground

//variables for timing
unsigned long t = 0;
unsigned long start = 0;

//max time the user can wait between dialing digits until it's interpreted as a new number
const int timeout = 4000;

//time limits for pulse dialing digits
const int Min = 30;
const int Max = 80;

//booleans to detected changes
boolean newValue = false;
boolean lastValue = digitalRead(pin);

//variables used for building the final number
String number;
int digit = 0;
boolean gotNewDigit = false;

void setup() {
  Serial.begin(9600);
  pinMode(pin, INPUT_PULLUP);
}

void loop() {
  newValue = digitalRead(pin);
  t = millis() - start;
  
  if (newValue != lastValue){ //detect if something changed
    start = millis();
    //a digit consists of the line being closed and opened, the following code will count how often that happens
    if (!newValue && t <= Max && t >= Min){ //check if the new value is LOW and the time between the the LOW before is within the limits of valid pulse dialing
      gotNewDigit = true; //tell the following code that the we got a new digit
      digit++; 
      if(digit == 10){ //10 times LOW means 0
        digit = 0;
      }
    }
    
    lastValue = newValue;
  }
  
  if(t > Max && gotNewDigit){ //happens when the time between two LOWs exceeds the max time for valid pulse dialing and there is a new digit
                              //so basicly this will be executed after every dialed digit
    Serial.println(digit);
    number = number + String(digit); //add new digit to number String
    digit = 0; //reset the digit which was added
    gotNewDigit = false;
  }else{
    delay(1);
  }
  
  if(t >= timeout && number != ""){ //if nothing nothing changes for long enough, we can assume that the user finished dialing his number
    Serial.println("Nummer: " + number); //print number to serial console
    number = ""; //reset number
  }
}
