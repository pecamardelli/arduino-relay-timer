byte checkPin(char *_pin) {
  int pin;
  sscanf(_pin, "%d", &pin);
  
  // Check if pin number is out of range
  if(pin < 0 || pin > 53) {
    printData(F("Pin number is out of range (0-53): "), false);
    printData(String(pin), true);
    return NULL;
  }

  // Check if given pin number is blacklisted
  for(int i=0;i<sizeof(unusablePins)/sizeof(byte);i++) {
    if(pin == unusablePins[i]) {
      printData(F("Pin is blacklisted."), true);
      return NULL;
    }
  }

  return (byte)pin;
}

void setRelayStatus(byte pin, bool _status) {
  node_t *target = searchRelay(pin);
  if(target) {
    target->relay.enabled = _status;
    printData(F("Relay "), false);
    printData(String(pin), false);
    printData(F(" enabled."), true);
  }
}

node_t *searchRelay(byte pin) {
  if(!pin) return NULL;
  
  node_t *aux = first;
  while(aux != NULL) {
    if(aux->relay.pin == pin) {
      return aux;
    }
    aux = aux->next;
  }
  
  printData(F("No relay found at pin "), false);
  printData(String(pin), true);
  
  return NULL;
}

void createRelay(byte pin) {
  if(searchRelay(pin)) {
    printData(F("Pin already in use: "), false);
    printData(String(pin), true);
    return;
  }
  
  node_t *aux = (node_t *)malloc(sizeof(node_t));
  aux->next = NULL; // Set to NULL to inform that it's a new list entry.
  
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
  printData(F("Added a new relay at pin "), false);
  printData(String(pin), true);
}

void checkRelays() {
  DateTime now = RTC.now();
  int startMins, endMins, currentMins;

  node_t *aux = first;
  while(aux != NULL) {
    if(aux->relay.enabled && !aux->relay.deleted && !aux->overrided) {
      startMins   = aux->relay.startHour * 60 + aux->relay.startMin;
      endMins     = aux->relay.endHour * 60 + aux->relay.endMin;
      currentMins  = now.hour() * 60 + now.minute();

      if(startMins <= currentMins) {
        if(endMins > startMins) {
          if(endMins < currentMins) {
            //digitalWrite(aux->relay.pin, HIGH);
            switchRelay(aux->relay.pin, HIGH, false);
          }
          else {
            //digitalWrite(aux->relay.pin, LOW);
            switchRelay(aux->relay.pin, LOW, false);
          }
        }
        else {
          //digitalWrite(aux->relay.pin, LOW);
          switchRelay(aux->relay.pin, LOW, false);
        }
      }
      else {
        if(startMins < endMins) {
          //digitalWrite(aux->relay.pin, HIGH);
          switchRelay(aux->relay.pin, HIGH, false);
        }
        else {          
          if(endMins > currentMins) {
            //digitalWrite(aux->relay.pin, LOW);
            switchRelay(aux->relay.pin, LOW, false);
          }
          else {
            //digitalWrite(aux->relay.pin, HIGH);
            switchRelay(aux->relay.pin, HIGH, false);
          }
        }
      }
    }
    else if(!aux->overrided) {
      //digitalWrite(aux->relay.pin, HIGH);
      switchRelay(aux->relay.pin, HIGH, false);
    }
    
    aux = aux->next;
  }
}

void deleteRelay(byte pin) {
  node_t *target = searchRelay(pin);

  if (target) {
      target->relay.deleted = true;
      printData(F("Relay successfully marked as deleted"), true);
  }
}

void setRelayDesc(byte pin, char *_desc) {
  node_t *target = searchRelay(pin);

  if (target) {
    if (strlen(_desc) <= RELAY_DESC_LEN) strcpy(target->relay.desc, _desc);
    else strncpy(target->relay.desc, _desc, RELAY_DESC_LEN);
    
    target->changeFlag = true;
    printData(F("Description set to: "), false);
    printData(target->relay.desc, true);
  }
}

