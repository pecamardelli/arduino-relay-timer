void addRelay(String source, char *_pin) {
  bool match = false;
  int pin;
  
  sscanf(_pin, "%d", &pin);
  
  // Check if pin number is out of range
  if(pin < 0 || pin > 53) {
    printData(source, F("Pin number is out of range (0-53): "), false);
    printData(source, String(pin), true);
    return;
  }
  
  // Check if given pin number is blacklisted
  for(int i=0;i<sizeof(unusablePins)/sizeof(int);i++) {
    if(pin == unusablePins[i]) {
      printData(source, "Pin " + String(pin) + " is unusable.", true);
      return;
    }
  }

  // Now, check if pin is already in use
  aux = first;
  while(aux != NULL) {
    if((int)aux->relay.pin == pin) {
      printData(source, F("Pin already in use: "), false);
      printData(source, String(pin), true);
      return;
    }
    aux = aux->next;
  }

  // And finally, we're gonna check if there's a deleted relay with
  // the given pin number. If so, we'll re-use it and allocate memory otherwise.
  aux = first;
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

  aux->relay.type       = 200;    // 200 is the standard number for a relay to be recongnized as such in the EEPROM
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
  printData(source, F("Added a new relay at pin "), false);
  printData(source, String(pin), true);
}

void checkRelays() {
  DateTime now = RTC.now();
  int startMins, endMins, currentMins;

  aux = first;
  while(aux != NULL) {
    if(aux->relay.enabled && !aux->relay.deleted && !aux->overrided) {
      startMins   = aux->relay.startHour * 60 + aux->relay.startMin;
      endMins     = aux->relay.endHour * 60 + aux->relay.endMin;
      currentMins  = now.hour() * 60 + now.minute();

      if(startMins <= currentMins) {
        if(endMins > startMins) {
          if(endMins < currentMins) {
            digitalWrite(aux->relay.pin, HIGH);
          }
          else {
            digitalWrite(aux->relay.pin, LOW);
          }
        }
        else {
          digitalWrite(aux->relay.pin, LOW);
        }
      }
      else {
        if(startMins < endMins) {
          digitalWrite(aux->relay.pin, HIGH);
        }
        else {          
          if(endMins > currentMins) {
            digitalWrite(aux->relay.pin, LOW);
          }
          else {
            digitalWrite(aux->relay.pin, HIGH);
          }
        }
      }
    }
    else if(!aux->overrided) {
      digitalWrite(aux->relay.pin, HIGH);
    }
    
    aux = aux->next;
  }
}

void deleteRelay(String source, char *_pin) {
  int pin;
  
  sscanf(_pin, "%d", &pin);

  // Check if pin number is out of range
  if(pin < 0 || pin > 53) {
    printData(source, "Pin number is out of range (0-53): " + String(pin), true);
    return;
  }
  
  aux = first;
  bool match = false;
  
  while(aux != NULL) {
    if((int)aux->relay.pin == pin) {
      aux->relay.deleted = true;
      printData(source, "Relay at pin " + String(pin) + " marked as deleted.", true);
      match = true;
      aux->changeFlag = true;
      return;
    }
    aux = aux->next;
  }

  if(!match) {
    printData(source, "No relay found at pin " + String(pin) + ".", true);
  }
}
