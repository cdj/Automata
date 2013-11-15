/*
 * Based off of documentation
 *
 * http://www.arduino.cc/en/Reference/Stepper
 * This example code is in the public domain.
 */

#include <Stepper.h>

#define STEPS 64

Stepper stepper(STEPS, 8, 9, 10, 11);

void setup()
{
  stepper.setSpeed(100);
}

void loop()
{
  stepper.step(64);
  //delay(2000);
}
