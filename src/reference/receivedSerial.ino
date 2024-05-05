// Arduino mega
#include <ArduinoJson.h>

JsonDocument getPayload;
JsonDocument putPayload;

int positionX;
int positionY;

unsigned long previousMillis;
bool receivedData = false;
void setup() {
  Serial.begin(115200);
  Serial.println("Hello from received");
  Serial1.begin(115200);

}

void loop() {
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
    Serial.print("do something :");
    delay(5000);
    putPayload["status"] = "done";
    serializeJson(putPayload, Serial1);
  }

}