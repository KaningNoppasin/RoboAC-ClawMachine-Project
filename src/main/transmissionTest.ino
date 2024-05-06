// raspberry pi
#include <ArduinoJson.h>

JsonDocument position;
JsonDocument Response;
unsigned long previousMillis;
bool receivedData = false;

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
        position["x"] = x;
        position["y"] = y;
        serializeJson(position, Serial1);
        Serial.print("x: ");Serial.print(x);
        Serial.print("y: ");Serial.println(y);
    }

    // Call back Response
    if (Serial1.available() > 0)
    {
        deserializeJson(Response, Serial1);
        previousMillis = millis();
        receivedData = true;
    }
    if (millis() - previousMillis > 200 && receivedData)
    {
        String status = Response["status"];
        previousMillis = millis();
        receivedData = false;
        Serial.print("status :");
        Serial.println(status);
    }
}
