void getReceivedText()
{
  char c;
  int charsWaiting;

  // copy waiting characters into textBuff
  //until textBuff full, CR received, or no more characters

  charsWaiting = client.available();
  do {
    c = client.read();
    //textBuff[charsReceived] = c;
    textBuff += c;
    charsReceived++;
    charsWaiting--;
  }
  while(charsReceived <= textBuffSize && c != 0x0d && charsWaiting > 0);
  
  //if CR found go look at received text and execute command
  if(c == 0x0d) {
    parseReceivedText("telnet");
    // after completing command, print a new prompt
    printPrompt();
  }

  // if textBuff full without reaching a CR, print an error message
  if(charsReceived >= textBuffSize) {
    client.println("El comando debe tener como maximo 64 caracteres.");
    printPrompt();
  }
  // if textBuff not full and no CR, do nothing else;
  // go back to loop until more characters are received
}
