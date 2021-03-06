#include <FileIO.h>

char* reportFilePath;

void initReportFile(char* filePath) {
  reportFilePath = filePath;
}

String makeTimeStampString (String dataToWrite) {
  if (dataToWrite) {
    // make a string that start with a timestamp for assembling the data to log:
    String dataString;
    dataString += getTimeStamp();
    dataString += ";";
    dataString += String(dataToWrite);
    return dataString;
  } else {
    return "error";
  }
}

void writeToFile(String dataString) {
  File outfile = FileSystem.open(reportFilePath, FILE_APPEND);
  // if the file is available, write to it:
  if (outfile) {
    outfile.println(dataString);
    outfile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("error openning");
  }
}

void logTemperature(float temperature) {
  writeToFile(makeTimeStampString(String(temperature)));
}

void logTemperature(float temperature, String user) {
  writeToFile(makeTimeStampString(String(temperature)) + ";" + user);
}
// This function return a string with the time stamp
// MS14 will call the Linux "data" command and get the time stamp

String getTimeStamp() {
  String result;
  Process time;
  // date is a command line utility to get the date and the time
  // in different formats depending on the additional parameter
  time.runShellCommand("echo `date +%F`T`date +%T`Z");
  // do nothing until the process finishes, so you get the whole output:
  while (time.running());
  // Read command output.
  while (time.available() > 0) {
    char c = time.read();
    if (c != '\n')
      result += c;
  }
  return result;
}
