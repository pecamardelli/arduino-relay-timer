// Generic help
void printHelp(String source) {
  printData(source, F("time\t\t\t-->\tReturns the current time."), true);
  printData(source, F("relay(0-50) (on-off)\t-->\tTurns on or off a relay."), true);
  printData(source, F("relays (on-off)\t\t-->\tTurns on or off all relays."), true);
  printData(source, F("relayinfo\t\t-->\tShow relay status and configuration."), true);
  printData(source, F("addrelay (0-50)\t\t-->\tAttaches a new relay to the given pin."), true);
  printData(source, F("delrelay (0-50)\t\t-->\tDeletes the relay at the given pin."), true);
  printData(source, F("set (parametro)\t\t-->\tSets a given parameter. 'set help' to see all options."), true);
  printData(source, F("hostname\t\t-->\tShow host name."), true);
  printData(source, F("ip\t\t\t-->\tShow IP conifg."), true);
  printData(source, F("version\t\t\t-->\tShow system version."), true);
  printData(source, F("save\t\t\t-->\tSave changes."), true);
  printData(source, F("reboot\t\t\t-->\tRestarts the system (TODO)."), true);
  printData(source, F("exit\t\t\t-->\tClose connection (telnet only)."), true);
  printData(source, F("quit\t\t\t-->\tClose connection (telnet only)."), true);
}

// Send data to the current interface (telnet or serial)
void printData(String source, String data, bool rc) {
  if(source == "serial") {
    if(rc) {
      Serial.println(data);
    }
    else {
      Serial.print(data);
    }
  }
  else if(source == "telnet") {
    if(rc) {
      client.println(data);
    }
    else {
      client.print(data);
    }
  }
}

// Print the prompt
void printPrompt() {
  timeOfLastActivity = millis();
  //client.flush();
  charsReceived = 0; // Count of characters received
  client.println("");
  client.print(">");
}

void printIpAddress(String source) {
  printData(source, "\nMAC:\t\t" +
                String(sys.mac[0], HEX) + ":" +
                String(sys.mac[1], HEX) + ":" +
                String(sys.mac[2], HEX) + ":" +
                String(sys.mac[3], HEX) + ":" +
                String(sys.mac[4], HEX) + ":" +
                String(sys.mac[5], HEX), true);
                
  char ipadd[16] = "";
  sprintf(ipadd, "%d.%d.%d.%d", sys.ip[0], sys.ip[1], sys.ip[2], sys.ip[3]);
  printData(source, "IP address:\t" + String(ipadd), true);
  
  sprintf(ipadd, "%d.%d.%d.%d", sys.subnet[0], sys.subnet[1], sys.subnet[2], sys.subnet[3]);
  printData(source, "Subnet mask:\t" + String(ipadd), true);
  
  sprintf(ipadd, "%d.%d.%d.%d", sys.gateway[0], sys.gateway[1], sys.gateway[2], sys.gateway[3]);
  printData(source, "Gateway:\t" + String(ipadd), true);
  
  sprintf(ipadd, "%d.%d.%d.%d", sys.dns[0], sys.dns[1], sys.dns[2], sys.dns[3]);
  printData(source, "DNS server:\t" + String(ipadd), true);
}
