//////////////////////// LIBRARIES ////////////////////////

#include "DHT.h"                    //Library for ESP32.        (M/T)
#include "ThingSpeak.h"             // Thingspeak Lib.         (TH-SP)
#include <Wire.h>                   //To setup I2C WIRES
#include <LiquidCrystal_I2C.h>      //For I2c LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);
#include <ESP32Time.h>              //A very important library
#include <TimeLib.h>                //Epoch Converter DO NOT DELETE
#include <WiFi.h>                   //Wifi capabilities Enabled
#include <NTPClient.h>              //For retrieving data from NTP SERVER
#include <WiFiUdp.h>                //Extra Library

////////////////////////// PINES //////////////////////////

#define RX2 16
#define TX2 17
#define DHTPIN 13 //D13 PIN                         (M/T)                                                           
#define DHTTYPE DHT11 // TYPE OF SENSOR.            (M/T)
#define MQ_135 (32)//defining the pin of the sensor (CO2)
#define LED_RED 12
#define LED_YELLOW 27
#define LED_PIN 26
#define ISRPINGPIO 34 //Pin chosen for the hard reset button.
#define ISRPINGPIO2 35 //Pin chosen for the cleaning.

///////////////////// Variable Definitions ////////////////

int CO2; // CO2 Variable                            (CO2)
ESP32Time rtc;//Real time clock of the esp32 activatedESP32Time rtc;//Real time clock of the esp32 activated
//const char* ssid = "Students"; //        (TH-SP) //ssid = "WIFI OLIVERA (2.4)"; //
//const char* password = "P0l1t3cn1c4.b1s"; //      (TH-SP) //password = "492CAE24325363C4"; //
//const char *ssid     = "IZZI-A582";     //  WiFi name
//const char *password = "F0AF854FA582";    // WiFi password
const char *ssid     = "ZTE Blade V2020";     //  WiFi name
const char *password = "Andy1234";    // WiFi password
//const char* ssid = "Students"; //        (TH-SP) //ssid = "WIFI OLIVERA (2.4)"; //
//const char* password = "P0l1t3cn1c4.b1s"; //      (TH-SP) //password = "492CAE24325363C4"; //
unsigned long channelID = 1581138; //              (TH-SP)
const char* WriteAPIKey = "BA4NUQ1U7ZUNJMB1"; //   (TH-SP)

