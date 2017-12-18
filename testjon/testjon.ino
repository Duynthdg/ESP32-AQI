#include <ArduinoJson.h>
void setup() {
  Serial.begin(9600);
  Serial.println();
  
}


void loop() {
  char JSONMessage[] = " {\"SensorType\": \"Temperature\", \"Value\": 10}";
  StaticJsonBuffer<300> JSONBuffer;
  JsonObject& parsed = JSONBuffer.parseObject(JSONMessage);
  if (!parsed.success()) { //Check for errors in parsing
  Serial.println("Parsing failed");
  delay(5000);
  return;
  }
  const char * sensorType = parsed["SensorType"]; //Get sensor type value
int value = parsed["Value"]; //Get value of sensor measurement

Serial.print("Sensor type: ");
  Serial.println(sensorType);
  Serial.print("Sensor value: ");
  Serial.println(value);
 
  Serial.println();
  delay(5000);

}
