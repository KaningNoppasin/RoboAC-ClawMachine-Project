#include <Arduino.h>
#include <ArduinoJson.h>

// const char* json = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
String a = "{\"x\":-500,\"y\":\"300\"}";
const char* json = "{\"x\":-500,\"y\":\"300\"}";

int positionX;
int positionY;

JsonDocument doc;


void setup()
{
    Serial.begin(115200);
    deserializeJson(doc, json);

    Serial.println(a.c_str());
    Serial.println(json);

    // const char* sensor = doc["sensor"];
    // long time          = doc["time"];
    // double latitude    = doc["data"][0];
    // double longitude   = doc["data"][1];
    positionX = doc["x"];
    positionY = doc["y"];

    Serial.print("positionX :");Serial.println(positionX);
    Serial.print("positionY :");Serial.println(positionY);
    // Serial.println(sensor);
    // Serial.println(time);
    // Serial.println(latitude);
    // Serial.println(longitude);


}

void loop()
{
    // Your code here
}
