#include <WiFi.h>

void setup()
{
  Serial.begin(9600);

    delay(300);
  Serial.println("=======START======");
  WiFi.softAP("ESP Network");
  Serial.println("the Ip off access point : ");
  Serial.println((WiFi.softAPIP()));

}


void loop()
{

    Serial.println("the number of connectes station");
    Serial.println((WiFi.softAPgetStationNum()));
    delay(3000);

}
