#ifndef JSON_FILE
#define JSON_FILE

#include <ArduinoJson.h>
#include "./StepperController.ino"

#define SERIAL_JSON Serial1

JsonDocument getPayload;
JsonDocument putPayload;

void Sender_serializeJson(String status)
{
    StaticJsonDocument<200> payload;
    payload["status"] = status;

    serializeJson(payload, Serial1);
}

void Receiver_deserializeJson(){
    if (SERIAL_JSON.available())
    {
        StaticJsonDocument<300> response;

        DeserializationError err = deserializeJson(response, SERIAL_JSON);

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
            while (SERIAL_JSON.available() > 0)
                SERIAL_JSON.read();
        }
    }
}

#endif