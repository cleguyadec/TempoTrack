
/**************************************************************************/
/*! 
    @file     TempoTrack.ino
    @author   Le Guyadec Clement
    @license  BSD (see license.txt)

*/
/**************************************************************************/

//Include
#include <Wire.h> //I2C for RFID
#include <Console.h> 
#include <FileIO.h>
#include <Adafruit_NFCShield_I2C.h> //RFID shield
#include <LiquidCrystal.h>  //LCD
#include <DHT.h> //temp sensor

#define DHTPIN A1     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 sensor
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);        // select the pins used on the LCD panel

/**
modulo pour découpler la lecture RFID de la lecture de temperature.
**/
int timeCount = 0;

/**
alias des boutons du LCD
**/
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

/**
Les booleans pour la gestion des boutons
**/
boolean LOGTEMP = false;

String dataLog;

void setup() {
   // Initialize the Bridge and the Serial 
  FileSystem.begin();
  Bridge.begin();
  Serial.begin(9600);
  while(!Serial); //wait for the Serial port to connect.
  
  initTemperatureCaptor(21,26);
  Serial.println("Hello!");
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  //initialize RFID
  initRfidReading();
}

void loop() {
  delay(1000);
  timeCount++;
  String rfidId;
  String rfiIdTemp;

  
  if(read_LCD_buttons() == btnLEFT){
    if(LOGTEMP == false){
      LOGTEMP = true;
      // open the file. note that only one file can be open at a time,
      // so you have to close this one before opening another.
      // The FileSystem card is mounted at the following "/mnt/FileSystema1"
      File dataFile = FileSystem.open("/mnt/sda1/datalog.csv", FILE_APPEND);
      writeToFile(dataFile, makeTimeStampString(";; start reading temp")); 
      }else{
        LOGTEMP = false;
        // open the file. note that only one file can be open at a time,
        // so you have to close this one before opening another.
        // The FileSystem card is mounted at the following "/mnt/FileSystema1"
        File dataFile = FileSystem.open("/mnt/sda1/datalog.csv", FILE_APPEND);
        writeToFile(dataFile, makeTimeStampString(";; reading temp off")); 
      }
  }
  
  
  if (LOGTEMP == true){  
    
    rfidId = readRfidCard();
    
    if(timeCount > 30 || rfidId != ""){
      float currentTemp=readSensorTemperature();
      
       // open the file. note that only one file can be open at a time,
      // so you have to close this one before opening another.
      // The FileSystem card is mounted at the following "/mnt/FileSystema1"
      File dataFile = FileSystem.open("/mnt/sda1/datalog.csv", FILE_APPEND);
      
      String dataLog = makeTimeStampString(String(currentTemp));
      
      writeToFile(dataFile, dataLog); 
      
      if(isBlocked()) {
        printLcd("Consigne enfreinte",0.0);
      }else{
        printLcd(getStateMessage(),currentTemp);
      }
   } 
  }
}

void printLcd(String labelMessage,float temperature){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(labelMessage);
        lcd.setCursor(0, 2);
        lcd.print(temperature);
}





