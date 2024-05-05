// raspberry pi
#include <ArduinoJson.h>

// const char* json = "{\"x\":\"-500\",\"y\":\"300\"}";
JsonDocument doc;
// StaticJsonDocument<512> doc;
JsonDocument getPayload;
unsigned long previousMillis;
bool receivedData = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello from transmission");

  doc["x"] = 0;
  doc["y"] = 0;

  Serial1.begin(115200);
  // Serial1.println(json);
  serializeJson(doc, Serial1);
}

void loop() {
  // Serial.println("loop");
  if (Serial1.available() > 0){
    deserializeJson(getPayload, Serial1);
    previousMillis = millis();
    receivedData = true;
  }
  if (millis() - previousMillis > 200 && receivedData){
    String status = getPayload["status"];
    previousMillis = millis();
    receivedData = false;
    Serial.print("status :");Serial.println(status);
  }
}
