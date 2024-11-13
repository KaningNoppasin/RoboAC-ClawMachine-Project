// #include <Arduino.h>
#include "./init_IO.h"
#include "./StepperController.ino"
#include "./JsonDocument.ino"
#include "./JoyController.ino"

String data;

unsigned long previousMillis;
bool receivedData = false;

/* ---------- SLECTOR SWITCH ---------- */
byte selectorSwitchA_Value = 0;
byte selectorSwitchB_Value = 0;
byte selectorSwitchC_Value = 0;


void getCurrentPosition(){
    Serial.print("X1 :");Serial.print(stepperX1.currentPosition());
    Serial.print("\t X2 :");Serial.print(stepperX2.currentPosition());
    Serial.print("\t Y :");Serial.print(stepperY.currentPosition());
    Serial.print("\t Z:");Serial.println(stepperZ.currentPosition());
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
}

void setup()
{
	Serial.begin(115200);
    Serial1.begin(115200);
    Serial.println("Hello from received");

	for (int i = 0; i < sizeof(pinIN) / sizeof(int); i++) pinMode(pinIN[i], INPUT_PULLUP);
    pinMode(Relay_Keep, OUTPUT);
    initStepper();

    setHome();
}

void loop()
{
/*
* Max Position
X1 2700 | X2 -2700
Y 2475
Z ok -1000 max(use)-2000
*/
// 690_690_
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
            Serial.println("plotXY_2 >>");
            while (true){
                if (Serial.available() > 0){
                    // plotXYZ();
                    plotXY_2();
                    Serial.println("done !");
                    break;
                }
            }
        }
    }
    if (SERIAL_JSON.available() > 0){
        StaticJsonDocument<300> doc;

        DeserializationError err = deserializeJson(doc, SERIAL_JSON);

        if (err == DeserializationError::Ok)
        {
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


}