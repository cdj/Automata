#include <Stepper.h>

#define STEPS 64 // 2048 4096 4076 48 64 ? http://forum.arduino.cc/index.php?topic=71964.15

const int pinLeftEye = 12;
const int pinRightEye = 11;
const int pinHead = 3;
const int pinWink = 4;

Stepper stepper(STEPS, 7, 8, 9, 10);
unsigned long blinkNext = 0;
const int blinkIntervalMax = 6000;
const int blinkIntervalMin = 1000;
const int winkIntervalMin = 2000;
const int blinkWait = 100;
bool needsOpening = false;
unsigned long openTime = 0;
unsigned long lastWink = 0;
unsigned long endPauseTime = 0;
int stepsFromNeutral = 0;
int targetStepsFromNeutral = 0;
const int maxStepsFromNeutral = 1019; // 90 degrees
const int headTurnMaxSteps = 800;
const int headTurnMinSteps = 200;
const int headTurnInterval = 8;
const int headPauseMax = 4000;
const int headPauseMin = 1000;


void setup() {
  Serial.begin(9600);
  pinMode(pinLeftEye, OUTPUT);
  pinMode(pinRightEye, OUTPUT);
  stepper.setSpeed(100);
  pinMode(pinHead, INPUT);
  digitalWrite(pinHead, HIGH);
  pinMode(pinWink, INPUT);
  digitalWrite(pinWink, HIGH);
}

void loop() {
  if(needsOpening) {
    if(millis() >= openTime) {
      openEyes();
    }
  } else {
    // blink if not wink
    if((digitalRead(pinWink) == HIGH) && (millis() > (lastWink + winkIntervalMin))) {
      winkEye(pinLeftEye);
    } else if(millis() >= blinkNext) { // time to blink eyes?
      blinkEyes();
    }
  }
  
  // check for manual turn head
  if(digitalRead(pinHead) == HIGH) {
    Serial.println("Head button pressed");
    // turn to neutral
    targetStepsFromNeutral = 0;
    if(stepsFromNeutral == 0) {
      endPauseTime = millis() + headPauseMax; // extend pause while button is pressed
    }
  }
  
  if((millis() > endPauseTime) && (stepsFromNeutral != targetStepsFromNeutral)) {
    // moving head
    int stepsToMove = headTurnInterval;
    int diff = stepsFromNeutral - targetStepsFromNeutral;
    
    if(diff < 0) stepsToMove *= -1; // moving in other direction
    
    if(abs(diff) < headTurnInterval) {
      // at the end of a head turn, about ready to pause
      stepsToMove = diff;
    }
    
    // move head towards goal
    stepper.step(stepsToMove);
    stepsFromNeutral -= stepsToMove;
    
    if(stepsFromNeutral == targetStepsFromNeutral) {
      // start head pause
      endPauseTime = millis() + (targetStepsFromNeutral == 0) ? headPauseMax : random(headPauseMin, headPauseMax);
    }
  } else {
    // determine next head position
    int shifted = stepsFromNeutral + random(headTurnMinSteps, headTurnMaxSteps);
    if(shifted > maxStepsFromNeutral) {
      targetStepsFromNeutral = shifted - maxStepsFromNeutral;
    } else {
      targetStepsFromNeutral = shifted;
    }
  }
}


//////////////////////
// Helper Functions //
//////////////////////

void blinkEyes() {
  Serial.println("Blinking");
  digitalWrite(pinLeftEye, HIGH);
  digitalWrite(pinRightEye, HIGH);
  
  openTime = millis() + blinkWait;
  needsOpening = true;
  blinkNext = millis() + random(blinkIntervalMin, blinkIntervalMax);
}

void winkEye(int toWink) {
  Serial.println("Winking");
  digitalWrite(toWink, HIGH);
  
  openTime = millis() + blinkWait * 2; // hold the wink a little longer
  needsOpening = true;
  blinkNext = millis() + random(blinkIntervalMin, blinkIntervalMax); // won't need to blink again right away
  lastWink = millis();
}

void openEyes() {
  Serial.println("Opening eyes");
  digitalWrite(pinLeftEye, LOW);
  digitalWrite(pinRightEye, LOW);
  needsOpening = false;
}
