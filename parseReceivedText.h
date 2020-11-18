void parseReceivedText(String source, char *command) {
  // Slicing arguments
  char *args[MAX_COMMAND_ARGS] = {""};
  char *token = strtok(command, " ");
  byte index = 0;

  while(token != NULL) {
    args[index] = token;
    index++;
    token = strtok(NULL, " ");
  }

  printData(source, "", true); 
  
  if(strncmp(args[0], "help", 4) == 0) {
    printHelp(source);
  }
  else if(strncmp(args[0], "time", 4) == 0) {
    printData(source, getDate(), true);
  }
  else if(strncmp(args[0], "exit", 4) == 0 ||
          strncmp(args[0], "quit", 4) == 0 &&
          source == "telnet") {
    closeConnection();
  }
  else if(strncmp(args[0], "set", 3) == 0) {
    //setParam(textBuff.substring(4), source);
  }
  else if(strncmp(args[0], "relayinfo", 9) == 0) {
    getRelayInfo(source);
  }
  else if(strncmp(args[0], "save", 4) == 0) {
    saveData(source);
  }
  else if(strncmp(args[0], "hostname", 8) == 0) {
    printData(source, sys.hostName, true);
  }
  else if(strncmp(args[0], "version", 7) == 0) {
    printData(source, "ARDUINO RELAY TIMER Version " + sysVersion, true);
  }
  else if(strncmp(args[0], "ip", 2) == 0) {
    printIpAddress(source);
  }
  else if(strncmp(args[0], "addrelay", 8) == 0) {
    addRelay(source);
  }
  else if(strncmp(args[0], "delrelay", 8) == 0) {
    deleteRelay(source);
  }
  else if(strncmp(args[0], "reboot", 6) == 0) {
    digitalWrite(resetPin, LOW);
  }
  else if(strncmp(args[0], "defragEeprom", 12) == 0) {
    // Currently unimplemented...
    printData(source, F("Feature not implemented"), true);
  }
  else {
    printData(source, F("Bad command \""), false);
    printData(source, args[0], false);
    printData(source, F("\""), true);
  }
}
