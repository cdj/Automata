#include <Servo.h>

// Globals
Servo leftServo;  // create servo object to control a servo
Servo rightServo;  // create servo object to control a servo
unsigned long blinkNext = 0;
unsigned long unblinkNext = 0;
unsigned long unblinkTime = 0;
int buttonState = 0;
boolean opening = false;
boolean closing = false;

// Settings
const int blinkIntervalMax = 6000;
const int blinkIntervalMin = 1000;
const int blinkDuration = 200;
//const int eyeOpenPos = 90;
//const int eyeClosedPos = 180;

// Pins
const int leftServoPin = 2;
const int rightServoPin = 4;
const int buttonPin = 7;


void setup() 
{ 
  leftServo.attach(leftServoPin);  // attaches the servo on pin 2 to the servo object
  rightServo.attach(rightServoPin);  // attaches the servo on pin 2 to the servo object
  pinMode(buttonPin, INPUT); 
  Serial.begin(9600);
} 

void loop() 
{
  buttonState = LOW;//digitalRead(buttonPin);
  // check for wink press
  if(buttonState == HIGH)
  {
    Serial.println("Button Pressed");
    // make sure eyes are open
    if(unblinkNext > blinkNext)
    {
      // eyes still need to be triggered to close
      leftServo.write(65);
      rightServo.write(145);
      delay(blinkDuration);
    }
    else if(millis() < unblinkTime)
    {
      delay(unblinkTime);
    }
    unblinkNext = 0;
    unblinkTime = 0;
    
    // trigger one eye
    Serial.println("Winking");
    leftServo.write(30);
    delay(blinkDuration);
    while(buttonState == HIGH)
    {
      // wait until button is no longer pressed
      buttonState = digitalRead(buttonPin);
      delay(15);
    }
    Serial.println("Un-winking");
    //open eye
    leftServo.write(65);
    delay(blinkDuration);
    //set timers
    unblinkTime = millis();
    blinkNext = unblinkTime + random(blinkIntervalMin, blinkIntervalMax);
    Serial.println("Done winking");
    
  }
  else
  {
    if(millis() > blinkNext)
    {
      if((unblinkNext != 0) && (millis() > unblinkNext) && !opening)
      {
        Serial.println("Un-blinking");
        // unblink eyes
        leftServo.write(66);
        rightServo.write(135);
        // set timers
        unblinkTime = millis() + blinkDuration;
        blinkNext = unblinkTime + random(blinkIntervalMin, blinkIntervalMax);
        unblinkNext = 0;
        closing = false;
        opening = true;
      }
      else if(!closing)
      {
        Serial.println("Blinking");
        // blink eyes
        leftServo.write(30);
        rightServo.write(180);
        // set timers
        unblinkNext = millis() + blinkDuration;
        closing = true;
        opening = false;
      }
    }
  }
  //Servo::refresh();
}


