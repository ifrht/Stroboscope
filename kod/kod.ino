#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//Sensör Değişkenleri
int sensor_pin = 6;
int sensor_durum;

//LED Değişkeni
int pwr_led = 7;

//Motor Değişkenleri
int motorFrwrd = 13;
int motorBckwrd = 8;
int motorPWM = 9;
int hiz = 150;

//Buck Converter Pulse
int buck = 10;

//Hesaplama Değişkenleri
double rpm;
int periyot;
double periyot_sag;
double periyot_sol;
unsigned long simdiki_zaman;
unsigned long onceki_zaman = 0;
int sayac = 0;
int gelenVeri;
int degisken = 0;
int x;
double gecikme = 0;
int kontrol = 1;
int eski_periyot;
int konum = 1;
unsigned long lcd_sayac = 0;

void setup() {
  //Haberleşmeyi başlatmak için kullanıldı. Baud Rate = 9600.
  Serial.begin(9600);
  //LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Power Electronic");
  lcd.setCursor(0, 1);
  lcd.print("Design Project");

  //Sensör Giriş Pini
  pinMode(sensor_pin, INPUT);
  //LED Kontrol Pini
  pinMode(pwr_led, OUTPUT);
  //Motor Pinleri
  pinMode(motorFrwrd, OUTPUT);
  pinMode(motorBckwrd, OUTPUT);
  pinMode(motorPWM, OUTPUT);
  //Motor Başlatma
  analogWrite(motorPWM, 150);
  digitalWrite(motorFrwrd, HIGH);
  digitalWrite(motorBckwrd, LOW);
  pinMode(buck, OUTPUT);
  tone(buck,1000);
}

//LED Kontrol Fonksiyonları
void yak(int yakKontrol) {
  if (yakKontrol == 1) {
    delay(periyot * gecikme);
    digitalWrite(pwr_led, HIGH);
    delayMicroseconds(300);
    digitalWrite(pwr_led, LOW);
  }

  else if (yakKontrol == 2) {
    periyot_sag = periyot_sag + 0.5;
    delay(periyot_sag);
    digitalWrite(pwr_led, HIGH);
    delayMicroseconds(300);
    digitalWrite(pwr_led, LOW);
    if (periyot_sag >= periyot) {
      periyot_sag = 0;
    }

  }
  else if (yakKontrol == 3) {
    periyot_sol = periyot_sol - 0.5;
    delay(periyot_sol);
    digitalWrite(pwr_led, HIGH);
    delayMicroseconds(300);
    digitalWrite(pwr_led, LOW);
    if (periyot_sol <= 1) {
      periyot_sol = periyot;
    }
  }
}

void loop() {
  sensor_durum = digitalRead(sensor_pin);
  if (sensor_durum == degisken) {
    sayac++;
    degisken = !degisken; //Sensörden gelen veri düzensiz olduğu için bu işlemler yapılmıştır.
    x = sayac % 2;        //Her geçişte sensör 2 pulse üretmektedir, bunu tek bir pulse'a çevirmek için bu işlemler yapılmıştır.
    if (x == 0) {
      simdiki_zaman = millis();
      eski_periyot = periyot;
      periyot = simdiki_zaman - onceki_zaman;
      onceki_zaman = millis();
      if (eski_periyot - periyot <= 2 && eski_periyot - periyot >= -2 ) {
        periyot = eski_periyot;
      }
      yak(kontrol);
    }
  }

  //Haberleşme
  if (Serial.available() > 0) {
    gelenVeri = Serial.read();
    if (gelenVeri == 97)
    {
      gecikme = 0;
      kontrol = 1;
      konum = 1;
    }
    else if (gelenVeri == 98)
    {
      gecikme = 0.25;
      kontrol = 1;
      konum = 2;
    }
    else if (gelenVeri == 99)
    {
      gecikme = 0.5;
      kontrol = 1;
      konum = 3;
    }
    else if (gelenVeri == 100)
    {
      gecikme = 0.75;
      kontrol = 1;
      konum = 4;
    }
    else if (gelenVeri == 101)
    {
      kontrol = 2;
      periyot_sag = 0;
    }
    else if (gelenVeri == 102)
    {
      kontrol = 3;
      periyot_sol = periyot;
    }
    else if (gelenVeri <= 9 && gelenVeri >= 0)
    {
      hiz = map(gelenVeri, 0, 9, 20, 200);
      //Motor Hız Kontrolü
      analogWrite(motorPWM, hiz);
    }
    else if (gelenVeri == 115)
    {
      digitalWrite(motorFrwrd, LOW);
      digitalWrite(motorBckwrd, HIGH);
    }
    else if (gelenVeri == 114)
    {
      digitalWrite(motorFrwrd, HIGH);
      digitalWrite(motorBckwrd, LOW);
    }
    else if (gelenVeri == 121)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Konum: ");
      lcd.print(konum);
      lcd.setCursor(0, 1);
      lcd.print("RPM: ");
      lcd.print(60000 / periyot);
    }
  }
}
