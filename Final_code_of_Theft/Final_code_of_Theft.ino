#include <SoftwareSerial.h>


const String PHONE = "+918526452896"; // enter the phone number of inserted sim in GSM moule 

// GSM Module RX pin to Arduino 3
// GSM Module TX pin to Arduino 2
#define rxPin 2
#define txPin 3
SoftwareSerial sim800(rxPin, txPin);


int pir_sensor = 11;
int ledPin = 13;
bool callInitiated = false;
unsigned long callStartTime = 0;
const unsigned long callDuration = 20000; 

void setup() {
  pinMode(pir_sensor, INPUT);    
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);    
  Serial.begin(115200);          

  sim800.begin(9600);
  Serial.println("SIM800L software serial initialize");

  sim800.println("AT");
  delay(1000);
}

void loop() {
  while (sim800.available()) {
    Serial.println(sim800.readString());
  }

  while (Serial.available()) {
    sim800.println(Serial.readString());
  }

  int val = digitalRead(pir_sensor); 
  if (val == HIGH && !callInitiated) { 
    Serial.println("Motion detected!");
    digitalWrite(ledPin, HIGH); 

    Serial.println("Calling....");
    sim800.println("ATD" + PHONE + ";");
    callInitiated = true;
    callStartTime = millis(); 
  }

  if (callInitiated && (millis() - callStartTime >= callDuration)) {
    sim800.println("ATH"); 
    callInitiated = false; 
    digitalWrite(ledPin, LOW); 
  }
}