void setRelayParams(byte pin, char *_param, byte type) {
  node_t *target = searchRelay(pin);
  int param;
  sscanf(_param, "%d", &param);

  if (target) {
    switch (type) {
      case TYPE_START_HOUR:
          if(param >= 0 && param <= 23) {
            target->relay.startHour = param;
            target->changeFlag = true;
            printData(F("Starting hour set to: "), false);
            printData(String(param), true);
          }
          else {
            printData(F("ERROR! Invalid starting hour \""), false);
            printData(String(param) + "\"", true);
            return;
          }
        break;
      case TYPE_END_HOUR:
          if(param >= 0 && param <= 23) {
            target->relay.endHour = param;
            target->changeFlag = true;
            printData("Pin " + String(pin), false);
            printData(F(": ending hour set to: "), false);
            printData(String(param), true);
          }
          else {
            printData(F("ERROR! Invalid ending hour \""), false);
            printData(String(param) + "\"", true);
            return;
          }
        break;
      case TYPE_START_MIN:
          if(param >= 0 && param <= 59) {
            target->relay.startMin = param;
            target->changeFlag = true;
            printData("Pin " + String(pin), false);
            printData(F(": starting minute set to: "), false);
            printData(String(param), true);
          }
          else {
            printData(F("ERROR! Invalid starting minute \""), false);
            printData(String(param) + "\"", true);
            return;
          }
        break;
      case TYPE_END_MIN:
          if(param >= 0 && param <= 59) {
            target->relay.endMin = param;
            target->changeFlag = true;
            printData("Pin " + String(pin), false);
            printData(F(": ending minute set to: "), false);
            printData(String(param), true);
          }
          else {
            printData(F("ERROR! Invalid ending minute \""), false);
            printData(String(param) + "\"", true);
            return;
          }
        break;
      default:
        printData(F("ERROR! Invalid param type \""), true);
    }
  }
}

void resumeRelay(byte pin) {
  node_t *target = searchRelay(pin);

  if (target) {
    target->overrided  = false;
    printData("Relay " + String(pin), false);
    printData(F(" resumed."), true);
  }
}

void changeRelayPin(byte currentPin, char *_newPin) {
  // Check if the new pin is valid
  const byte newPin = checkPin(_newPin);
  // Now check if someone else has it.
  node_t *isUsed = searchRelay(newPin);

  if (isUsed) return printData(F("Error: new pin already in use."), true);
  
  // Looks like we're good.
  node_t *target = searchRelay(currentPin);

  if (target) {
    target->relay.pin  = newPin;
    target->changeFlag = true;
    printData(F("New pin successfully changed to: "), false);
    printData(String(newPin), true);
  }
}

void switchRelay(byte pin, uint8_t state, bool overrided) {
  node_t *target = searchRelay(pin);

  if (target) {
    if (digitalRead(target->relay.pin) == HIGH && state == LOW) target->relay.startTime = millis();
    if (digitalRead(target->relay.pin) == LOW && state == HIGH) target->relay.startTime = 0;
    
    digitalWrite(target->relay.pin, state);
    target->overrided   = overrided;
  }
}

void getRelayInfo() {
  printData(F("PIN\tENABLED\tDESCRIPTION\t\SWITCH ON\tSWITCH OFF\tSTATUS\tUPTIME"), true);  
  node_t *aux = first;

  while(aux != NULL) {
    if(!aux->relay.deleted) {
      printData(String(aux->relay.pin) + "\t", false);

      const char *_enabled = (aux->relay.enabled) ? "true" : "false";
      printData(_enabled, false);
      printData("\t", false);
      printData(aux->relay.desc, false);

      const byte *descLen = strlen(aux->relay.desc);
      
      if (descLen < 8) printData("\t\t\t", false);
      else if (descLen < 16) printData("\t\t", false);
      else printData("\t", false);

      String starth = (aux->relay.startHour < 10) ? "0" + String(aux->relay.startHour) : String(aux->relay.startHour);
      String startm = (aux->relay.startMin < 10) ? "0" + String(aux->relay.startMin) : String(aux->relay.startMin);
      printData( starth + ":" + startm + "\t", false);

      String endh = (aux->relay.endHour < 10) ? "0" + String(aux->relay.endHour) : String(aux->relay.endHour);
      String endm = (aux->relay.endMin < 10) ? "0" + String(aux->relay.endMin) : String(aux->relay.endMin);
      printData(endh + ":" + endm + "\t", false);

      const char *_mode = digitalRead(aux->relay.pin) ? "off" : "on";
      printData(_mode, false);
      printData("\t", false);

      if (aux->relay.startTime) printData(parseUpTime(aux->relay.startTime), false);
      else printData(F("--d --:--:--"), false);
      
      printData("\t", false);
      
      if (aux->changeFlag) printData("*", true);
      else printData("", true);
    }
    aux = aux->next;
  }
}
