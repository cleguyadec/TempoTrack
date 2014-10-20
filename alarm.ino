#include <DHT.h> //temp sensor

float TEMPERATURE_MAX_ALLOWED;
float TEMPERATURE_MIN_ALLOWED;
/** 
  -X=nombre de fois où la temperature était au dessous du seuil 
  0= RAS
  X=nombre de fois où la temperature était au dessus du seuil 
**/
int STATE = 0; 
/** 
  le temps max qu'on peut rester hors norme
**/
int maxDeviationAllowed=10;

/**
  si le seuil a été dépassé trop longtemps
**/
boolean blocked=false;

void initTemperatureCaptor(float minAllowed,float maxAllowed){
  
  dht.begin();  
  TEMPERATURE_MIN_ALLOWED=minAllowed;
  TEMPERATURE_MAX_ALLOWED=maxAllowed;
        
}

float readSensorTemperature () {
     float currentTemp = dht.readTemperature();
     updateState(currentTemp);
     checkStateCriticity();
     return currentTemp;
}

void checkStateCriticity(){
    if(abs(STATE)>maxDeviationAllowed){
      blocked=true;
    }
}

void updateState(float currentTemp){
       if(currentTemp<TEMPERATURE_MIN_ALLOWED){
         STATE=STATE-1;
     } else if(currentTemp>TEMPERATURE_MAX_ALLOWED){
         STATE=STATE+1;
     } else{
         STATE=0;
     }
}
String getStateMessage(){
    if(STATE==0) {
       return "accepted value";
    } else if(STATE<0){
       return "Erreur Cons inf";
    } else if(STATE>0){
       return "Erreur Cons sup";
    } else {
       return "WTF value";
    }
}

boolean isBlocked(){
   return blocked;      
}
