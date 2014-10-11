#include "Timer.h"

Timer t;
int s1pin = 9;
int s2pin = 10;


// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(s1pin, OUTPUT);
  pinMode(s2pin, OUTPUT); 
  //t.pulse(s1pin, 2000, LOW);
//  t.oscillate(s1pin, 1000, LOW);
//  //t.pulse(s2pin, 2000, LOW);
//  t.oscillate(s2pin, 1000, LOW);
}

// the loop routine runs over and over again forever:
void loop() {
  while(true){
  digitalWrite(s1pin, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(s2pin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(s1pin, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(s2pin, LOW);    // turn the LED off by making the voltage LOW
  delay(500);               // wait for a second
  }
  //t.update();
}
