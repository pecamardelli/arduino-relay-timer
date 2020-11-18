void saveData(String source)
{

  printData(source, "Guardando cambios en EEPROM...", true);
  
  int q = 0;
  aux       = first;
  eeAddress = 0;

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

  printData(source, String(q) + " reles cambiados.", true);
  
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
  }
  

  /*
  File f;
  String fileName;

  if(!SD.exists(sysFolder))
  {
    SD.mkdir(sysFolder);
  }
  
  fileName = sysFolder + "hostname.txt";
  
  SD.remove(fileName);
  f = SD.open(fileName, FILE_WRITE);
  f.print(hostName);
  f.close();

  for(int i=0;i<4;i++)
  {
    fileName = sysFolder + "r" + String(i) + "enab.txt";
    SD.remove(fileName);
    f = SD.open(fileName, FILE_WRITE);
    f.print(relayEnabled[i]);
    f.close();

    fileName = sysFolder + "r" + String(i) + "desc.txt";
    SD.remove(fileName);
    f = SD.open(fileName, FILE_WRITE);
    f.print(relayDesc[i]);
    f.close();

    fileName = sysFolder + "r" + String(i) + "starth.txt";
    SD.remove(fileName);
    f = SD.open(fileName, FILE_WRITE);
    f.print(relayStartHour[i]);
    f.close();

    fileName = sysFolder + "r" + String(i) + "startm.txt";
    SD.remove(fileName);
    f = SD.open(fileName, FILE_WRITE);
    f.print(relayStartMin[i]);
    f.close();

    fileName = sysFolder + "r" + String(i) + "endh.txt";
    SD.remove(fileName);
    f = SD.open(fileName, FILE_WRITE);
    f.print(relayEndHour[i]);
    f.close();
    
    fileName = sysFolder + "r" + String(i) + "endm.txt";
    SD.remove(fileName);
    f = SD.open(fileName, FILE_WRITE);
    f.print(relayEndMin[i]);
    f.close();
  }  
  */
}
