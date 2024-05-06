// #include <Arduino.h>
#include <AccelStepper.h>
#include <ArduinoJson.h>
#include "./init_IO.h"
#include "StepperController.h"

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

// defaultSpeed = 1000 4step y,z Axis Ok with delay 1 ms


double defaultSpeed = 500;
String data;

JsonDocument getPayload;
JsonDocument putPayload;

int positionX;
int positionY;

unsigned long previousMillis;
bool receivedData = false;

float getTimeFormPosition(float position){
    return position / (139.2 / 500) / 500;
}

void setSpeedAllStepper(double speedX1, double speedX2, double speedY, double speedZ){
        stepperX1.setSpeedAndLimitPositionOutOfRange(speedX1);
        stepperX2.setSpeedAndLimitPositionOutOfRange(speedX2);
        stepperY.setSpeedAndLimitPositionOutOfRange(speedY);
        stepperZ.setSpeedAndLimitPositionOutOfRange(speedZ);
}

void runSpeedAllStepper(){
	stepperX1.runSpeed();
	stepperX2.runSpeed();
	stepperY.runSpeed();
	stepperZ.runSpeed();
}

void setHome(){
    if (stepperX1.currentPosition() > 0) stepperX1.setSpeed(-defaultSpeed);
    else if (stepperX1.currentPosition() < 0) stepperX1.setSpeed(defaultSpeed);

    if (stepperX2.currentPosition() > 0) stepperX2.setSpeed(-defaultSpeed);
    else if (stepperX2.currentPosition() < 0) stepperX2.setSpeed(defaultSpeed);

    if (stepperY.currentPosition() > 0) stepperY.setSpeed(-defaultSpeed);
    else if (stepperY.currentPosition() < 0) stepperY.setSpeed(defaultSpeed);

    if (stepperZ.currentPosition() > 0) stepperZ.setSpeed(-defaultSpeed);
    else if (stepperZ.currentPosition() < 0) stepperZ.setSpeed(defaultSpeed);

    while (stepperX1.currentPosition() != 0 || stepperX2.currentPosition() != 0 || stepperY.currentPosition() != 0 || stepperZ.currentPosition() != 0){
        if (stepperX1.currentPosition() == 0) stepperX1.setSpeed(0);
        if (stepperX2.currentPosition() == 0) stepperX2.setSpeed(0);
        if (stepperY.currentPosition() == 0) stepperY.setSpeed(0);
        if (stepperZ.currentPosition() == 0) stepperZ.setSpeed(0);
        delay(1);
        runSpeedAllStepper();
    }
    setSpeedAllStepper(0, 0, 0, 0);
}

void getCurrentPosition(){
    Serial.print("X1 :");Serial.print(stepperX1.currentPosition());
    Serial.print("\t X2 :");Serial.print(stepperX2.currentPosition());
    Serial.print("\t Y :");Serial.print(stepperY.currentPosition());
    Serial.print("\t Z:");Serial.println(stepperZ.currentPosition());
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


void moveWithTime(){
    // float time = getTimeFormPosition(position);
    float time = data.toInt();
    unsigned long previousMillis = millis();
    while (millis() - previousMillis < time * 1000){
        // 245 => x | 255 => z | 255=> y 2sec * mm
        //  139.2 mm => x position => 494 time 1s
        //  speed 500 pulse / s
        stepperX1.setSpeedAndLimitPositionOutOfRange(-defaultSpeed);
        stepperX2.setSpeedAndLimitPositionOutOfRange(defaultSpeed);
        stepperY.setSpeedAndLimitPositionOutOfRange(defaultSpeed);
        stepperZ.setSpeedAndLimitPositionOutOfRange(defaultSpeed);
        runSpeedAllStepper();
        if (Serial.available() > 0){
            String data = Serial.readStringUntil('\n');
            if (data == "Stop") break;
        }
    }
}

void JoyController(){
    if (!digitalRead(JoyC_XAxis)) stepperY.setSpeedAndLimitPositionOutOfRange(defaultSpeed);
	else if (!digitalRead(JoyD_XAxis)) stepperY.setSpeedAndLimitPositionOutOfRange(-defaultSpeed);
	else stepperY.setSpeedAndLimitPositionOutOfRange(0);

	if (!digitalRead(JoyA_YAxis))
	{
        stepperX1.setSpeedAndLimitPositionOutOfRange(-defaultSpeed);
        stepperX2.setSpeedAndLimitPositionOutOfRange(defaultSpeed);
	}
	else if (!digitalRead(JoyB_YAxis))
	{
        stepperX1.setSpeedAndLimitPositionOutOfRange(defaultSpeed);
        stepperX2.setSpeedAndLimitPositionOutOfRange(-defaultSpeed);
	}
    else
	{
        stepperX1.setSpeedAndLimitPositionOutOfRange(0);
        stepperX2.setSpeedAndLimitPositionOutOfRange(0);
	}

	if (!digitalRead(KeepA_ZAxis)) stepperZ.setSpeedAndLimitPositionOutOfRange(defaultSpeed);
	else if (!digitalRead(KeepB_ZAxis)) stepperZ.setSpeedAndLimitPositionOutOfRange(-defaultSpeed);
	else stepperZ.setSpeedAndLimitPositionOutOfRange(0);

}

void getInputValue()
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

void setup()
{
	Serial.begin(115200);
    Serial1.begin(115200);
    Serial.println("Hello from received");

	for (int i = 0; i < 6; i++) pinMode(pinIN[i], INPUT_PULLUP);
    stepperX1.setLimitPositivePosition(0);stepperX1.setLimitNegativePosition(2900);
    stepperX2.setLimitPositivePosition(2900);stepperX2.setLimitNegativePosition(0);
    stepperY.setLimitPositivePosition(2500);stepperY.setLimitNegativePosition(0);
    stepperZ.setLimitPositivePosition(1900);stepperZ.setLimitNegativePosition(0);

}

void loop()
{
    // getCurrentPosition();
    // getInputValue();
    if (Serial.available() > 0){
        String data = Serial.readStringUntil('\n');
        Serial.println(data);
        if (data == "Home"){
            setHome();
        }
        else{
            while (true){
                if (Serial.available() > 0){
                    plotXYZ();
                    break;
                }
            }
        }
    }
    if (Serial1.available() > 0){
        deserializeJson(getPayload, Serial1);
        previousMillis = millis();
        receivedData = true;
    }
    if (millis() - previousMillis > 100 && receivedData){
        positionX = getPayload["x"];
        positionY = getPayload["y"];
        previousMillis = millis();
        receivedData = false;
        Serial.print("positionX :");Serial.println(positionX);
        Serial.print("positionY :");Serial.println(positionY);
        putPayload["status"] = "processing";
        serializeJson(putPayload, Serial1);
        plotXYZ_withOutSerial(positionX, positionY, 0);
        Serial.print("do something :");
        // delay(5000);
        putPayload["status"] = "done";
        serializeJson(putPayload, Serial1);
    }
    delay(2);
    JoyController();
    runSpeedAllStepper();

}
