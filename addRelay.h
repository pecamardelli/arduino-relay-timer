void addRelay(String source) {
  // Obtain only two digits and ignore the rest of the string.
  //int pin = textBuff.substring(9,11).toInt();
  int pin = 45;
  bool match = false;

  // Check if pin number is out of range
  if(pin < 0 || pin > 53) {
    printData(source, "Pin number is out of range (0-53): " + String(pin), true);
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
      printData(source, "Pin already in use: " + String(pin), true);
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
  printData(source, "Added a new relay at pin " + String(pin), true);
}
