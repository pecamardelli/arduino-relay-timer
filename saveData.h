void saveData(String source)
{
  int q = 0;
  aux         = first;
  eeAddress   = 0;
  bool saved  = false;

  while(aux != NULL)
  {
    if(aux->changeFlag)
    {
      EEPROM.put(eeAddress, aux->relay);
      aux->changeFlag = false;
      q++;
    }
    eeAddress += sizeof(relayData);
    aux = aux->next;
  }

  if(q > 0)
  {
    printData(source, String(q) + " reles guardados.", true);
  }
  
  if(sysChangeFlag)
  {
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
    printData(source, "Informacion de sistema guardada.", true);
    sysChangeFlag = false;
    saved = true;
  }

  if(q == 0 && !saved)
  {
    printData(source, "No hay cambios para guardar.", true);
  }
}
