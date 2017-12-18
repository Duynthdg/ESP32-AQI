#include <WiFi.h>
#include <PubSubClient.h>

#define ssid "Abc123"
#define password "0904407094"
//#define mqtt_server "35.185.154.250"
#define mqtt_server "192.168.1.13"
#define mqtt_topic_pub "aqi/data"
#define mqtt_user ""
#define mqtt_pwd ""
#define msg_length 100
#define COV_RATIO 0.2 
#define CO_SENSOR_PIN 23
#define PM_SENSOR_PIN 22
#define R0 20.0
#define RL 10.0
#define SYS_VOLTAGE 5000
#define NO_DUST_VOLTAGE 400 
char buf[100];
const uint16_t mqtt_port = 1883;
String data = "{\"data\":{\"co\":\"v_co\",\"pm\":\"v_pm\"}}";
char msg[msg_length];
String vCoKey = "v_co";
String vPmKey = "v_pm";
String vLkKey = "v_k";
String vLvKey = "v_v";
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
    setup_wifi();
    MQTT.setServer(mqtt_server, mqtt_port);
    Serial.println("Conenected MQTT Broker: ");
    Serial.print(mqtt_server);
    Serial.print("--- Port: ");
    Serial.print(mqtt_port);
}
void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting .... to SSID: ");
    Serial.print(ssid);
    Serial.println();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.print(WiFi.localIP());
    Serial.println("\n-----------Setup finish--------\n");
}
void reconnect() {
    // Lap cho den khi ket noi lai
    while (!MQTT.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (MQTT.connect("ESP32Client", mqtt_user, mqtt_pwd)) {
            Serial.println("connected");
            MQTT.publish("test","gui du lieu thu");
        } else {
            Serial.print("failed, rc=");
            Serial.print(MQTT.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}
void sendData() {
  delay(2000);
  Serial.println("Send data...");
  Serial.println(data);
  data.toCharArray(msg, msg_length);
  MQTT.publish(mqtt_topic_pub, msg);
  
}
void resetData() {
  data = "{\"data\":{\"co\":\"v_co\",\"pm\":\"v_pm\"}}";
  for( int i = 0; i < sizeof(msg);  ++i )
    msg[i] = (char)0;
   Serial.print("\n");
}
void calcCo() {
  int sensorValue = analogRead(CO_SENSOR_PIN);
  float ppm, t;
  unsigned int PPM;
  float voltage = sensorValue * (5.0 / 1023.0);
  t = RL / R0 * (5.0 - voltage) / voltage;
  t = (1.590868067 - log10(t)) / 0.821067123;
  ppm = pow(10, t);
  PPM = ppm;
  
//  if (PPM < OFFSET) PPM = 0;
//  else PPM = PPM - OFFSET;
  int co = ppm * 1000;
  Serial.print("CO: ");
  Serial.print(co);
  Serial.print(" ug/m3 \n");

  String vCoStr(co);
  data.replace(vCoKey, "600");
}
int Filter(int m)
{
  static int flag_first = 0, _buff[10], sum;
  const int _buff_max = 10;
  int i;
  if (flag_first == 0) {
    flag_first = 1;

    for (i = 0, sum = 0; i < _buff_max; i++) {
      _buff[i] = m;
      sum += _buff[i];
    }
    return m;
  } else {
    sum -= _buff[0];
    for (i = 0; i < (_buff_max - 1); i++)
    {
      _buff[i] = _buff[i + 1];
    }
    _buff[9] = m;
    sum += _buff[9];
    i = sum / 10.0;
    return i;
  }
}

void calcPm() {

  float density;
  int adcvalue = analogRead(PM_SENSOR_PIN);
  adcvalue = Filter(adcvalue);
  float voltage = (SYS_VOLTAGE / 1024.0) * adcvalue * 11;
//  chuyen tu dien ap sang do bui
  if (voltage >= NO_DUST_VOLTAGE) {
    voltage -= NO_DUST_VOLTAGE;
    density = voltage * COV_RATIO;
  } else {
    density = 0;
  }
  int dust = (unsigned int)density;
  Serial.print("Dust pm2.5 : ");
  Serial.print(dust);
  Serial.print(" ug/m3\n");

  String vPmStr(dust);
  data.replace(vPmKey, "600");
}
void loop() {
    if (!MQTT.connected()) {
        reconnect();
    }
    MQTT.loop();
    delay(1000);
    calcCo();
    Serial.print(data);
    Serial.print("\n");
    delay(2000);
    calcPm();
    Serial.print(data);
    Serial.print("\n");
     delay(3000);
    sendData();
    Serial.print(data);
    Serial.print("\n");
    delay(4000);
    resetData();
    Serial.print(data);
    Serial.print("\n");
    delay(1000);
 
 
}








