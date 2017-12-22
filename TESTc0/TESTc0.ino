#define CO_SENSOR_PIN 4
#define COV_RATIO 0.2 
#define R0 20.0
#define RL 10.0
void setup() {
    Serial.begin(9600);
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
  Serial.print(sensorValue);
  Serial.print("\n");
  Serial.print("CO: ");
  Serial.print(co);
  Serial.print(" ug/m3 \n");
}

void loop() {
  
    calcCo();
    delay(2000);
  
}
