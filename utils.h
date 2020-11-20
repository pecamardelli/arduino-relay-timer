String parseUpTime(unsigned long startTime) {
  unsigned long diff = millis() - startTime;
  int _days = 0;
  int _hours = 0;
  int _mins = 0;
  int _secs = 0;

  while(diff >= 84600000) {
    diff -= 84600000;
    _days++;
  }

  while(diff >= 3600000){
    diff -= 3600000;
    _hours++;
  }

  while(diff >= 60000){
    diff -= 60000;
    _mins++;
  }

  while(diff >= 1000){
    diff -= 1000;
    _secs++;
  }

  String d = (_days < 10) ? "0" + String(_days) : String(_days);
  String h = (_hours < 10) ? "0" + String(_hours) : String(_hours);
  String m = (_mins < 10) ? "0" + String(_mins) : String(_mins);
  String s = (_secs < 10) ? "0" + String(_secs) : String(_secs);
  
  return d + "d " + h + ":" + m + ":" + s;
}

bool charAllowed(char c) {
  for(byte i=0; i<sizeof(specialChars)/sizeof(byte);i++) {
    if (c == specialChars[i]) return true;
  }
  return false;
}

void clearArgs(char **args) {
  for(byte i=0; i<MAX_COMMAND_ARGS; i++) {
    args[i] = "";
  }
}

void loadSystemData() {
  // LOAD SYSTEM CONFIGURATIONS
  // They're stored at the end of the EEPROM.
  // Relay data is stored at the beginning.
 
  eeAddress = EEPROM.length() - sizeof(systemData) - 1;
  EEPROM.get(eeAddress, sys);

  // Set a default mac address if it's not defined.
  if(sys.mac[0] == 0xff) sys.mac[0] = 0xDE;
  if(sys.mac[1] == 0xff) sys.mac[1] = 0xAD;
  if(sys.mac[2] == 0xff) sys.mac[2] = 0xBE;
  if(sys.mac[3] == 0xff) sys.mac[3] = 0xEF;
  if(sys.mac[4] == 0xff) sys.mac[4] = 0xF0;
  if(sys.mac[5] == 0xff) sys.mac[5] = 0x18;
  
  Serial.print(F("Starting ethernet shield: "));
  Ethernet.begin(sys.mac, sys.ip, sys.dns, sys.gateway, sys.subnet);
  Serial.println(F("done."));

  byte pos      = 0;
  relayQuantity = 0;
  eeAddress     = 0;

  // All relays are loaded into a dynamic list. This way, more relays can be added later.

  // From byte 0 to 100 is stored the "file system". Every two bytes is an integer
  // that represents a memory address where the relay data is stored on the EEPROM.
  for(byte i=0;i<MAX_RELAY_NUMBER;i++) {
    EEPROM.get(eeAddress, pos);
  }
  
  do {
    node_t *aux = (node_t *)malloc(sizeof(node_t));
    
    if (!aux) {
        Serial.print(F("ERROR: Could not allocate memory. "));
        Serial.print(String(relayQuantity));
        Serial.println(F(" relay(s) loaded."));
        break;
    } else {
      EEPROM.get(eeAddress, aux->relay);
  
      if(aux->relay.type == 200) {
        if (!aux->relay.deleted) {
          if(first == NULL) {
            first       = aux;
            first->next = NULL;
            last        = first;
          } else {
            last->next  = aux;
            last        = aux;
            last->next  = NULL;
          }
    
          pinMode(aux->relay.pin, OUTPUT);
          digitalWrite(aux->relay.pin, HIGH);
          aux->changeFlag = false;
          aux->overrided  = false;
    
          relayQuantity++;
        }
        else free(aux);

        eeAddress += sizeof(relayData);
      } else {
        Serial.print(F("All relays loaded. Total: "));
        Serial.println(String(relayQuantity));
        break;
      }
    }
  }
  while(1);
}

String arrayToString(byte array[], unsigned int len) {
  char buffer[20] = "";
  
  for (unsigned int i = 0; i < len; i++) {
      byte nib1 = (array[i] >> 4) & 0x0F;
      byte nib2 = (array[i] >> 0) & 0x0F;
      buffer[i*3+0] = nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA;
      buffer[i*3+1] = nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA;
      
      if(i < len-1) {
        buffer[i*3+2] = ':';
      }
  }
  
  buffer[len*3] = '\0';
  return String(buffer);
}

void checkConnectionTimeout() {
  if(millis() - timeOfLastActivity > allowedConnectTime) {
    client.println();
    client.println(F("Timeout disconnect."));
    client.stop();
    connectFlag = 0;
  }
}

void closeConnection() {
  client.println(F("\nBye.\n"));
  client.stop();
  connectFlag = 0;
  output = COMM_SERIAL;
}
