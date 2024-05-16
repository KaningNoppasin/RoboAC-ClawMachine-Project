// raspberry pi
#include <ArduinoJson.h>

JsonDocument position;
JsonDocument Response;
unsigned long previousMillis;
bool receivedData = false;

void Sender_serializeJson(int xPosition,int yPosition)
{
    StaticJsonDocument<200> payload;
    payload["x"] = xPosition;
    payload["y"] = yPosition;

    serializeJson(payload, Serial1);
}

void Receiver_deserializeJson(){
    if (Serial1.available())
    {
        StaticJsonDocument<300> response;

        DeserializationError err = deserializeJson(response, Serial1);

        if (err == DeserializationError::Ok)
        {
            Serial.print("status = ");
            Serial.println(response["status"].as<String>());
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

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello from transmission");

    // position["x"] = 0;
    // position["y"] = 0;

    Serial1.begin(115200);
    // serializeJson(position, Serial1);
}

void loop()
{
    // Send Data
    if (Serial.available() > 0){
        int x = Serial.readStringUntil('_').toInt();
        int y = Serial.readStringUntil('_').toInt();
        Sender_serializeJson(x, y);
        // position["x"] = x;
        // position["y"] = y;
        // serializeJson(position, Serial1);

        Serial.print("x: ");Serial.print(x);
        Serial.print("y: ");Serial.println(y);
    }

    if (millis() - previousMillis > 1000){
        Receiver_deserializeJson();
        previousMillis = millis();
    }



    // Call back Response
    // if (Serial1.available() > 0)
    // {
    //     deserializeJson(Response, Serial1);
    //     previousMillis = millis();
    //     receivedData = true;
    // }
    // if (millis() - previousMillis > 200 && receivedData)
    // {
    //     String status = Response["status"];
    //     previousMillis = millis();
    //     receivedData = false;
    //     Serial.print("status :");
    //     Serial.println(status);
    // }
}
