#include <avr/interrupt.h>

//#define T_MAX 600 //3s
#define T_MAX 30
#define msg_length 100
#define R0 20.0
#define RL 10.0
#define OFFSET  5
#define COV_RATIO 0.2            //ug/mmm / mv
#define NO_DUST_VOLTAGE 400          //mv
#define SYS_VOLTAGE 5000

#define CO_SENSOR_PIN A0
#define PM_SENSOR_PIN A1
const int iled = 2;                                            //pin 2 ket noi voi led cua sensor
const int vout = 1;                                            //analog input
unsigned int timeCount = 0;
String data = "{\"data\":{\"co\":\"v_co\",\"pm\":\"v_pm\"}}\r\n";
char msg[msg_length];
String vCoKey = "v_co";
String vPmKey = "v_pm";
String vLkKey = "v_k";
String vLvKey = "v_v";

void setupTimer() {
  cli();
  /* Reset Timer/Counter1 */
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;
  /* Setup Timer/Counter1 */
  TCCR1B |= (1 << CS11) | (1 << CS10);    
  TCNT1 = 40536;
  TIMSK1 = (1 << TOIE1);                  
  sei();
}

void sendData() {
  Serial.println("Send data...");
  Serial.println(data);
  data.toCharArray(msg, msg_length);
  Serial1.write(msg);
}

void resetData() {
  data = "{\"data\":{\"co\":\"v_co\",\"pm\":\"v_pm\"}}\r\n";
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
  data.replace(vCoKey, vCoStr);
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
  int adcvalue = analogRead(A1);
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
  data.replace(vPmKey, vPmStr);
}

void calcLocation() {

}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
//  delay(1000);
//  Serial.println("3");
//  delay(1000);
//  Serial.println("2");
//  delay(1000);
//  Serial.println("1");
//  delay(1000);
  Serial.println("Power On");
  setupTimer();
}

void loop() {
  while(Serial1.available() > 0)
  {
    Serial.write(Serial1.read());
  }
}

ISR (TIMER1_OVF_vect)
{
  if (timeCount < T_MAX) {
    timeCount++;
  } else {
    timeCount = 0;
    calcCo();
    calcPm();
    sendData();
    resetData();
  }

  TCNT1 = 40536;
}





