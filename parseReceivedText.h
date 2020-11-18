void parseReceivedText(String source) {
  if(textBuff.substring(0,4) == "help") {
    printData(source, "time\t\t\t-->\tReturns the current time.", true);
    printData(source, "relay(0-50) (on-off)\t-->\tTurns on or off a relay.", true);
    printData(source, "relays (on-off)\t\t-->\tTurns on or off all relays.", true);
    printData(source, "relayinfo\t\t-->\tShow relay status and configuration.", true);
    printData(source, "addrelay (0-50)\t\t-->\tAttaches a new relay to the given pin.", true);
    printData(source, "delrelay (0-50)\t\t-->\tDeletes the relay at the given pin.", true);
    printData(source, "set (parametro)\t\t-->\tSets a given parameter. 'set help' to see all options.", true);
    printData(source, "hostname\t\t-->\tShow host name.", true);
    printData(source, "ip\t\t\t-->\tShow IP conifg.", true);
    printData(source, "version\t\t\t-->\tShow system version.", true);
    printData(source, "save\t\t\t-->\tSave changes.", true);
    printData(source, "reboot\t\t\t-->\tRestarts the system (TODO).", true);
    printData(source, "exit\t\t\t-->\tClose connection (telnet only).", true);
    printData(source, "quit\t\t\t-->\tClose connection (telnet only).", true);
  }
  else if(textBuff.substring(0,4) == "time") {
    printData(source, getDate(), true);
  }
  else if(textBuff.substring(0,4) == "exit" ||
          textBuff.substring(0,5) == "quit" &&
          source == "telnet") {
    closeConnection();
  }
  else if(textBuff.substring(0,3) == "set") {
    setParam(textBuff.substring(4), source);
  }
  else if(textBuff.substring(0,9) == "relayinfo") {
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
        printData(source, estados[digitalRead(aux->relay.pin)] + "\t", false);

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
  else if(textBuff.substring(0,8) == "addrelay") {
    int pin = textBuff.substring(9,11).toInt();
    bool match = false;

    for(int i=0;i<sizeof(unusablePins)/sizeof(int);i++) {
      if(pin == unusablePins[i]) {
        match = true;
        break;
      }
    }

    if(match) {
      printData(source, "Pin " + String(pin) + " is unusable.", true);
      return;
    }
    
    if(pin < 0 || pin > 53) {
      printData(source, "Invalid pin number: " + String(pin) + " (0-53).", true);
    }
    else {
      aux = first;
      bool match = false;
      
      while(aux != NULL) {
        if((int)aux->relay.pin == pin) {
          match = true;
        }
        aux = aux->next;
      }
  
      if(match) {
        printData(source, "Pin already in use: " + String(pin), true);
      }
      else {

        aux = first;
        bool match = false;
        
        while(aux != NULL) {
          if(aux->relay.deleted) {
            match = true;
            break;
          }
          aux = aux->next;
        }

        if(!match) {
          aux = (node_t *)malloc(sizeof(node_t));
          aux->next = NULL; // Set to NULL to inform that it's a new list entry.
        }
        
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);

        aux->relay.type       = 200;    // 200 is the standard number to be recongnized as a relay in the EEPROM
        aux->relay.pin        = pin;
        aux->relay.enabled    = false;
        sprintf(aux->relay.desc, "%s", "No description");
        aux->relay.startHour  = 0;
        aux->relay.startMin   = 0;
        aux->relay.endHour    = 0;
        aux->relay.endMin     = 0;
        aux->relay.deleted    = false;
        aux->changeFlag       = true;
        aux->overrided        = false;

        if(aux->next == NULL) {
          if(first == NULL) {
            first = aux;
            last  = aux;
          }
          else {
            last->next  = aux;
            last        = aux;
            //last->next  = NULL;  In this case, next has been set to NULL when memory was allocated.
          }
        }
        printData(source, "Added a new relay at pin " + String(pin), true);
      }
    }
  }
  else if(textBuff.substring(0,8) == "delrelay") {
    int pin = textBuff.substring(9,11).toInt();

    aux = first;
    bool match = false;
    
    while(aux != NULL) {
      if((int)aux->relay.pin == pin) {
        aux->relay.deleted = true;
        printData(source, "Relay at pin " + String(pin) + " marked as deleted.", true);
        match = true;
        aux->changeFlag = true;
      }
      aux = aux->next;
    }

    if(!match) {
      printData(source, "No relay found at pin " + String(pin) + ".", true);
    }
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
