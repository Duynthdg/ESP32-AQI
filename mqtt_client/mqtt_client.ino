#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define ssid "NCT"
#define password "dccndccn"
//#define mqtt_server "35.185.154.250"
#define mqtt_server "192.168.1.125"

#define mqtt_topic_pub "aqi/data"
#define mqtt_user ""
#define mqtt_pwd ""
#define msg_length 100
const uint16_t mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
char msg[msg_length];
int l = 0;

void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
	  Serial.println("Conenected MQTT Broker: ");
	  Serial.print(mqtt_server);
	  Serial.print("--- Port: ");
	  Serial.print(mqtt_port);
    client.setCallback(callback);
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

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void reconnect() {
    // Lap cho den khi ket noi lai
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP8266Client", mqtt_user, mqtt_pwd)) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void reciverChar(char inChar) {
    switch (inChar) {
        case '\r':
            break;
        case '\n':
            msg[l] = 0;
            client.publish(mqtt_topic_pub, msg);
            l = 0;
            break;
        default:
            msg[l] = inChar;
            l++;
    }
}

void sendData() {
    char inChar;
    // Doc du lieu Serial
    while (Serial.available() > 0) {
        inChar = Serial.read();
        reciverChar(inChar);
    }
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    sendData();
}








