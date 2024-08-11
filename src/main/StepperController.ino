#ifndef STEPPER_FILE
#define STEPPER_FILE

#include <AccelStepper.h>
#include "StepperController.h"
#include "./init_IO.h"

/*

*    -------
*    |  Z  Y
*    |  |  |
*    X1---X2

*/

#define Mode 1

StepperController stepperX1(Mode, PUL_X1, DIR_X1);
StepperController stepperX2(Mode, PUL_X2, DIR_X2);
StepperController stepperY(Mode, PUL_Y, DIR_Y);
StepperController stepperZ(Mode, PUL_Z, DIR_Z);

int limitPosition = 0;
double defaultSpeed = 500;
const float ACCELERATION = 3000;
const float MAX_SPEED = 5000;
// defaultSpeed = 1000 4step y,z Axis Ok with delay 1 ms


void setHome(){
    while (
        digitalRead(Limit_XAxis) ||
        digitalRead(Limit_YAxis) ||
        digitalRead(Limit_ZAxis)
    ){
        delay(1);
        // BACKWARD
        stepperX1.setSpeed(digitalRead(Limit_XAxis) ? -defaultSpeed : 0);
        stepperX2.setSpeed(digitalRead(Limit_XAxis) ? defaultSpeed : 0);
        // RIGHT
        stepperY.setSpeed(digitalRead(Limit_YAxis) ? defaultSpeed : 0);
        // DOWN
        stepperZ.setSpeed(digitalRead(Limit_ZAxis) ? defaultSpeed : 0);

        stepperX1.runSpeed();
        stepperX2.runSpeed();
        stepperY.runSpeed();
        stepperZ.runSpeed();
    }
    stepperX1.setCurrentPosition(0);
    stepperX2.setCurrentPosition(0);
    stepperY.setCurrentPosition(0);
    stepperZ.setCurrentPosition(0);
    delay(500);
    stepperZ.moveTo(-1900);
    while(stepperZ.run())delay(1);
}

void plotXYZ(){
    String data = Serial.readStringUntil('_');
    // int positionX = data.toFloat() / (139.2 / 494);
    int positionX = data.toFloat() * 100 / 30;

    data = Serial.readStringUntil('_');
    // int positionY = data.toFloat() / (139.2 / 494);
    int positionY = data.toFloat() * 100 / 30;

    data = Serial.readStringUntil('_');
    // int positionZ = data.toFloat() / (139.2 / 494);
    int positionZ = data.toFloat() * 100 / 30;

    // position 100 = 3cm

    Serial.print("positionX: ");Serial.print(positionX);
    Serial.print("positionY: ");Serial.print(positionY);
    Serial.print("positionZ: ");Serial.println(positionZ);

    positionX = positionX > stepperX1.getLimitNegativePosition() ? stepperX1.getLimitNegativePosition() : positionX;
    positionY = positionY > stepperY.getLimitPositivePosition() ? stepperY.getLimitPositivePosition() : positionY;
    positionZ = positionZ > stepperZ.getLimitPositivePosition() ? stepperZ.getLimitPositivePosition() : positionZ;

    // Up
    stepperZ.moveTo(stepperZ.getLimitPositivePosition());
    while (stepperZ.run());

    stepperX1.moveTo(-positionX);
    stepperX2.moveTo(positionX);
    stepperY.moveTo(positionY);
    // Down
    stepperZ.moveTo(positionZ);

    // while (stepperX1.run() || stepperX2.run() || stepperY.run());
    while (
        stepperX1.currentPosition() != -positionX ||
        stepperX2.currentPosition() != positionX ||
        stepperY.currentPosition() != positionY
    ){
        // Serial.print("While <=");
        // Serial.print("positionX: ");Serial.print(positionX);
        // Serial.print("  positionY: ");Serial.print(positionY);
        // Serial.print("  positionZ: ");Serial.print(positionZ);
        // getCurrentPosition();
        stepperX1.run();
        stepperX2.run();
        stepperY.run();
    }
    // while (stepperY.run());
    while (stepperZ.run());

}

