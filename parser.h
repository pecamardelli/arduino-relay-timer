void parser(String source, char *command) {
  // Tokenizing arguments
  char *args[MAX_COMMAND_ARGS];
  clearArgs(args);
  
  char *token = strtok(command, " ");
  byte index = 0;

  while(token != NULL) {
    args[index] = token;
    index++;
    token = strtok(NULL, " ");
  }
  
  if(strncmp(args[0], "help", 4) == 0) printHelp(source);
  else if(strncmp(args[0], "time", 4) == 0) printData(source, getDate(), true);
  else if(strncmp(args[0], "exit", 4) == 0 ||
          strncmp(args[0], "quit", 4) == 0 &&
          source == "telnet") {
    closeConnection();
  }
  else if(strncmp(args[0], "set", 3) == 0) {
    if(strncmp(args[1], "help", 4) == 0) printSetHelp(source);
    if(strncmp(args[1], "hostname", 8) == 0) setHostname(source, args[2]);  // By default, args[2] is an empty string. setHostname will deal with it.
    if(strncmp(args[1], "ip", 2) == 0) setAddress(source, args[2], IP_ADDRESS);
    if(strncmp(args[1], "mask", 2) == 0) setAddress(source, args[2], SUBNET_MASK);
    if(strncmp(args[1], "gateway", 7) == 0) setAddress(source, args[2], DEFAULT_GATEWAY);
    if(strncmp(args[1], "dns", 3) == 0) setAddress(source, args[2], DNS_SERVER);
  }
  else if(strncmp(args[0], "relay", 5) == 0) {
    if(strncmp(args[1], "info", 4) == 0) getRelayInfo(source);
    if(strncmp(args[1], "add", 3) == 0) addRelay(source, args[2]);
    if(strncmp(args[1], "delete", 6) == 0) deleteRelay(source, args[2]);
  }
  else if(strncmp(args[0], "show", 4) == 0) {
    if(strncmp(args[1], "relays", 6) == 0) getRelayInfo(source);
    if(strncmp(args[1], "ip", 2) == 0) printIpAddress(source);
    if(strncmp(args[1], "version", 7) == 0) {
      printData(source, F("ARDUINO RELAY TIMER Version "), false);
      printData(source, sysVersion, true);
    }
  }
  else if(strncmp(args[0], "save", 4) == 0) saveData(source);
  else if(strncmp(args[0], "hostname", 8) == 0) printData(source, sys.hostname, true);
  else if(strncmp(args[0], "reboot", 6) == 0) digitalWrite(sys.resetPin, LOW);
  else if(strncmp(args[0], "defragEeprom", 12) == 0) printData(source, F("Feature not implemented"), true);
  else {
    printData(source, F("Bad command \""), false);
    printData(source, args[0], false);
    printData(source, F("\""), true);
  }

  
}
