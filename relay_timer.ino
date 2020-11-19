// * ARDUINO RELAY TIMER - September 25th 2018

#include "header.h"
#include "utils.h"
#include "getters.h"
#include "setters.h"
#include "parser.h"
#include "printFunctions.h"
#include "setParam.h"
#include "saveData.h"
#include "relayFunctions.h"

void setup() {
  Serial.begin(BAUD_RATE);

  Serial.print(F("Starting I2C port: "));
  Wire.begin();
  Serial.println(F("done...\n"));

  Serial.print(F("Starting RTC clock: "));
  RTC.begin();
  Serial.println(F("done...\n"));

  // Start the telnet server
  Serial.print(F("Starting telnet server: "));
  server.begin();
  Serial.println(F("done...\n"));
  
  loadSystemData();

  // In order to get reset functionality to work, a 1k resistor must be placed between the reset pin
  // and the selected pin. Otherwise, the board will be in an infinite loop because
  // at start every pin is set to OUTPUT an LOW for a moment.
  pinMode(sys.resetPin, OUTPUT);
  digitalWrite(sys.resetPin, HIGH);
}

void loop() {
  if(server.available() && !connectFlag) {
    connectFlag = 1;
    client = server.available();
    client.flush();
    client.println(F("ARDUINO RELAY TIMER"));
    client.print(BOARD);
    client.println(" - " + String(sys.hostname));
    client.println("Version " + sysVersion);
    client.println(F("Type 'help' to show all commands"));
    client.flush();
  }

  // Check to see if text received
  if (client.connected() && client.available()) {
    getReceivedText("telnet");
    printPrompt();
  }

  // Check to see if connection has timed out
  if(connectFlag) checkConnectionTimeout();

  if(Serial.available()) getReceivedText("serial");
    
  // Check relays every five seconds...
  if(millis() - tstamp >= 5000) {
    checkRelays();
    tstamp = millis();
  }
}
