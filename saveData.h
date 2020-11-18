void saveData(String source) {
  printData(source, F("Saving changes to EEPROM..."), true);
  
  int q = 0;
  aux       = first;
  eeAddress = 0;

  while(aux != NULL) {
    if(aux->changeFlag) {
      EEPROM.put(eeAddress, aux->relay);
      aux->changeFlag = false;
      q++;
    }
    eeAddress += sizeof(relayData);
    aux = aux->next;
  }

  printData(source, String(q) + " relays updated.", true);
  
  if(sysChangeFlag) {
    eeAddress = EEPROM.length() - sizeof(systemData) - 1;
    /*
     * byte mac[] =     { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    sys.mac[0] = 0x88;
    sys.mac[1] = 0xD7;
    sys.mac[2] = 0xF6;
    sys.mac[3] = 0x3D;
    sys.mac[4] = 0xFE;
    sys.mac[5] = 0xED;
    */
    EEPROM.put(eeAddress, sys);
    printData(source, F("System data saved."), true);
    sysChangeFlag = false;
  }
}