void plotXYZ_2(){
    String data = Serial.readStringUntil('_');
    // int positionX = data.toFloat() / (139.2 / 494);
    int positionX = data.toFloat() * 100 / 30;

    data = Serial.readStringUntil('_');
    // int positionY = data.toFloat() / (139.2 / 494);
    int positionY = data.toFloat() * 100 / 30;

    data = Serial.readStringUntil('_');
    // int positionZ = data.toFloat() / (139.2 / 494);
    int positionZ = data.toFloat() * 100 / 30;

    // position 100 = 3cm

    positionX = positionX > stepperX1.getLimitPositivePosition() ? stepperX1.getLimitPositivePosition() : positionX;
    positionY = positionY > stepperY.getLimitNegativePosition() ? stepperY.getLimitNegativePosition() : positionY;
    positionZ = positionZ > stepperZ.getLimitNegativePosition() ? stepperZ.getLimitNegativePosition() : positionZ;

    // Up
    stepperZ.moveTo(-stepperZ.getLimitNegativePosition());
    while (stepperZ.run())delay(1);

    stepperX1.moveTo(positionX);
    stepperX2.moveTo(-positionX);
    stepperY.moveTo(-positionY);
    // Down
    stepperZ.moveTo(-positionZ);

    while (
        stepperX1.currentPosition() != -positionX ||
        stepperX2.currentPosition() != positionX ||
        stepperY.currentPosition() != positionY
    ){
        stepperX1.run();
        stepperX2.run();
        stepperY.run();
        delay(1);
    }
    while (stepperZ.run())delay(1);

}

void plotXYZ_withOutSerial(int positionX, int positionY, int positionZ){
    positionX *= 100 / 30;
    positionY *= 100 / 30;
    positionZ *= 100 / 30;

    positionX = positionX > stepperX1.getLimitNegativePosition() ? stepperX1.getLimitNegativePosition() : positionX;
    positionY = positionY > stepperY.getLimitPositivePosition() ? stepperY.getLimitPositivePosition() : positionY;
    positionZ = positionZ > stepperZ.getLimitPositivePosition() ? stepperZ.getLimitPositivePosition() : positionZ;

    // UP
    stepperZ.moveTo(stepperZ.getLimitPositivePosition());
    while (stepperZ.run());

    stepperX1.moveTo(-positionX);
    stepperX2.moveTo(positionX);
    stepperY.moveTo(positionY);
    // Down
    stepperZ.moveTo(positionZ);

    while (
        stepperX1.currentPosition() != -positionX ||
        stepperX2.currentPosition() != positionX ||
        stepperY.currentPosition() != positionY
    ){
        stepperX1.run();
        stepperX2.run();
        stepperY.run();
    }
    while (stepperZ.run());

}

void initStepper(){
    stepperX1.setLimitPositivePosition(2700);stepperX1.setLimitNegativePosition(limitPosition);
    stepperX2.setLimitPositivePosition(limitPosition);stepperX2.setLimitNegativePosition(2700);
    stepperY.setLimitPositivePosition(limitPosition);stepperY.setLimitNegativePosition(2400);
    stepperZ.setLimitPositivePosition(limitPosition);stepperZ.setLimitNegativePosition(1900);

    stepperX1.setAcceleration(ACCELERATION);
    stepperX2.setAcceleration(ACCELERATION);
    stepperY.setAcceleration(ACCELERATION);
    stepperZ.setAcceleration(ACCELERATION);

    stepperX1.setMaxSpeed(MAX_SPEED);
    stepperX2.setMaxSpeed(MAX_SPEED);
    stepperY.setMaxSpeed(MAX_SPEED);
    stepperZ.setMaxSpeed(MAX_SPEED);
}

#endif