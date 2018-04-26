int sensorPin = 2;
float periyot;
float frekans;
//Motor pinleri
int in1 = 6;
int in2 = 7;
int ena = 9;
//Led Pini
int led = 8;
void setup() {
  // put your setup code here, to run once:
  pinMode(sensorPin, INPUT);
  pinMode(4, OUTPUT);
  Serial.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  //Motor kısmı
  analogWrite(ena, 150);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  //RPM Okuma
  int sensorDurum = digitalRead(sensorPin);
  periyot = (float)pulseIn(sensorPin, HIGH);
  frekans = 1000 / periyot;
  Serial.println("Frekans:");
  Serial.println(frekans);
  Serial.println("RPM:");
  Serial.println(frekans * 60);
  digitalWrite(led, HIGH);
  int per1 = (int)periyot;
  delay(per1);
  digitalWrite(led, LOW);
  delay(per1);
}
