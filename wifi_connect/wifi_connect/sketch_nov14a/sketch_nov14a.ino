#include <WiFi.h>

void setup()
{
  Serial.begin(9600);

    Serial.println("Program is connnecting");
  WiFi.begin("NCT","dccndccn");
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);

  }
  Serial.println("I m connected");
  Serial.println("Your IP : ");
  Serial.println((WiFi.localIP()));
  Serial.println((WiFi.gatewayIP().toString().c_str()));

}


void loop()
{


}
