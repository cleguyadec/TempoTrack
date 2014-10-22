
/**************************************************************************/
/*! 
    @file     TempoTrack.ino
    @author   Le Guyadec Clement
    @license  BSD (see license.txt)

*/
/**************************************************************************/

//Include
#include <Wire.h> //I2C for RFID
#include <Adafruit_NFCShield_I2C.h> //RFID shield
#include <LiquidCrystal.h>  //LCD
#include <DHT.h> //temp sensor
#include <Console.h> 
#include <FileIO.h>

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
boolean LOG_TEMPERATURE_ENABLE = false;

String dataLog;
String rfidId;
String rfiIdTemp;
int boxCount = 0;

void setup() {
   // Initialize the Bridge and the Serial 
  FileSystem.begin();
  Bridge.begin();
  Serial.begin(9600);
  //while(!Serial); //wait for the Serial port to connect.
    
  initTemperatureCaptor(21,26);
  Serial.println("Hello!");
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  printLcd("Ready",0.0);
  
  //initialize RFID
  initRfidReading();
}

void loop() {
  delay(1000);
  timeCount++;
  
  if(isButtonSelectPressed()){
    if(LOG_TEMPERATURE_ENABLE){
      stopLogging();
    }else{
      startLogging();  
    }
  }
  
 if (LOG_TEMPERATURE_ENABLE){  
    
    rfidId = readRfidCard();
    
    if(timeCount > 10 || rfidId != ""){
      float currentTemp=readSensorTemperature();
      
      //Serial.println(dataLog);
      logTemperature(currentTemp,rfidId); 
      
      //Serial.print(rfiIdTemp);
      //Serial.println(boxCount);
      
      if (rfiIdTemp != rfidId){
        //Serial.println(rfiIdTemp);
        boxCount++;
        rfiIdTemp = rfidId;
        }
         
        
      if(isBlocked()) {
        printLcd("GAME OVER",0.0);
        }else{
          printLcd(getStateMessage(),currentTemp);
          lcd.setCursor(6, 2);
          lcd.print(rfidId);
          lcd.setCursor(11, 0);
          lcd.print(boxCount);
        }
        timeCount = 0;
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

void startLogging(){
      LOG_TEMPERATURE_ENABLE = true;
      writeToFile(makeTimeStampString("0.0;start reading temp"));
      printLcd("Start logging",0.0);
}

void stopLogging(){
    LOG_TEMPERATURE_ENABLE = false;
    writeToFile(makeTimeStampString("0.0;reading temp off"));
    printLcd("Stop logging",0.0);
}

boolean isButtonSelectPressed(){
  return read_LCD_buttons() == btnSELECT;
}


