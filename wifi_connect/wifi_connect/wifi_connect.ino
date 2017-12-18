#include <WiFi.h>

void setup()
{
  Serial.begin(9600);
  delay(1000)
    Serial.println("Program is connnecting");
  WiFi.begin("Abc123","0904407094");
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(3000);
  }
  Serial.println("I m connected");
  Serial.println("Your IP : ");
  Serial.println((WiFi.localIP()));
  Serial.println((WiFi.gatewayIP().toString().c_str()));

}


void loop()
{


}
