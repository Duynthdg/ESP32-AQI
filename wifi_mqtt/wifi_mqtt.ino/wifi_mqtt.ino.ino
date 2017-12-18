#include <WiFi.h>
#include <PubSubClient.h>


#define mqtt_server "m14.cloudmqtt.com"
const uint16_t mqtt_port = 13601;
const int ESP23LED = 23;
const int ESP22LED = 22;
const int  ESP4LED = 4;
 const int ESP2LED = 2;

void nhandl(char * tp, byte * nd ,unsigned int length )
{
  String topic(tp);
  String noidung= String((char *) nd);
  noidung.remove(length);
  Serial.println(topic);
    Serial.println(noidung);

 }
WiFiClient ESP32client;
PubSubClient MQTT (mqtt_server,mqtt_port,nhandl,ESP32client);
void setup() {
  Serial.begin(9600);
   Serial.println("Program is connnecting");
  WiFi.begin("Abc123","0904407094");
   while ((!(WiFi.status() == WL_CONNECTED))){
    delay(3000);
  }
  Serial.println("I m connected");
  Serial.println("Your IP : ");
  Serial.println((WiFi.localIP()));
  Serial.println((WiFi.gatewayIP().toString().c_str()));
  while (1)
  {
    delay(5000);
    if(MQTT.connect("ESP32","qskswhuy","mljvJcf1cDY5"))
    break;
  }
Serial.println("I m connected to MQTT broker");
MQTT.publish("test","gui du lieu thu");
MQTT.subscribe("ESPr/23");
MQTT.subscribe("ESPr/22");
MQTT.subscribe("ESPr/4");
MQTT.subscribe("ESPr/2");
pinMode (ESP23LED , OUTPUT);
pinMode (ESP22LED , OUTPUT);
pinMode (ESP4LED , OUTPUT);
pinMode (ESP2LED , OUTPUT);
digitalWrite(ESP23LED,HIGH);
digitalWrite(ESP22LED,HIGH);
digitalWrite(ESP4LED,HIGH);
digitalWrite(ESP2LED,HIGH);
}

void loop() {
 MQTT.loop();
MQTT.publish("ESPs/23",String(digitalRead(23)).c_str());
  //MQTT.publish("ESPs/22",String(digitalRead(22)).c_str());
   //MQTT.publish("ESPs/4",String(digitalRead(4)).c_str());
    //MQTT.publish("ESPs/2",String(digitalRead(2)).c_str());

}
