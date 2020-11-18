// * ARDUINO RELAY TIMER - September 25th 2018

#include "header.h"
#include "utils.h"
#include "getters.h"
#include "parseReceivedText.h"
#include "printFunctions.h"
#include "closeConnection.h"
#include "setParam.h"
#include "saveData.h"
#include "arrayToString.h"
#include "checkRelays.h"
#include "addRelay.h"
#include "deleteRelay.h"

void setup() {
  // In order to reset to work, a 1k resistor must be placed between the reset pin
  // and the selected pin. Otherwise, the board will be in an infinite loop because
  // at start every pin is set to OUTPUT an LOW for a moment.
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, HIGH);
  
  Serial.begin(9600);

  Serial.print(F("Starting I2C port: "));
  Wire.begin();
  Serial.println(F("done...\n"));

  Serial.print(F("Starting RTC clock: "));
  RTC.begin();
  Serial.println(F("done...\n"));

  // Start the telnet server
  Serial.print(F("Starting telnet server: "));
  server.begin();
  Serial.println(F("done....\n"));
  
  loadSystemData();
}

void loop() {
  // put your main code here, to run repeatedly:

  if(server.available() && !connectFlag) {
    connectFlag = 1;
    client = server.available();
    client.flush();
    client.println(F("ARDUINO RELAY TIMER"));
    client.print(BOARD);
    client.println(" - " + String(sys.hostName));
    client.println("Version " + sysVersion);
    client.println(F("Type 'help' to show all commands"));
    client.flush();
    printPrompt();
  }

  // Check to see if text received
  if (client.connected() && client.available()) getReceivedText("telnet");

  // Check to see if connection has timed out
  if(connectFlag) checkConnectionTimeout();

  if(Serial.available()) getReceivedText("serial");
    
  // Check relays every five seconds...
  if(millis() - tstamp >= 5000) {
    checkRelays();
    tstamp = millis();
  }
}

void checkConnectionTimeout()
{
  if(millis() - timeOfLastActivity > allowedConnectTime) {
    client.println();
    client.println(F("Timeout disconnect."));
    client.stop();
    connectFlag = 0;
  }
}
