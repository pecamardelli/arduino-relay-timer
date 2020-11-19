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
  byte charsWaiting  = 0;
  byte charsReceived = 0;
  char *command = NULL;
  
  if(source == "telnet") {
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
      if (isalpha(c) or
          isdigit(c) or
          c == 0x20 or
          c == 0x2d or
          c == 0x2e) {
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
  }
  else if(source == "serial") {
    command = (char*) malloc (MAX_COMMAND_LEN*sizeof(char));
    while(Serial.available()) {
      c = (char)Serial.read();
      
      // Include letters, digits, space (0x20), hyphen (0x2d) and dot (0x2e).
      if (isalpha(c) or
          isdigit(c) or
          c == 0x20 or
          c == 0x2d or
          c == 0x2e) {
        command[charsReceived] = (char)c;
        charsReceived++;
        if (charsReceived >= MAX_COMMAND_LEN) break;
      }
      delay(5);
    }
    command[charsReceived] = (char)0x20;
  }

  if (charsReceived) parser(source, command);
  if (command) free(command);
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
