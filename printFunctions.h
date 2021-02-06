// Generic help
void printHelp() {
  printData(F("time\t\t\t--> Returns the current time."), true);
  printData(F("relay (pinNumber)\t--> Runs command on specified relay. Use 'relay help' for further options."), true);
  printData(F("set (parameter)\t\t--> Sets a given parameter. Use 'set help' for further options."), true);
  printData(F("show (parameter)\t--> Shows specified information. Use 'show help' for further options."), true);
  printData(F("save\t\t\t--> Save changes."), true);
  printData(F("reboot\t\t\t--> Restarts the system (TODO)."), true);
  printData(F("exit\t\t\t--> Close connection (telnet only)."), true);
  printData(F("quit\t\t\t--> Close connection (telnet only)."), true);
}

void showCommandHelp() {
  printData(F("show version\t\t--> Show system version."), true);
  printData(F("show ip\t\t\t--> Show system version."), true);
  printData(F("relays\t\t\t--> Show relay information."), true);
}

void relayCommandHelp() {
  printData(F("relay info\t\t\t-> Shows relay information."), true);
  printData(F("relay (pinNumber) create\t\t-> Creates a relay at the specified pin."), true);
  printData(F("relay (pinNumber) delete \t-> Deletes the specified relay."), true);
  printData(F("relay (pinNumber) enable\t-> Enables the specified relay."), true);
  printData(F("relay (pinNumber) disable\t-> Disables the specified relay."), true);
  printData(F("relay (pinNumber) resume\t-> Resumes operation of the specified relay."), true);
  printData(F("relay (pinNumber) desc (string)\t-> Adds a description to the specified relay."), true);
  printData(F("relay (pinNumber) starth (hour)\t-> Sets starting hour."), true);
  printData(F("relay (pinNumber) endh (hour)\t-> Sets the ending hour."), true);
  printData(F("relay (pinNumber) startm (minute)\t-> Sets the starting minute."), true);
  printData(F("relay (pinNumber) endm (minute)\t-> Sets the ending minute."), true);
  printData(F("relay (pinNumber) pin (pinNumber)\t-> Sets a new pin for the specified relay."), true);
  printData(F("relay (pinNumber) on (pinNumber)\t-> Turns on the specified relay."), true);
  printData(F("relay (pinNumber) off (pinNumber)\t-> Turns off the specified relay."), true);
}

void setCommandHelp() {
  printData(F("set hostname\t--> Set the device name."), true);
  printData(F("set ip\t\t--> Set IP address."), true);
  printData(F("set mask\t--> Set subnet mask."), true);
  printData(F("set gateway\t--> Set default gateway."), true);
  printData(F("set dns\t\t--> Set the DNS server."), true);
  printData(F("set mac\t\t--> Set the MAC address."), true);
  printData(F("set datetime\t--> Set date and time. Format yyyy/mm/dd hh:mm:ss"), true);
}

// Send data to the current interface (telnet or serial)
void printData(String data, bool carriageReturn) {
  // output variable is set to COMM_TELNET when a connection arrives. Otherwise, is set to COMM_SERIAL. 
  switch(output) {
    case COMM_SERIAL:
      if(carriageReturn) Serial.println(data);
      else Serial.print(data);
      break;
    case COMM_TELNET:
      if(carriageReturn) client.println(data);
      else client.print(data);
      break;
  }
}

// Print the prompt
void printPrompt() {
  timeOfLastActivity = millis();
  while (client.read() != -1) {};
  client.println();
  client.print(sys.hostname);
  client.print(F("#>"));
}

void printIpAddress() {
  printData(F("\nMAC:\t\t"), false);
  printData(String(sys.mac[0], HEX) + ":" +
                String(sys.mac[1], HEX) + ":" +
                String(sys.mac[2], HEX) + ":" +
                String(sys.mac[3], HEX) + ":" +
                String(sys.mac[4], HEX) + ":" +
                String(sys.mac[5], HEX), true);
                
  char ipadd[16] = "";
  sprintf(ipadd, "%d.%d.%d.%d", sys.ip[0], sys.ip[1], sys.ip[2], sys.ip[3]);
  printData(F("IP address:\t"), false);
  printData(String(ipadd), true);
  
  sprintf(ipadd, "%d.%d.%d.%d", sys.subnet[0], sys.subnet[1], sys.subnet[2], sys.subnet[3]);
  printData(F("Subnet mask:\t"), false);
  printData(String(ipadd), true);
  
  sprintf(ipadd, "%d.%d.%d.%d", sys.gateway[0], sys.gateway[1], sys.gateway[2], sys.gateway[3]);
  printData(F("Gateway:\t"), false);
  printData(String(ipadd), true);
  
  sprintf(ipadd, "%d.%d.%d.%d", sys.dns[0], sys.dns[1], sys.dns[2], sys.dns[3]);
  printData(F("DNS server:\t"), false);
  printData(String(ipadd), true);
}
