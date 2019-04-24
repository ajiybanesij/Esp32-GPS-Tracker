
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>
 
const char* ssid = "ssid";
const char* password = "pass";
String dateTimeData="";

TinyGPSPlus gps;
SoftwareSerial gpsSerial(16, 17); //rx,tx



void setup()
{
  Serial.begin(115200);
  gpsSerial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

}

void loop()
{
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      displayInfo();
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected"));
    while (true)
      ;
  }
  delay(100);
}

void displayInfo()
{
  if (gps.location.isValid())
  {
    dateTimeData="";
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
 
    GPSDate();
    GPSTime(String(gps.location.lat(), 6),String(gps.location.lng(), 6));
  }
  else
  {
    Serial.println("Location: Not Available");
  }
  Serial.println();
  Serial.println();
  delay(100);
}

void GPSDate()
{
  Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
    String dateDatas=String(gps.date.month())+"="+String(gps.date.day())+"="+String(gps.date.year());
    dateTimeData=dateTimeData+""+dateDatas;
  
  }
  else
  {
    Serial.println("Not Available");
  }
}
void GPSTime(String lats,String lngs)
{
  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10)
      Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10)
      Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10)
      Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10)
      Serial.print(F("0"));
    Serial.println(gps.time.centisecond());

    String timeDatas=String(gps.time.hour())+"="+String(gps.time.minute())+"="+String(gps.time.second() );
    dateTimeData=dateTimeData+"-"+timeDatas;
    Serial.println("********");
    Serial.println(dateTimeData);
    SendData(lats,lngs,dateTimeData);
  }
  else
  {
    Serial.println("Not Available");
  }
}

void SendData(String lats,String lngs,String datetimew){
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;
    String URL="";

    http.begin(URL); //Specify the URL
    int httpCode = http.GET();                                        //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
  }
}
