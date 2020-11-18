String getDate() {
  DateTime now = RTC.now();
  return  String(now.hour()) + ":" +
          String(now.minute()) + ":" +
          String(now.second()) + " " +
          days[now.dayOfTheWeek()] + " " +
          String(now.day()) + "/" +
          String(now.month()) + "/" +
          String(now.year());
}

void getReceivedText(String source) {
  char c;
  int charsWaiting  = 0;
  const int textBuffSize          = 64;
  //char textBuffer[textBuffSize] = "";
  //charsReceived = 0;

  // copy waiting characters into textBuff
  //until textBuff full, CR received, or no more characters

  if(source == "telnet") {
    charsWaiting = client.available();
    do {
      c = client.read();
      //textBuff[charsReceived] = c;
      if(c != 0x0d && charsReceived > 0) {
        textBuff += (char)c;
      }
      charsReceived++;
      charsWaiting--;     
    }
    while (charsReceived <= textBuffSize && c != 0x0d && charsWaiting > 0);
    
    //if CR found go look at received text and execute command
    if(c == 0x0d) {
      //textBuffer[charsReceived - 1] = '\0'; 
      parseReceivedText("telnet");
      // after completing command, print a new prompt
      printPrompt();
    }
  
    // if textBuff full without reaching a CR, print an error message
    if(charsReceived >= textBuffSize) {
      client.println("Command must have a maximum of 64 characters.");
      printPrompt();
    }
      
    // if textBuff not full and no CR, do nothing else;
    // go back to loop until more characters are received
  }
  else if(source == "serial") {
    while(Serial.available()) {
      //textBuffer[charsReceived] = (char)Serial.read();
      c = (char)Serial.read();
      textBuff += c;
      delay(5);
      charsReceived++;
    }
    parseReceivedText(source);
  }  
}

void getRelayInfo(String source) {
  printData(source, "PIN\tENABLED\tDESCRIPTION\t\tSTARTH\tSTARTM\tENDH\tENDM\tSTATUS", true);  
  aux = first;
  String var = "";

  while(aux != NULL) {
    if(!aux->relay.deleted) {
      printData(source, String(aux->relay.pin) + "\t", false);
      printData(source, String(aux->relay.enabled) + "\t", false);
      printData(source, String(aux->relay.desc), false);
      
      var = String(aux->relay.desc);

      if(var.length() < 8) {
        printData(source, "\t\t\t", false);
      }
      else if(var.length() < 16) {
        printData(source, "\t\t", false);
      }
      else {
        printData(source, "\t", false);
      }
      
      printData(source, String(aux->relay.startHour) + "\t", false);
      printData(source, String(aux->relay.startMin) + "\t", false);
      printData(source, String(aux->relay.endHour) + "\t", false);
      printData(source, String(aux->relay.endMin) + "\t", false);
      printData(source, statuses[digitalRead(aux->relay.pin)] + "\t", false);

      if(aux->changeFlag) {
        printData(source, "*", true);
      }
      else {
        printData(source, "", true);
      }
      //printData(source, String(aux->relay.deleted) + "\t", true);
    }
    aux = aux->next;
  }
}
