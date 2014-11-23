/***********************************************************
 * Arduino YUN Time Synchronization
 * license: Creative Commons Attribution 4.0
 * author: Physics Light
 * date: 2 July 2014
 ***********************************************************/
// Libraries

// Init YUN Server
YunServer server; // init YunServer object named server

// Function
void msgCmd (YunClient client) {
  char command;
  String msgCmd = client.readStringUntil('/');
  msgCmd.trim();
  // clock setting message
  if (msgCmd == "clock") {
    // call the function to set time
    setBoardTime(client);
  }
}
// set onboard time via browser clock
void setBoardTime(YunClient client) {
  String result;
  // read time from browser, format: seconds since 1 Jan 1970
  // after point there are milliseconds, eliminated
  String time = client.readStringUntil('.');
  time.trim();
  Process setTime;
  // @ time in seconds since 1 Jan 1970
  String cmdTimeStrg = "date +%s -s @" + time;
  setTime.runShellCommand(cmdTimeStrg);
}

void initYunServer() {
  server.listenOnLocalhost();
  server.begin();
}

void printDuinoTime() {
  // check the Arduino YUN internal time
  String boardTime = getTimeStamp();
  // start client
  YunClient client = server.accept(); // get clients coming from server
  if (client) {
    String cmd = client.readStringUntil('/');
    cmd.trim(); // eliminate whitespace
    // data command: print the Arduino YUN clock in client browser
    if ( cmd == "data") {
      client.print( "<p> Time " + boardTime + "</p>");
    }
    // msg command: get clock from client browser
    if ( cmd == "msg" ) {
      msgCmd(client);
    }
  }
  // stop client
  client.stop();
}


