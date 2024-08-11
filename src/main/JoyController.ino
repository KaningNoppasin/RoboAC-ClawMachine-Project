#ifndef JOY_FILE
#define JOY_FILE


#include "./init_IO.h"
#include "./StepperController.ino"


void gripperKeep(){
    // DOWN
    stepperZ.moveTo(-1000);
    while(stepperZ.run())delay(1);
    // Serial.println("goToTargetPosition -1000");
    // stepperZ.goToTargetPosition(-1000);

    // KEEP
    digitalWrite(Relay_Keep, HIGH);
    delay(1000);

    // UP to Show
    stepperZ.moveTo(-1900);
    while(stepperZ.run())delay(1);
    // Serial.println("goToTargetPosition -1900");
    // stepperZ.goToTargetPosition(-1900);
    delay(1000);

    // Release
    digitalWrite(Relay_Keep, LOW);
}

void JoyController(){
    // Y
    if (!digitalRead(JoyA_YAxis)) stepperY.setSpeedAndLimitPositionOutOfRange(-defaultSpeed);
	else if (!digitalRead(JoyB_YAxis)) stepperY.setSpeedAndLimitPositionOutOfRange(defaultSpeed);
	else stepperY.setSpeedAndLimitPositionOutOfRange(0);

    // X
	if (!digitalRead(JoyC_XAxis))
	{
        stepperX1.setSpeedAndLimitPositionOutOfRange(defaultSpeed);
        stepperX2.setSpeedAndLimitPositionOutOfRange(defaultSpeed);
	}
	else if (!digitalRead(JoyD_XAxis))
	{
        stepperX1.setSpeedAndLimitPositionOutOfRange(-defaultSpeed);
        stepperX2.setSpeedAndLimitPositionOutOfRange(-defaultSpeed);
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