WiFiClient Client;  //                             (TH-SP)
DHT dht(DHTPIN, DHTTYPE);//OBJECT CREATION          (M/T)
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -18000, 10000); //YUCATAN IS -6 GMT, HENCE 3600*-6 AND 1 SECOND OFFSET
// NTPClient(UDP& udp, const char* poolServerName, long timeOffset, unsigned long updateInterval);
char dayWeek [7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

////////////////////////// SETUP /////////////////////////

//////////////////////// The Interruption ////////////////////



void IRAM_ATTR isr() {
  //lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print("Restaring");//Prints temperature 
  delay(100);//Prevents spam
  ESP.restart(); //HARD RESET FOR THE ESP32
  //detachInterrupt(ISRPINGPIO);//prevents the user from spamming or jamming the device with the button
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!THE INTERRUPTION
void IRAM_ATTR LED() {
  //lcd.clear();
  //Serial.println("RESTARTING");//DO NOT PUT SERIAL IN AN INTERRUPTION
  toggledevice(LED_PIN);
}
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  


void toggledevice(int pin){//GOES TO ON TO OFF AND FROM OFF TO ON LIKE A SWITCH 
         int logicstatus=digitalRead(pin);
         delay(100);
         if(logicstatus==0){digitalWrite(pin, HIGH);}
         if(logicstatus==1){digitalWrite(pin, LOW);}     
  }  

void setup() {
   Serial.begin(9600);//BAUD RATE          (M/T) & (CO2) & (TH-SP)
   Serial2.begin(115200, SERIAL_8N1,RX2,TX2);

  //pinMode(ISRPINGPIO, INPUT_PULLUP);//SETTING PIN MODE
  pinMode(ISRPINGPIO2, INPUT_PULLUP);//SETTING PIN MODE
  pinMode(LED_PIN, OUTPUT);//SETTING PIN MODE 
  digitalWrite(LED_PIN, LOW);
  attachInterrupt(ISRPINGPIO, isr, FALLING);//INTERRUPT ADDED
  //attachInterrupt(ISRPINGPIO2, LED, FALLING);//INTERRUPT ADDED


  
   WiFi.begin(ssid,password);  //                   (TH-SP)
   Wire.begin(21,22); //SDA,SCL  
   lcd.begin(); 
   lcd.backlight();
    delay(1100);// Wifi Verification Time //        (TH-SP)
    Serial.print("Connecting...\n"); //             (TH-SP)
    lcd.setCursor(0, 0);
    lcd.print("CONNECTING          ");
    while(WiFi.status()!= WL_CONNECTED){  //        (TH-SP)
      lcd.setCursor(0, 0);
      lcd.print("CONNECTING          ");
      delay(500);// Wifi Verification Time //        (TH-SP)
      Serial.print ( "." );
    }

     timeClient.begin();
  int a=0,b=0,c=0,d=0,e=0,f=0;
  gettimefromwifi(a,b,c,d,e,f);
  //(horas,minutos,segundos,anio,mes,fecha)
  rtc.setTime(c, b, a, f, e+1, d);  // Obtienes la fecha actual de NTP y del EPOCHTIME
    
    Serial.print("Wifi Connected\n"); //            (TH-SP)
    Serial.println("-----------------------");   // (TH-SP)
    
    ThingSpeak.begin(Client); //                    (TH-SP)
    
    dht.begin();//STARTS SENSOR                      (M/T)
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    
}


/////////////////////// WIFI CONNECTED ///////////////////////

void showWIFIISCONNECTED(){//This function shows is the device is connected to a local network
     lcd.setCursor(12,3);
    if(WiFi.status()!= WL_CONNECTED){
          lcd.print("No WiFi");
      }else{
          lcd.print("   WiFi");
        }
        

  }



//////////////////////// LCD TEMP ////////////////////

void LCDShowTemp(int x, int y){///displays temps
   lcd.setCursor(x,y);
   float t = dht.readTemperature();
   float temp=(t/1000)*1.0e3;
   if(t<10){
    lcd.print("0");
    lcd.print(temp);
    }else{
      lcd.print(temp);
      }//Hour
   //lcd.print("%.1f",t);//Prints temperature  
   lcd.print("C");//Prints temperature 
   ThingSpeak.setField(1,t);            //          (TH-SP)
  }

  ////////////////////// Time from Internet //////////////////
  
void gettimefromwifi(int& horas, int& minutos, int& segundos, int& anio, int& mes, int& fecha){
  timeClient.update();
  Serial.println(dayWeek[timeClient.getDay()]); 
  //Serial.println(" ");
  horas=timeClient.getHours();
  minutos=timeClient.getMinutes();
  segundos=timeClient.getSeconds();
  //Serial.println(timeClient.getEpochTime()); //Check unix hash date
  anio=year(timeClient.getEpochTime());
  mes=month(timeClient.getEpochTime());
  fecha=day(timeClient.getEpochTime());
  //Serial.println(anio);
  //Serial.println(mes);
  //Serial.println(fecha);
  //delay(800);
  }
  
  //////////////////////// Moist /////////////////////

void get_humidity(int x, int y){       //           (M/T)
  float h = dht.readHumidity();        //            (M)
  //float t = dht.readTemperature();     //            (T)

  Serial.print("Moisture: ");          //            (M)
  Serial.print(h);                     //            (M)
  Serial.println(" %.");               //            (M)
  lcd.setCursor(x,y);
  lcd.print("M:"+String(h)+"%");
  
  
  /*Serial.print("Temperature: ");       //            (T)
  Serial.print(t);                     //            (T)
  Serial.println(" ºC.");              //            (T)
  lcd.setCursor(0,1);
  lcd.print("T:"+String(t)+"C");*/

    if(h>80){//MOISTURE WARNING
    digitalWrite(LED_YELLOW, HIGH);
    }else{
      digitalWrite(LED_YELLOW, LOW);
      }

  //ThingSpeak.setField(1,t);            //          (TH-SP)
  ThingSpeak.setField(2,h);            //          (TH-SP)
  }

  ///////////////////////////// CO2 //////////////////////////////////

void Co2(int x, int y){                            //           (CO2) 
  CO2=analogRead(MQ_135);              //           (CO2)
  //Serial.println(CO2);               //           (CO2)
  
  if(CO2>400){//WARNING IN CASE OF BAD AIR
    digitalWrite(LED_RED, HIGH);
    }else{
      digitalWrite(LED_RED, LOW);
      }
  //
  Serial.print("CO2: ");               //           (CO2)
  Serial.print(CO2,DEC);               //           (CO2)
  Serial.println(" ppm.");             //           (CO2)
  lcd.setCursor(x,y);
  lcd.print(String(CO2)+"ppm");
  ThingSpeak.setField(3,CO2);          //          (TH-SP)
  }

  /////////////////////// PRINT TIME //////////////////

void showthehour(int WORKHOUR,int MINUTEHOUR,int SECONDHOUR,int DAYDATE,int MONTH,int YEAR,String DATTIME){
  lcd.setCursor(0, 0);
  lcd.print("Hour: ");
  if(WORKHOUR<10){lcd.print("0");lcd.print(WORKHOUR);lcd.print(":");}else{lcd.print(WORKHOUR);lcd.print(":");}//Hour
  if(MINUTEHOUR<10){lcd.print("0");lcd.print(MINUTEHOUR);lcd.print(":");}else{lcd.print(MINUTEHOUR);lcd.print(":");}//Minutes
  if(SECONDHOUR<10){lcd.print("0"); lcd.print(SECONDHOUR);lcd.print(" ");}else{lcd.print(SECONDHOUR);lcd.print(" ");}//Seconds
  lcd.print(DATTIME);//Am or Pm
  lcd.print("   ");
  
  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  if(DAYDATE<10){lcd.print("0");lcd.print(DAYDATE);lcd.print("/");}else{lcd.print(DAYDATE);lcd.print("/");}//Day
  if(MONTH<10){lcd.print("0");lcd.print(MONTH);lcd.print("/");}else{lcd.print(MONTH);lcd.print("/");}//Month
  lcd.print(YEAR);lcd.print(" "); //Year
  }

  //////////////////////// LOOP //////////////////////

  void loop() {

    
    showWIFIISCONNECTED();

    // put your main code here, to run repeatedly:
  int pikachu=rtc.getHour();
  int tortuga=rtc.getMinute();
  int lamborghini=rtc.getSecond();
  String perro=rtc.getAmPm();
  int mona=rtc.getDay();
  int galleta=rtc.getMonth();
  int cacerola=rtc.getYear();

  int tomatito;

  //twelve2twentyfour(perro,pikachu,tomatito); 
  pikachu=pikachu-1;//substract 1 hoir

        
      Serial.println(pikachu);//hora
      Serial.println(tortuga);// min
      Serial.println(perro);// min
      

  showthehour(pikachu,tortuga,lamborghini,mona,galleta,cacerola,perro);
  
  //delay(1000); //Time between readings.    (M/T) & (CO2) & (TH-SP)
  LCDShowTemp(0,3);
  get_humidity(9,2);                      //           (M/T)
  Co2(1,2);                               //           (CO2)
 
  ThingSpeak.writeFields(channelID,WriteAPIKey);// (TH-SP)
  Serial.print(Serial2.readString());
  Serial.println("Data Sent to ThingSpeak");    // (TH-SP)
  Serial.println("-----------------------");    // (TH-SP)

  //delay(27000); //Time between readings.    (M/T) & (CO2) & (TH-SP)
}
