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
