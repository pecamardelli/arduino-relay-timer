void loadSystemData() {
  // LOAD SYSTEM CONFIGS
  // They're stored at the end of the EEPROM.
  // Relay data is stored at the beginning.
 
  eeAddress = EEPROM.length() - sizeof(systemData) - 1;
  EEPROM.get(eeAddress, sys);

  Serial.print("Iniciando ethernet shield: ");
  Ethernet.begin(sys.mac, sys.ip, sys.dns, sys.gateway, sys.subnet);
  Serial.println("hecho.\n");

  int pos       = 0;
  relayQuantity = 0;
  eeAddress     = 0;

  // All relays are loaded into a dynamic list. This way, more relays can be added later.

  // From byte 0 to 100 is stored the "file system". Every two bytes is an integer
  // that represents a memory address where the relay data is stored on the EEPROM.
  for(int i=0;i<50;i++) {
    EEPROM.get(eeAddress, pos);
  }
  
  do {
    aux = (node_t *)malloc(sizeof(node_t));
    
    if (aux == NULL) {
        Serial.println("ERROR: Could not allocate memory. " + String(relayQuantity) + " relay(s) loaded.");
        break;
    } else {
      EEPROM.get(eeAddress, aux->relay);
  
      if(aux->relay.type == 200) {
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
        
        eeAddress += sizeof(relayData);
        relayQuantity++;
      } else {
        Serial.println("All relays loaded. Total: " + String(relayQuantity));
        break;
      }
    }
  }
  while(1);
}
