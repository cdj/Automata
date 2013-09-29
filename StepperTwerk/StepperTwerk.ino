/*
 * Based off of documentation
 *
 * http://www.arduino.cc/en/Reference/Stepper
 * This example code is in the public domain.
 */

#include <Stepper.h>

#define STEPS 64

Stepper stepper(STEPS, 8, 9, 10, 11);

int previous = 0;
bool cw = true;

void setup()
{
  stepper.setSpeed(380);
}

void loop()
{
  if(cw){
    stepper.step(18);
  } else {
    stepper.step(-18);
  }
  cw = !cw;
  delay(100);
}
