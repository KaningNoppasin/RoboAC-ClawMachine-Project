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
// TODO: Chage Y <=> Z
// StepperController stepperY(Mode, PUL_Z, DIR_Z);
// StepperController stepperZ(Mode, PUL_Y, DIR_Y);

// defaultSpeed = 1000 4step y,z Axis Ok with delay 1 ms

int limitPosition = 0;

double defaultSpeed = 500;
String data;

JsonDocument getPayload;
JsonDocument putPayload;

// JsonDocument getPayload;
// JsonDocument putPayload;

int positionX;
int positionY;

unsigned long previousMillis;
bool receivedData = false;

/* ---------- SLECTOR SWITCH ---------- */
byte selectorSwitchA_Value = 0;
byte selectorSwitchB_Value = 0;
byte selectorSwitchC_Value = 0;

void runSpeedAllStepper(){
	stepperX1.runSpeed();
	stepperX2.runSpeed();
	stepperY.runSpeed();
	stepperZ.runSpeed();
}


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

        runSpeedAllStepper();
    }
    stepperX1.setCurrentPosition(0);
    stepperX2.setCurrentPosition(0);
    stepperY.setCurrentPosition(0);
    stepperZ.setCurrentPosition(0);
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


}

void getInputValue()
{
    for (int i = 0; i < sizeof(pinIN) / sizeof(int) ; i++){
        Serial.print(" >>");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(digitalRead(pinIN[i]));
    }
    Serial.println();
}

void setup()
{
	Serial.begin(115200);
    Serial1.begin(115200);
    Serial.println("Hello from received");

	for (int i = 0; i < sizeof(pinIN) / sizeof(int); i++) pinMode(pinIN[i], INPUT_PULLUP);
    pinMode(Relay_Keep, OUTPUT);
    // x 2900
    // x 810 mm
    // y 720 mm
    stepperX1.setLimitPositivePosition(2700);stepperX1.setLimitNegativePosition(limitPosition);
    stepperX2.setLimitPositivePosition(limitPosition);stepperX2.setLimitNegativePosition(2700);
    stepperY.setLimitPositivePosition(limitPosition);stepperY.setLimitNegativePosition(2400);
    stepperZ.setLimitPositivePosition(limitPosition);stepperZ.setLimitNegativePosition(1900);

    stepperX1.setAcceleration(3000);
    stepperX2.setAcceleration(3000);
    stepperY.setAcceleration(3000);
    stepperZ.setAcceleration(3000);

    stepperX1.setMaxSpeed(5000);
    stepperX2.setMaxSpeed(5000);
    stepperY.setMaxSpeed(5000);
    stepperZ.setMaxSpeed(5000);

    setHome();
    delay(500);
    stepperZ.moveTo(-1900);
    while(stepperZ.run())delay(1);
}

void readSelectorSwitch(){
    // A
    if (!digitalRead(SLECTOR_SWITCH_A1)) selectorSwitchA_Value = 1;
    else if (!digitalRead(SLECTOR_SWITCH_A2)) selectorSwitchA_Value = 2;
    else if (!digitalRead(SLECTOR_SWITCH_A3)) selectorSwitchA_Value = 3;
    else selectorSwitchA_Value = 0;
    // B
    if (!digitalRead(SLECTOR_SWITCH_B1)) selectorSwitchB_Value = 1;
    else if (!digitalRead(SLECTOR_SWITCH_B2)) selectorSwitchB_Value = 2;
    else if (!digitalRead(SLECTOR_SWITCH_B3)) selectorSwitchB_Value = 3;
    else selectorSwitchB_Value = 0;
    // C
    if (!digitalRead(SLECTOR_SWITCH_C1)) selectorSwitchC_Value = 1;
    else if (!digitalRead(SLECTOR_SWITCH_C2)) selectorSwitchC_Value = 2;
    else if (!digitalRead(SLECTOR_SWITCH_C3)) selectorSwitchC_Value = 3;
    else selectorSwitchC_Value = 0;
    // Serial.print(selectorSwitchA_Value);
    // Serial.print(selectorSwitchB_Value);
    // Serial.println(selectorSwitchC_Value);
}

void Sender_serializeJson(String status)
{
    StaticJsonDocument<200> payload;
    payload["status"] = status;

    serializeJson(payload, Serial1);
}

void Receiver_deserializeJson(){
    if (Serial1.available())
    {
        StaticJsonDocument<300> response;

        DeserializationError err = deserializeJson(response, Serial1);

        if (err == DeserializationError::Ok)
        {
            Serial.print("x = ");
            Serial.println(response["x"].as<int>());
            Serial.print("y = ");
            Serial.println(response["y"].as<int>());
            Sender_serializeJson("processing");
            plotXYZ_withOutSerial(response["x"].as<int>(), response["y"].as<int>(), 0);
            Sender_serializeJson("done");
        }
        else
        {
            // Print error to the "debug" serial port
            Serial.print("deserializeJson() returned ");
            Serial.println(err.c_str());

            // Flush all bytes in the "link" serial port buffer
            while (Serial1.available() > 0)
                Serial1.read();
        }
    }
}

void loop()
{
/*
* Max Position
X1 2700 | X2 -2700
Y -2475
Z ok -1000 max(use)-2000

*/
    // digitalWrite(Relay_Keep,LOW);
    // readSelectorSwitch();
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
                    // plotXYZ();
                    plotXYZ_2();
                    break;
                }
            }
        }
    }
    if (Serial1.available() > 0){
        // Serial.println(Serial1.read());
        // deserializeJson(getPayload, Serial1);
        // previousMillis = millis();
        // receivedData = true;
        StaticJsonDocument<300> doc;

        DeserializationError err = deserializeJson(doc, Serial1);

        if (err == DeserializationError::Ok)
        {
            // Serial.print("x = ");
            // Serial.println(doc["x"].as<String>());
            // Serial.print("y = ");
            // Serial.println(doc["y"].as<String>());
            Serial.print("x = ");
            Serial.println(doc["x"].as<int>());
            Serial.print("y = ");
            Serial.println(doc["y"].as<int>());
            plotXYZ_withOutSerial(doc["x"].as<int>(), doc["y"].as<int>(), 0);
            putPayload["status"] = "DONE";
            serializeJson(putPayload, Serial1);
        }
        else
        {
            // Print error to the "debug" serial port
            Serial.print("deserializeJson() returned ");
            Serial.println(err.c_str());

            // Flush all bytes in the "link" serial port buffer
            while (Serial1.available() > 0)
                Serial1.read();
        }
    }

    delay(2);
    JoyController();
    runSpeedAllStepper();

}
