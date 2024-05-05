#include <Arduino.h>
#include "StepperController.h"

// int StepperController::_limitPosition = 1000;

StepperController::StepperController(uint8_t interface, uint8_t pin1, uint8_t pin2) : AccelStepper(interface, pin1, pin2) {
    this->_limitPositivePosition = 10000;
    this->_limitNegativePosition = 10000;
    this->_defaultSpeed = 1000;
    setMaxSpeed(10000);
    setAcceleration(500);
}

void StepperController::setSpeedAndLimitPositionOutOfRange(float speedx){
    // if (this->currentPosition() >= this->_limitPosition && this->speed() > 0) this->setSpeed(0);
    // else if (this->currentPosition() <= -this->_limitPosition && this->speed() < 0) this->setSpeed(0);
    if (this->currentPosition() >= this->_limitPositivePosition && speedx > 0) this->setSpeed(0);
    else if (this->currentPosition() <= -this->_limitNegativePosition && speedx < 0) this->setSpeed(0);
    else this->setSpeed(speedx);
}

void StepperController::setLimitPositivePosition(unsigned int limitPositivePosition) {
    this->_limitPositivePosition = limitPositivePosition;
}

unsigned int StepperController::getLimitPositivePosition(){
    return this->_limitPositivePosition;
}

void StepperController::setLimitNegativePosition(unsigned int limitNegativePosition) {
    this->_limitNegativePosition = limitNegativePosition;
}

unsigned int StepperController::getLimitNegativePosition(){
    return this->_limitNegativePosition;
}

void StepperController::setDefaultSpeed(float defaultSpeed) {
    this->_defaultSpeed = defaultSpeed;
}

unsigned int StepperController::getDefaultSpeed(){
    return this->_defaultSpeed;
}

void StepperController::setHome(){
    if (this->currentPosition() > 0) this->setSpeed(-this->_defaultSpeed);
    else if (this->currentPosition() < 0) this->setSpeed(this->_defaultSpeed);

    while (this->currentPosition() != 0){
        // The delay statements introduced within the loop provide pauses to ensure that the motor has time to reach the desired position.
        delay(1);
        this->runSpeed();
    }

    this->setSpeed(0);
}