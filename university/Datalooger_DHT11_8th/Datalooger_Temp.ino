//------------------------- WIFI and Server SETUP --------------------------- 
#include <WiFi.h>
const char* ssid = "Red_Herrera";     // your network SSID (name of wifi network)
const char* password = "Herrera1020"; // your network password
WiFiClient  client;
//------------------------- ThinkSpeak Variables ---------------------------
#include "ThingSpeak.h"
unsigned long myChannelNumber = 1791093; //Write your canal number of ThingSpeak
//unsigned long myChannelNumber = 1793715; //Write your canal number of ThingSpeak
const char * myWriteAPIKey = "DA10TOJQK6X9LTTM"; //Write your KEY API
//const char * myWriteAPIKey = "4NZWPSSD36STQHW2"; //Write your KEY API

//------------------------- LCD ---------------------------
#include <LiquidCrystal.h>//include LCD library
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);// initialize the library with the numbers of the interface pins
//------------------------- DHT11 ---------------------------
#include "DHT.h"

#define DHTPIN 22
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
//------------------------- Sensor Variables ---------------------------
float Temp=0;
float Moist=0;
int x=0;
 
void setup() {
  Serial.begin(115200); //Computer Serial
  //------------------Configurate WiFi------------------- 
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // wait 1 second for re-trying
    delay(1000);
  }
  Serial.print("Connected to ");
  Serial.println(ssid);
  //------------------------ThingSpeak----------------------------- 
  Serial.println("Attempting to connect to Start Thingspeak Server ");
  ThingSpeak.begin(client); //Start ThingSpeak Server
  Serial.println("Ok");
  delay(1000);
  //-----------------------Configurate Pines-----------------------
  lcd.begin(16, 2);// set up the LCD's number of columns and rows
  dht.begin();
}
 
void loop() {
  
  Temp = dht.readTemperature();
  Moist = dht.readHumidity();
  
  lcd.setCursor(0, 0);
  lcd.print("Moist:");
  lcd.print(Moist);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.print(Temp);
  lcd.print("*C");
  delay(1000);

   x++;
  if(x>1800){
    ThingSpeak.setField(1, Moist);
    ThingSpeak.setField(2, Temp); 
    x=0;
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    Serial.println("Datos enviados a ThingSpeak");
  }

  Serial.println(x);
}
