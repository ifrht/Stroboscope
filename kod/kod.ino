int sensor_pin = 2;
int sensor_durum;
int pwr_led = 3;
int motorFrwrd = 13;
int motorBckwrd = 12;
int motorPWM = 11;
int hiz = 150;
double rpm;
int periyot;
double periyot_sag;
double periyot_sol;
unsigned long simdiki_zaman;
unsigned long onceki_zaman = 0;
int sayac = 0;
int yazma = 0;
int gelenVeri;
int degisken = 0;
int x;
double gecikme = 0;
int cevir;
int kontrol = 1;

void setup() {
  Serial.begin(9600);
  pinMode(sensor_pin, INPUT);
  pinMode(pwr_led, OUTPUT);
  pinMode(motorFrwrd, OUTPUT);
  pinMode(motorBckwrd, OUTPUT);
  pinMode(motorPWM, OUTPUT);
  digitalWrite(motorFrwrd, HIGH);
  digitalWrite(motorBckwrd, LOW);
}

void yak(int yakKontrol) {
  if (yakKontrol == 1) {
    delay(periyot * gecikme);
    digitalWrite(pwr_led, HIGH);
    delayMicroseconds(50);
    digitalWrite(pwr_led, LOW);
  }

  else if (yakKontrol == 2) {
    periyot_sag = periyot_sag + 0.5;
    delay(periyot_sag);
    digitalWrite(pwr_led, HIGH);
    delayMicroseconds(50);
    digitalWrite(pwr_led, LOW);
    if (periyot_sag >= 2 * periyot) {
      periyot_sag = periyot;
    }

  }
  else if (yakKontrol == 3) {
    periyot_sol = periyot_sol - 0.5;
    delay(periyot_sol);
    digitalWrite(pwr_led, HIGH);
    delayMicroseconds(50);
    digitalWrite(pwr_led, LOW);
    if (periyot_sol <= 1) {
      periyot_sol = periyot;
    }
  }
}

//void sagaCevir() {
//  periyot_sag = periyot;
//  while (periyot_sag <= periyot * 2) {
//    delay(periyot_sag);
//    digitalWrite(pwr_led, HIGH);
//    delayMicroseconds(50);
//    digitalWrite(pwr_led, LOW);
//    periyot_sag = periyot_sag + 0.5;
//  }
//  if (gelenVeri == 101) {
//    sagaCevir();
//  }
//}
//
//void solaCevir() {
//  periyot_sol = periyot;
//  while (periyot_sol >= 0) {
//    delay(periyot_sol);
//    digitalWrite(pwr_led, HIGH);
//    delayMicroseconds(50);
//    digitalWrite(pwr_led, LOW);
//    periyot_sol = periyot_sol - 0.5;
//  }
//  if (gelenVeri == 102) {
//    solaCevir();
//  }
//}

void loop() {

  sensor_durum = digitalRead(sensor_pin);
  if (sensor_durum == degisken) {
    sayac++;
    degisken = !degisken;
    x = sayac % 2;
    if (x == 0) {
      simdiki_zaman = millis();
      periyot = simdiki_zaman - onceki_zaman;
      //    Serial.println(periyot);
      onceki_zaman = millis();
      
      yak(kontrol);
    }
  }

  //Motor Hızı
  analogWrite(motorPWM, hiz);

  //Haberleşme
  if (Serial.available() > 0) {
    gelenVeri = Serial.read();
    Serial.println(gelenVeri);
    if (gelenVeri == 97)
    {
      gecikme = 0;
      kontrol = 1;
    }
    else if (gelenVeri == 98)
    {
      gecikme = 0.25;
      kontrol = 1;
    }
    else if (gelenVeri == 99)
    {
      gecikme = 0.5;
      kontrol = 1;
    }
    else if (gelenVeri == 100)
    {
      gecikme = 0.75;
      kontrol = 1;
    }
    else if (gelenVeri == 101)
    {
      kontrol = 2;
      periyot_sag = periyot;
    }
    else if (gelenVeri == 102)
    {
      kontrol = 3;
      periyot_sol = periyot;
    }
    else if (gelenVeri <= 57 && gelenVeri >= 48)
    {
      hiz = map(gelenVeri, 48, 57, 80, 255);
      Serial.println(hiz);
    }
    else if (gelenVeri == 115)
    {
      digitalWrite(motorFrwrd, LOW);
      digitalWrite(motorBckwrd, LOW);
    }
    else if (gelenVeri == 114)
    {
      digitalWrite(motorFrwrd, HIGH);
      digitalWrite(motorBckwrd, LOW);
    }
    else if (gelenVeri == 43)
    {
      gecikme = gecikme + 0.01;
      Serial.println(gecikme);
      Serial.println(periyot);
      Serial.println(periyot * gecikme);
    }
    else if (gelenVeri == 45)
    {
      gecikme = gecikme - 0.01;
      Serial.println(gecikme);
      Serial.println(periyot);
      Serial.println(periyot * gecikme);
    }
  }

}
