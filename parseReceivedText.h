void parseReceivedText(String source) {
  if(textBuff.substring(0,4) == "help") {
    printHelp(source);
  }
  else if(textBuff.substring(0,4) == "time") {
    printData(source, getDate(), true);
  }
  else if(textBuff.substring(0,4) == "exit" ||
          textBuff.substring(0,4) == "quit" &&
          source == "telnet") {
    closeConnection();
  }
  else if(textBuff.substring(0,3) == "set") {
    setParam(textBuff.substring(4), source);
  }
  else if(textBuff.substring(0,9) == "relayinfo") {
    getRelayInfo(source);
  }
  else if(textBuff.substring(0,4) == "save") {
    saveData(source);
  }
  else if(textBuff.substring(0,8) == "hostname") {
    printData(source, sys.hostName, true);
  }
  else if(textBuff.substring(0,7) == "version") {
    printData(source, "ARDUINO RELAY TIMER Version " + sysVersion, true);
  }
  else if(textBuff.substring(0,2) == "ip") {
    printIpAddress(source);
  }
  else if(textBuff.substring(0,8) == "addrelay") {
    addRelay(source);
  }
  else if(textBuff.substring(0,8) == "delrelay") {
    deleteRelay(source);
  }
  else if(textBuff.substring(0,6) == "reboot") {
    digitalWrite(resetPin, LOW);
  }
  else if(textBuff.substring(0,12) == "defragEeprom") {
    // Currently unimplemented...
    saveData(source);    
  }
  else {
    printData(source, "Bad command -" + textBuff + "-", true);
  }
  textBuff = "";
}
