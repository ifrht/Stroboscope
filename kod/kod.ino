int encoder_pin = 2;
int led = 8;
int motor1 = 6;
int motor2 = 7;
int motorpwm = 9;
int sayac = 0;
int rpm;
unsigned long ilkzaman;
unsigned long sonzaman;
double periyot = 0;
unsigned long yaz = 0;

void counter()
{
  periyot = millis() - ilkzaman;
  digitalWrite(led, HIGH);
  delayMicroseconds(100);
  digitalWrite(led, LOW);
  ilkzaman = millis();
}

void setup()
{
  Serial.begin(9600);
  pinMode(encoder_pin, INPUT);
  pinMode(led, OUTPUT);
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(motorpwm, OUTPUT);

  analogWrite(motorpwm, 180);
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2, LOW);
  attachInterrupt(0, counter, FALLING);
  ilkzaman = 0;

}
void loop()
{

//int sensorDurum = digitalRead(encoder_pin);
//periyot = (float)pulseIn(encoder_pin, LOW);
//  if ((millis() - yaz) > 1000){
//    detachInterrupt(0);
//    Serial.print("RPM= ");
//    Serial.println(periyot);
//    yaz = millis();
//    attachInterrupt(0, counter, FALLING);
//    }
}
