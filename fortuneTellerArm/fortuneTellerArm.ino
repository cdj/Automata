#include <Event.h>
#include <Timer.h>

//
//Fortune teller program for Dagu Robot Arm
//2013 Carl Jamilkowski and Will Canine, based on code by Nick Yulman 
//NYU ITP class Automata: telling Stories With Machines


#include <Servo.h> 

byte ledPin = 4;
byte buttonPin = 3;

Timer t;
byte updateSpeed = 20; //how often will we call out update function

const byte numServos =6; // how many joints in out arm?
Servo myServo[numServos];  //create an array of servo objects - 1 for each joint        
boolean hitTarget[numServos]; //target state booleans for each servo
byte numReached = 0; //how many servos have reached their targets?

//Magic Eight Ball 
int answer=2; 

//servo position
byte pos[numServos]; //current position for each servo
byte tPos[numServos]; //target position for each servo
byte servoSpeed=2; //speed at which servo positions will increment

byte hangOut[]={90,90,90,20,90,90}; //an example of a predefined named position
byte nodUp[]={90,90,90,90,90,90}; //an example of a predefined named position
byte nodDown[]={90,90,90,0,90,90}; //an example of a predefined named position
byte shakeLeft[]={0,90,90,0,90,90}; //an example of a predefined named position
byte shakeRight[]={100,90,90,0,90,90}; //an example of a predefined named position

byte* positions[3][9] = {{shakeLeft, shakeRight, shakeLeft, shakeRight, shakeLeft, shakeRight, hangOut,hangOut,hangOut}, // NO
                         {nodUp, nodDown, nodUp, nodDown, nodUp, nodDown, nodUp, nodDown, hangOut}, // YES
                         {hangOut,hangOut,hangOut,hangOut,hangOut,hangOut,hangOut,hangOut,hangOut}}; // REST

//timing variables
long waitTimes[]={250};  //sequence of wait times
long currentWait =250;
long timeMark;

byte numPos;  //how many positions are we cycling through - we'll set the value later
byte counter =0; //the current set of target positions

//button state variables
boolean buttonState = 0; 
boolean pButtonState = 0;

boolean running = false; //running state variable
boolean randomizer = false;  

void setup() 
{ 
  Serial.begin(9600);
  delay(300);
  for(int i = 0; i<numServos; i++){
    myServo[i].attach(8+i);  // attaches the servos to 6 pins 8-13
    hitTarget[i] =0; //initialize out hit target booleans to 0
    
  }
  numPos=sizeof(positions[1])/(sizeof(byte(*)[9]));
  


  Serial.print("there are ");
  Serial.print(numPos);
  Serial.println(" positions");

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  //initialize timer event.
  int updateEvent = t.every(updateSpeed, updateServos);
} 


void loop() 
{ 
  if(running){   
    t.update();
  }
  checkButton();
}

void updateServos(){
  
  if(millis()-timeMark>currentWait){ //if the current waitTime  has ellapsed
    for(int i = 0; i<numServos; i++){
      {
        if(!hitTarget[i]){ //if the servo hasn't hit it's target yet
          if(pos[i]<tPos[i]){ //if the current position is lower than the target
            pos[i]+=servoSpeed; //add the servo speed
          }
          else if(pos[i]>tPos[i]){//if the current position is higher than the target
            pos[i]-=servoSpeed;     //subtract the servo speed
          } 
          myServo[i].write(pos[i]);  //write the new position to the servo
       

        if(abs(tPos[i]-pos[i])<servoSpeed){  //if the servo has reached its target
          hitTarget[i]=1;           //change the target state boolean for that servo
          numReached++;             //add one to our number reached count
          Serial.print("Servo# ");  //tell us which servo got where
          Serial.print(i+1);
          Serial.print(" got to ");
          Serial.println(positions[answer][counter][i]);
           }
        }
      }
    }

    if(numReached==numServos){    //if all the servos have reached their targets
      Serial.print("I reached position ");  //tell us about it
      Serial.println(counter+1);

      timeMark=millis();  //mark the time when we completed the current action
      //we'll start the wait time from here
      Serial.print("Number of positions: ");
      Serial.println(numPos);
      Serial.print("Counter: ");
      Serial.println(counter);
      if( counter<numPos-1){ //if we aren't at the end of the cycle  
        currentWait=waitTimes[counter];  //set the wait time
        counter++;                       //increment the counter
        numReached=0;                    //reset the number reached count
        for(int i = 0; i<numServos; i++){        //loop through our servos
          tPos[i] = positions[answer][counter][i];    //and assign new target positions
          hitTarget[i]=0;                //reset hit target state booleans
          
        }
      }
      else {
        Serial.println("Completed positions");
        Serial.println("======");
        reset();
      }
    }
  }
}


//button
void checkButton(){
  if(buttonState && !pButtonState){
    pButtonState=buttonState;
    running = !running;
    Serial.print("running= ");
    Serial.println(running);
    if(running){
        Serial.println("======");
        Serial.println("Answering question...");
        randomSeed(analogRead(0));
        answer = random(0,1);
        if (answer == 0){
           //positions=no;
           Serial.println("Arm chooses NO");
         }
         else if(answer == 1) {
           //positions = yes;
           Serial.println("Arm chooses YES");
         }
        //numPos=sizeof(positions)/(sizeof(byte(*)[9]));
        reset();
        digitalWrite(ledPin, HIGH);
      } else {
        digitalWrite(ledPin, LOW);
      }
    } else if(!buttonState && pButtonState){
      pButtonState=buttonState;
//    positions= rest;
//    numPos=sizeof(positions)/(sizeof(byte(*)[9]));
    }
    buttonState =  digitalRead(buttonPin);
}

void reset(){  //if we are at the end of the cycle reset to 0
    currentWait=waitTimes[counter];
    counter=0;
    numReached=0;
    for(int i = 0; i<numServos; i++){
      tPos[i] = positions[answer][counter][i];
      hitTarget[i]=0;
    } 
    Serial.println("RESET");
}

