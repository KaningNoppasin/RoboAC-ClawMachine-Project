// #include <Arduino.h>
#include <AccelStepper.h>
#include "StepperController.h"

/*

*    -------
*    |  Z  Y
*    |  |  |
*    X1---X2

*/

#define Mode 1
#define PUL_Z 26
#define DIR_Z 24

AccelStepper stepperZ(Mode, PUL_Z, DIR_Z);

// defaultSpeed = 1000 4step y,z Axis Ok with delay 1 ms


double defaultSpeed = -1000;
String data;

void setup()
{
	Serial.begin(115200);
    stepperZ.setMaxSpeed(10000);
    stepperZ.setAcceleration(500);
    stepperZ.setSpeed(defaultSpeed);

    stepperZ.runToNewPosition(10000);
    // stepperZ.moveTo(1000);

}

void loop()
{

    delay(1);
	

    Serial.print("\t Z:");Serial.println(stepperZ.currentPosition());

	// stepperZ.runSpeed();
	// stepperZ.run();

}
