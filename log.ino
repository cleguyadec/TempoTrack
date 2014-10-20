
#include <FileIO.h> 

String makeTimeStampString (String dataToWrite){
    
  if (dataToWrite){
    // make a string that start with a timestamp for assembling the data to log:
    String dataString;
    dataString += getTimeStamp();
    dataString += ";";
    dataString += String(dataToWrite);
    return dataString;
  }else{return "error";}
}

void writeToFile(File dataFile, String dataString){
    
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);       
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening");
  }
}

// This function return a string with the time stamp
// MS14 will call the Linux "data" command and get the time stamp
String getTimeStamp() {
  String result;
  Process time;
  // date is a command line utility to get the date and the time 
  // in different formats depending on the additional parameter 
  time.begin("date");
  time.addParameter("+%D;%T");  // parameters: D for the complete date mm/dd/yy
                                //             T for the time hh:mm:ss    
  time.run();  // run the command

  // read the output of the command
  while(time.available()>0) {
    char c = time.read();
    if(c != '\n')
      result += c;
  }
  
  return result;
}

