// * ARDUINO RELAY TIMER - September 25th 2018

#include "header.h"
#include "utils.h"
#include "getters.h"
#include "setters.h"
#include "printFunctions.h"
#include "saveData.h"
#include "relayFunctions.h"
#include "parser.h"

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
    output = COMM_TELNET;
    client = server.available();
    client.println(F("ARDUINO RELAY TIMER"));
    client.print(BOARD);
    client.println(" - " + String(sys.hostname));
    client.print(F("Version "));
    client.println(VERSION);
    client.println(F("Type 'help' to show all commands"));
    printPrompt();
  }

  // Check to see if text received
  if (client.connected() && client.available()) {
    getReceivedText();
    printPrompt();
  }

  // Check to see if connection has timed out
  if(connectFlag) checkConnectionTimeout();

  if(Serial.available()) {
    if (connectFlag) {
      while(Serial.read() != -1) {}
      printData(F("A telnet session is currently running..."), true);
    }
    else {
      output = COMM_SERIAL;
      getReceivedText();
    }
  }
    
  // Check relays every five seconds...
  if(millis() - tstamp >= 5000) {
    checkRelays();
    tstamp = millis();
  }
}
