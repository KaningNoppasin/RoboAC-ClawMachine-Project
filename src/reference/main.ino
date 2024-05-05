// #include <Arduino.h>
#include <AccelStepper.h>
#include "./init_IO.h"

/*

*    -------
*    |  Z  Y
*    |  |  |
*    X1---X2

*/

#define Mode 1

AccelStepper stepperX1(Mode, PUL_X1, DIR_X1);
AccelStepper stepperX2(Mode, PUL_X2, DIR_X2);
AccelStepper stepperY(Mode, PUL_Y, DIR_Y);
AccelStepper stepperZ(Mode, PUL_Z, DIR_Z);

AccelStepper stepper[4] = {
							stepperX1,
							stepperX2,
							stepperY,
							stepperZ,
						};
// 100000
double maxSpeed = 50000;
double defaultSpeed = 50000;
int acceleration = 3000;
// int acceleration = 100;
int speedXAxis;
int speedYAxis;

double positionX1 = 0;
double positionX2 = 0;
double positionY = 0;
double positionZ = 0;

void initStepperMotor(AccelStepper &StepperMotor, double MaxSpeed, int Acceleration)
{
	// setMaxSpeed => step per sec.
	StepperMotor.setMaxSpeed(MaxSpeed);

	// setAcceleration => step per sec^2.
	// StepperMotor.setAcceleration(Acceleration);
}

int SMCutterClockWise(AccelStepper &StepperMotor, int currentPosition, int round)
{
	round = !round ? 1 : round;
	// currentPosition += 3200 * round;
	currentPosition += 10 * round;
	StepperMotor.moveTo(currentPosition);
	return currentPosition;
}

int SMClockWise(AccelStepper &StepperMotor, int currentPosition, int round)
{
	round = !round ? 1 : round;
	// currentPosition -= 3200 * round;
	currentPosition -= 10 * round;
	StepperMotor.moveTo(currentPosition);
	return currentPosition;
}

void GetInputValue()
{
	// Serial.print("StartButton:" + String(digitalRead(StartButton)));
	// Serial.print("\tKeep:" + String(digitalRead(Keep)));
	Serial.print("KeepA_ZAxis:" + String(digitalRead(KeepA_ZAxis)));
	Serial.print("\tKeepB_ZAxis:" + String(digitalRead(KeepB_ZAxis)));
	Serial.print("\tJoyA_YAxis:" + String(digitalRead(JoyA_YAxis)));
	Serial.print("\tJoyB_YAxis:" + String(digitalRead(JoyB_YAxis)));
	Serial.print("\tJoyC_XAxis:" + String(digitalRead(JoyC_XAxis)));
	Serial.println("\tJoyD_XAxis:" + String(digitalRead(JoyD_XAxis)));
}

void GetPosition()
{
	Serial.print("PositionX1 :" + String(positionX1));
	Serial.print("\tPositionX2 :" + String(positionX2));
	Serial.print("\tPositionY :" + String(positionY));
	Serial.println("\tPositionZ :" + String(positionZ));
}

void GetCurrentPosition()
{
	Serial.print("CurrentPositionX1 :" + String(stepperX1.currentPosition()));
	Serial.print("\tCurrentPositionX2 :" + String(stepperX2.currentPosition()));
	Serial.print("\tCurrentPositionY :" + String(stepperY.currentPosition()));
	Serial.println("\tCurrentPositionZ :" + String(stepperZ.currentPosition()));
}

void SetHome()
{
	positionX1 = 0;
	positionX2 = 0;
	positionY = 0;
	positionZ = 0;

	stepperX1.moveTo(0);
	stepperX2.moveTo(0);
	stepperY.moveTo(0);
	stepperZ.moveTo(0);
}

void testStepperMotor()
{
	if (Serial.available())
	{
		String axis = Serial.readStringUntil(' ');
		String direction = Serial.readStringUntil(' ');
		int round = Serial.parseInt();

		if (axis == "X1" && direction == "ccw")
		{
			SMCutterClockWise(stepperX1, positionX1, round);
		}
		else if (axis == "X1" && direction == "cw")
		{
			SMClockWise(stepperX1, positionX1, round);
		}
		else if (axis == "X2" && direction == "ccw")
		{
			SMCutterClockWise(stepperX2, positionX2, round);
		}
		else if (axis == "X2" && direction == "cw")
		{
			SMClockWise(stepperX2, positionX2, round);
		}
		else if (axis == "Y" && direction == "ccw")
		{
			SMCutterClockWise(stepperY, positionY, round);
		}
		else if (axis == "Y" && direction == "cw")
		{
			SMClockWise(stepperY, positionY, round);
		}
		else if (axis == "Z" && direction == "ccw")
		{
			positionZ = SMCutterClockWise(stepperZ, positionZ, round);
		}
		else if (axis == "Z" && direction == "cw")
		{
			positionZ = SMClockWise(stepperZ, positionZ, round);
		}
	}
}

