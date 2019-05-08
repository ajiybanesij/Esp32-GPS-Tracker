#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>

TinyGPSPlus gps;
SoftwareSerial gpsSerial(16, 17); //rx,tx
HTTPClient http;

const char* ssid = "";          //Wifi Adı
const char* password = "";  //Wifi Sifresi

int LEDPIN = 23;
int REDLED = 34;
int YELLOWLED = 32;

void setup()
{
  pinMode(LEDPIN, OUTPUT);
  pinMode(REDLED,OUTPUT);
  pinMode(YELLOWLED,OUTPUT);
  Serial.begin(115200);


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Wifi Bağlanılıyor...");
  }
  Serial.println("Wifi Bağlandı.");
  delay(1000);

  gpsSerial.begin(9600);
}

void loop()
{
  digitalWrite(LEDPIN, HIGH);
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      displayInfo();
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("GPS Modülü Bulunamadı"));
    while (true)
      ;
  }
  digitalWrite(LEDPIN, LOW);
  delay(100);
  
}

void displayInfo()
{
  digitalWrite(YELLOWLED, LOW);

  String LatData = "";
  String LongData = "";
  if (gps.location.isValid())
  {
    //Serial.print("Altitude: ");
    //Serial.println(gps.altitude.meters());
    LatData = String(gps.location.lat(), 6);
    LongData = String(gps.location.lng(), 6);
    GPSDate(LatData, LongData);
    digitalWrite(YELLOWLED, HIGH);
  }
  else
  {
    digitalWrite(YELLOWLED, LOW);
    Serial.println("Lokasyon Bulunamadı.");
    LatData = "";
    LongData = "";
  }
  Serial.println();
  Serial.println();
}

void GPSDate(String LatData, String LongData)
{
  String DateData = "";
  if (gps.date.isValid())
  {
    DateData = String(gps.date.month()) + "=" + String(gps.date.day()) + "=" + String(gps.date.year());
    GPSTime(LatData, LongData, DateData);
  }
  else
  {
    DateData = "";
    Serial.println("Tarih Bulunamadı.");
  }
}
void GPSTime(String LatData, String LongData, String DateData)
{
  String DateTime = "";
  String TimeData = "";
  if (gps.time.isValid())
  {
    String saat = "";
    String dakika = "";
    String saniye = "";
    if (gps.time.hour() < 10) {
      saat = "0" + String(gps.time.hour());
    } else {
      saat = String(gps.time.hour());
    }
    if (gps.time.minute() < 10) {
      dakika = "0" + String(gps.time.minute());
    } else {
      dakika = String(gps.time.minute());
    }
    if (gps.time.second() < 10) {
      saniye = "0" + String(gps.time.second());
    } else {
      saniye = String(gps.time.second());
    }
    TimeData = String(saat) + "=" + String(dakika) + "=" + String(saniye);
    DateTime = String(DateData) + "-" + String(TimeData);
    SendData(LatData, LongData, DateTime);
  }
  else
  {
    TimeData = "";
    Serial.println("Zaman Bulunamadı.");
  }
}

void SendData(String LatData, String LongData, String DateTime) {
  Serial.println(LatData);
  Serial.println(LongData);
  Serial.println(DateTime);
  Serial.println();
  

  if ((WiFi.status() == WL_CONNECTED)) {
    String URL = "/api/add/" + LatData + "/" + LongData + "/" + DateTime;
    http.begin(URL);
    int httpCode = http.GET();
    if (httpCode > 0) {
      digitalWrite(REDLED,HIGH);
      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
      delay(1000);
      digitalWrite(REDLED,LOW);
    }
    else {
      Serial.println("HTTP isteği hatalı!");
    }
    http.end();
  }
}
