#ifndef JOY_FILE
#define JOY_FILE


#include "./init_IO.h"
#include "./StepperController.ino"



void JoyController(){
    // setSpeedAndLimitPositionOutOfRange
    // Y
    if (!digitalRead(JoyA_YAxis)) stepperY.setSpeedAndLimitPositionOutOfRange(defaultSpeed);
	else if (!digitalRead(JoyB_YAxis)) stepperY.setSpeedAndLimitPositionOutOfRange(-defaultSpeed);
	else stepperY.setSpeedAndLimitPositionOutOfRange(0);
    // if (!digitalRead(JoyA_YAxis)) stepperZ.setSpeedAndLimitPositionOutOfRange(defaultSpeed);
	// else if (!digitalRead(JoyB_YAxis)) stepperZ.setSpeedAndLimitPositionOutOfRange(-defaultSpeed);
	// else stepperZ.setSpeedAndLimitPositionOutOfRange(0);

    // X
	if (!digitalRead(JoyC_XAxis))
	{
        stepperX1.setSpeedAndLimitPositionOutOfRange(defaultSpeed);
        stepperX2.setSpeedAndLimitPositionOutOfRange(-defaultSpeed);
	}
	else if (!digitalRead(JoyD_XAxis))
	{
        stepperX1.setSpeedAndLimitPositionOutOfRange(-defaultSpeed);
        stepperX2.setSpeedAndLimitPositionOutOfRange(defaultSpeed);
	}
    else
	{
        stepperX1.setSpeedAndLimitPositionOutOfRange(0);
        stepperX2.setSpeedAndLimitPositionOutOfRange(0);
	}

    // Keep
    if (!digitalRead(Joy_Keep)) gripperKeep();

    stepperX1.runSpeed();
    stepperX2.runSpeed();
    stepperY.runSpeed();
    stepperZ.runSpeed();

}

#endif