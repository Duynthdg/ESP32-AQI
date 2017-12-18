#include <WiFi.h>
const char*ssid ="Abc123";
const char*password ="09044070";
void setup()
{
Serial.begin(9600);
scanNetworks();
connectToNetwork();
Serial.println(WiFi.macAddress());
Serial.println(WiFi.localIP());
WiFi.disconnect(true);
Serial.println(WiFi.localIP());
}
void loop() 
{

}
