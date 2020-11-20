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

void getReceivedText() {
  char c;
  byte charsWaiting  = 0;
  byte charsReceived = 0;
  char *command = NULL;
  
  switch (output) {
    case COMM_TELNET:
      charsWaiting = client.available();    
  
      if (!charsWaiting) return;
  
      if (charsWaiting > MAX_COMMAND_LEN) {
        client.print(F("Command must have a maximum of "));
        client.print(String(MAX_COMMAND_LEN));
        client.println(F(" characters."));
        return;
      }
  
      // Looks like we have a command to parse. Let's do it.
      command = (char*) malloc (charsWaiting*sizeof(char));
      
      while(charsWaiting > 0) {
        c = client.read();
  
        // Include letters, digits, space (0x20), hyphen (0x2d) and dot (0x2e).
        if (isalpha(c) or isdigit(c) or charAllowed(c)) {
          command[charsReceived] = (char)c;
          charsReceived++;
        }
        else if (c == 0x0d) {   // Carriage return. Parse command.
          // Add space character to the end of the string in order to give strtok the ability
          // to slice it and avoid taking any garbage from memory.
          // See parser function.
          command[charsReceived] = (char)0x20;
          break;
        }
        charsWaiting--;
      }
      break;
    case COMM_SERIAL:
      command = (char*) malloc (MAX_COMMAND_LEN*sizeof(char));
      while(Serial.available()) {
        c = (char)Serial.read();
        
        // Include letters, digits, space (0x20), hyphen (0x2d) and dot (0x2e).
        if (isalpha(c) or isdigit(c) or charAllowed(c)) {
          command[charsReceived] = (char)c;
          charsReceived++;
          if (charsReceived >= MAX_COMMAND_LEN) break;
        }
        delay(5);
      }
      command[charsReceived] = (char)0x20;
      break;
  }
    
  if (charsReceived) parser(command);
  if (command) free(command);
}