void setup()
{
	Serial.begin(115200);

	// for (int i = 0; i < 8; i++) pinMode(pinOUT[i], OUTPUT);

	for (int i = 0; i < 6; i++) pinMode(pinIN[i], INPUT_PULLUP);

	Serial.println("Start: mawaaa");
	// stepperZ.setMaxSpeed(10000);
	// stepperZ.setSpeed(defaultSpeed);

	// stepperZ.setMaxSpeed(defaultSpeed);
    // stepperZ.setSpeed(defaultSpeed);

	initStepperMotor(stepperX1, maxSpeed, acceleration);
	initStepperMotor(stepperX2, maxSpeed, acceleration);
	initStepperMotor(stepperY, maxSpeed, acceleration);
	initStepperMotor(stepperZ, maxSpeed, acceleration);
	stepperZ.setSpeed(defaultSpeed);
	// for (int i = 0; i < 4; i++) initStepperMotor(stepper[i], maxSpeed, acceleration);


}

void loop()
{
	delay(1);
	// ReadPosition();
	// GetCurrentPosition();
	// GetInputValue();

	if (!digitalRead(JoyC_XAxis))
	{
		stepperY.setSpeed(defaultSpeed);
		// positionY = SMClockWise(stepperY, positionY, 1);
		stepperX1.setSpeed(0);
		stepperX2.setSpeed(0);
		stepperZ.setSpeed(0);
	}
	else if (!digitalRead(JoyD_XAxis))
	{
		stepperY.setSpeed(-defaultSpeed);
		// positionY = SMCutterClockWise(stepperY, positionY, 1);
		stepperX1.setSpeed(0);
		stepperX2.setSpeed(0);
		stepperZ.setSpeed(0);
	}
	else if (!digitalRead(JoyA_YAxis))
	{
		stepperX1.setSpeed(-defaultSpeed);
		stepperX2.setSpeed(defaultSpeed);
		// positionX1 = SMCutterClockWise(stepperX1, positionX1, 1);
		// positionX2 = SMCutterClockWise(stepperX2, positionX2, 1);
		stepperY.setSpeed(0);
		stepperZ.setSpeed(0);
	}
	else if (!digitalRead(JoyB_YAxis))
	{
		// positionX1 = SMClockWise(stepperX1, positionX1, 1);
		// positionX2 = SMClockWise(stepperX2, positionX2, 1);
		stepperX1.setSpeed(defaultSpeed);
		stepperX2.setSpeed(-defaultSpeed);

		stepperY.setSpeed(0);
		stepperZ.setSpeed(0);
	}
	// else if (!digitalRead(Keep))
	// {
	// 	// positionZ = SMClockWise(stepperX1, positionX1, 1);
	// 	// SetHome();
	// 	stepperZ.setSpeed(defaultSpeed);
	// }
	else if (!digitalRead(KeepA_ZAxis))
	{
		// positionZ = SMClockWise(stepperX1, positionX1, 1);
		// SetHome();
		stepperZ.setSpeed(defaultSpeed);

		stepperX1.setSpeed(0);
		stepperX2.setSpeed(0);
		stepperY.setSpeed(0);
	}
	else if (!digitalRead(KeepB_ZAxis))
	{
		// positionZ = SMClockWise(stepperX1, positionX1, 1);
		// SetHome();
		stepperZ.setSpeed(-defaultSpeed);

		stepperX1.setSpeed(0);
		stepperX2.setSpeed(0);
		stepperY.setSpeed(0);
	}
	else
	{
		// for (int i = 0; i < 4; i++) stepper[i].setSpeed(0);
		stepperX1.setSpeed(0);
		stepperX2.setSpeed(0);
		stepperY.setSpeed(0);
		stepperZ.setSpeed(0);
	}

	// for (int i = 0; i < 4; i++) stepper[i].runSpeed();
	stepperX1.runSpeed();
	stepperX2.runSpeed();
	stepperY.runSpeed();
	// Serial.println("CurrentPositionZ :" + String(stepperZ.currentPosition()));
    // if (stepperZ.currentPosition() >= 10000){
    //     stepperZ.setSpeed(0);
    // }
	stepperZ.runSpeed();

	// stepperX1.run();
	// stepperX2.run();
	// stepperY.run();
	// stepperZ.run();
}

// * Example
// #include <AccelStepper.h>
// * 1
// stepperZ.setMaxSpeed(10000);
// stepperZ.setAcceleration(5000);
// stepperZ.runToNewPosition(-3200);

// * 2
// stepperZ.setMaxSpeed(10000);
// stepperZ.setAcceleration(5000);
// stepperZ.moveTo(3200);
// stepperZ.run();

// * 3
// stepperZ.setMaxSpeed(10000);
// stepperZ.setSpeed(5000);
// stepperZ.moveTo(3200);
// stepperZ.runSpeedToPosition();

// * 4
// stepperZ.setMaxSpeed(10000);
// stepperZ.setSpeed(5000);
// stepperZ.runSpeed();

