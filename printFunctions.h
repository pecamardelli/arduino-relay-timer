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

void printSetHelp(String source) {
  printData(source, F("'set' command arguments:"), true);
  printData(source, F("relay(0-50)pin (0-50)\t\t-> Change the pin number of the given relay."), true);
  printData(source, F("relay(0-50)enable (0-1)\t\t-> Enable the given relay."), true);
  printData(source, F("relay(0-50)desc (descripcion)\t-> Set relay description."), true);
  printData(source, F("relay(0-50)starth (-23)\t\t-> Set the starting hour of the given relay."), true);
  printData(source, F("relay(0-50)startm (-59)\t\t-> Set the starting minute of the given relay."), true);
  printData(source, F("relay(0-50)endh (-23)\t\t-> Set the ending hour of the given relay."), true);
  printData(source, F("relay(0-50)endm (-59)\t\t-> Set the ending minute of the given relay."), true);
  printData(source, F("relay(0-50)resume\t\t-> Resumes the given relay."), true);
  printData(source, F("hostname\t\t\t-> Set the device name."), true);
  printData(source, F("ipadrress\t\t\t-> Set IP address."), true);
  printData(source, F("subnet\t\t\t\t-> Set subnet mask."), true);
  printData(source, F("gateway\t\t\t\t-> Set default gateway."), true);
  printData(source, F("dns\t\t\t\t-> Set the DNS server."), true);
  printData(source, F("datetime (xx:xx:xx xx/xx/xxxx)\t-> Set date and time."), true);
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
  client.flush();
  client.println();
  client.print(sys.hostname);
  client.print(F("#>"));
}

void printIpAddress(String source) {
  printData(source, F("\nMAC:\t\t"), false);
  printData(source, String(sys.mac[0], HEX) + ":" +
                String(sys.mac[1], HEX) + ":" +
                String(sys.mac[2], HEX) + ":" +
                String(sys.mac[3], HEX) + ":" +
                String(sys.mac[4], HEX) + ":" +
                String(sys.mac[5], HEX), true);
                
  char ipadd[16] = "";
  sprintf(ipadd, "%d.%d.%d.%d", sys.ip[0], sys.ip[1], sys.ip[2], sys.ip[3]);
  printData(source, F("IP address:\t"), false);
  printData(source, String(ipadd), true);
  
  sprintf(ipadd, "%d.%d.%d.%d", sys.subnet[0], sys.subnet[1], sys.subnet[2], sys.subnet[3]);
  printData(source, F("Subnet mask:\t"), false);
  printData(source, String(ipadd), true);
  
  sprintf(ipadd, "%d.%d.%d.%d", sys.gateway[0], sys.gateway[1], sys.gateway[2], sys.gateway[3]);
  printData(source, F("Gateway:\t"), false);
  printData(source, String(ipadd), true);
  
  sprintf(ipadd, "%d.%d.%d.%d", sys.dns[0], sys.dns[1], sys.dns[2], sys.dns[3]);
  printData(source, F("DNS server:\t"), false);
  printData(source, String(ipadd), true);
}
