#include <WiFi.h>
#include <PubSubClient.h>
void nhandl(char * tp, byte * nd, unsigned int length)
{
  String topic(tp);
  String noidung= String((char*) nd);
  noidung.remove(length);

  Serial.println(topic);
  Serial.println(noidung);

  if(topic=="ESPn/RLA")
  {
    if(noidung=="1")
      digitalWrite(23,LOW);
    if(noidung=="0")
      digitalWrite(23,HIGH);
  }

  if(topic=="ESPn/RLB")
  {
    if(noidung=="1")
      digitalWrite(22,LOW);
    if(noidung=="0")
      digitalWrite(22,HIGH);
  }

  if(topic=="ESPn/RLC")
  {
    if(noidung=="1")
      digitalWrite(4,LOW);
    if(noidung=="0")
      digitalWrite(4,HIGH);
  }

  if(topic=="ESPn/RLD")
  {
    if(noidung=="1")
      digitalWrite(2,LOW);
    if(noidung=="0")
      digitalWrite(2,HIGH);
  }
}
WiFiClient c;
PubSubClient MQTT ("m14.cloudmqtt.com",13601,nhandl,c);
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
    delay(500);
    if(MQTT.connect("ESP32","qskswhuy","mljvJcf1cDY5"))
      break;
  }
  Serial.println("Da vao duoc MQTT");
  MQTT.publish("test", "gui du lieu thu");
  MQTT.subscribe("ESPn/RLA");
  MQTT.subscribe("ESPn/RLB");
  MQTT.subscribe("ESPn/RLC");
  MQTT.subscribe("ESPn/RLD");

  pinMode(23,OUTPUT);
  pinMode(22,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(2,OUTPUT);

  digitalWrite(23,HIGH);
  digitalWrite(22,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(2,HIGH);

}

void loop() {
  MQTT.loop();
  MQTT.publish("ESPg/RLA", String(!digitalRead(23)).c_str());
  MQTT.publish("ESPg/RLB", String(!digitalRead(22)).c_str());
  MQTT.publish("ESPg/RLC", String(!digitalRead(4)).c_str());
  MQTT.publish("ESPg/RLD", String(!digitalRead(2)).c_str());
}
