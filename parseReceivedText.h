void parseReceivedText(String source)
{
  //client.println(textBuff.substring(1,5));
  File root;
  
  if(textBuff.substring(1,5) == "help" || textBuff.substring(1,1) == "?")
  {
    printData(source, "time\t\t\t-->\tDevuelve la hora actual.", true);
    printData(source, "relay(0-3) (on-off)\t-->\tEnciende o apaga el rele indicado.", true);
    printData(source, "relays (on-off)\t\t-->\tEnciende o apaga todos los reles.", true);
    printData(source, "relayinfo\t\t-->\tMuestra el estado y la configuracion de los reles.", true);
    printData(source, "ls\t\t\t-->\tLista los archivos de la SD.", true);
    printData(source, "cat (archivo)\t\t-->\tImprime el archivo especificado.", true);
    printData(source, "del (archivo)\t\t-->\tElimina el archivo especificado.", true);
    printData(source, "set (parametro)\t\t-->\tCambiar un parametro del sistema. 'set help' para ver opciones.", true);
    printData(source, "hostname\t\t-->\tMuestra el nombre del sistema.", true);
    printData(source, "ip\t\t\t-->\tMuestra la configuracion de IP actual.", true);
    printData(source, "version\t\t\t-->\tMuestra la version actual del sistema.", true);
    printData(source, "save\t\t\t-->\tGuarda los cambios.", true);
    printData(source, "exit\t\t\t-->\tCerrar conexion (solo telnet).", true);
    printData(source, "quit\t\t\t-->\tCerrar conexion (solo telnet).", true);
  }
  else if(textBuff.substring(1,5) == "time")
  {
    printData(source, getDate(), true);
  }
  else if(textBuff.substring(1,10) == "relay0 on")
  {
    digitalWrite(relayPin[0], LOW);
    overrided[0] = true;
  }
  else if(textBuff.substring(1,11) == "relay0 off")
  {
    digitalWrite(relayPin[0], HIGH);
    overrided[0] = true;
  }
  else if(textBuff.substring(1,10) == "relay1 on")
  {
    digitalWrite(relayPin[1], LOW);
    overrided[1] = true;
  }
  else if(textBuff.substring(1,11) == "relay1 off")
  {
    digitalWrite(relayPin[1], HIGH);
    overrided[1] = true;
  }
  else if(textBuff.substring(1,10) == "relay2 on")
  {
    digitalWrite(relayPin[2], LOW);
    overrided[2] = true;
  }
  else if(textBuff.substring(1,11) == "relay2 off")
  {
    digitalWrite(relayPin[2], HIGH);
    overrided[2] = true;
  }
  else if(textBuff.substring(1,10) == "relay3 on")
  {
    digitalWrite(relayPin[3], LOW);
    overrided[3] = true;
  }
  else if(textBuff.substring(1,11) == "relay3 off")
  {
    digitalWrite(relayPin[3], HIGH);
    overrided[3] = true;
  }
  else if(textBuff.substring(1,11) == "relays off")
  {
    digitalWrite(relayPin[0], HIGH);
    overrided[0] = true;
    digitalWrite(relayPin[1], HIGH);
    overrided[1] = true;
    digitalWrite(relayPin[2], HIGH);
    overrided[2] = true;
    digitalWrite(relayPin[3], HIGH);
    overrided[3] = true;
  }
  else if(textBuff.substring(1,10) == "relays on")
  {
    digitalWrite(relayPin[0], LOW);
    overrided[0] = true;
    digitalWrite(relayPin[1], LOW);
    overrided[1] = true;
    digitalWrite(relayPin[2], LOW);
    overrided[2] = true;
    digitalWrite(relayPin[3], LOW);
    overrided[3] = true;
  }
  else if(textBuff.substring(1,5) == "exit" || textBuff.substring(1,5) == "quit" && source == "telnet")
  {
    closeConnection();
  }
  else if(textBuff.substring(1,3) == "ls")
  {
    String dir = "/";
    
    if(textBuff.length() > 4)
    {
      dir = textBuff.substring(4,textBuff.length() - 1) + "/";
    }
    //printData(source, "-" + dir + "-", true);
    root = SD.open(dir);
    
    if(root)
    {
      printData(source, "Contenido de " + dir, true);
      printDirectory(root, 0, source);
      root.close();
    }
    else
    {
      printData(source, "No existe el directorio -" + dir + "-", true);
    }
  }
  else if(textBuff.substring(1,4) == "cat")
  {
    String f = textBuff.substring(5,textBuff.length() - 1);
    //printData(source, f, true);
    printFile(f, source);
  }
  else if(textBuff.substring(1,4) == "del")
  {
    String f = textBuff.substring(5,textBuff.length() - 1);
    deleteFile(f, source);
  }
  else if(textBuff.substring(1,4) == "set")
  {
    String f = textBuff.substring(5,textBuff.length() - 1);
    //printData(source, f, true);
    setParam(f, source);
  }
  else if(textBuff.substring(1,10) == "relayinfo")
  {
    printData(source, "NRO\tENABLED\tDESC\t\tSTARTH\tSTARTM\tENDH\tENDM", true);
    for(int i=0;i<4;i++)
    {
      printData(source, String(i) + "\t", false);
      printData(source, String(relayEnabled[i]) + "\t", false);

      String tabs;
      
      if(relayDesc[i].length() <= 8)
      {
        tabs = "\t\t";
      }
      else if(relayDesc[i].length() <= 16)
      {
        tabs = "\t";
      }
      
      printData(source, relayDesc[i] + tabs, false);
      printData(source, String(relayStartHour[i]) + "\t", false);
      printData(source, String(relayStartMin[i]) + "\t", false);
      printData(source, String(relayEndHour[i]) + "\t", false);
      printData(source, String(relayEndMin[i]) + "\t", true);
    }
    
  }
  else if(textBuff.substring(1,5) == "save")
  {
    saveData();
  }
  else if(textBuff.substring(1,9) == "hostname")
  {
    printData(source, hostName, true);
  }
  else if(textBuff.substring(1,8) == "version")
  {
    printData(source, "ARDUINO RELAY TIMER Version " + sysVersion, true);
  }
  else if(textBuff.substring(1,3) == "ip")
  {
    //Serial.write((long)mac);
   printData(source, "MAC address:\t" + arrayToString(mac, 6), true);
   char ipadd[16] = "";
   sprintf(ipadd, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
   printData(source, "IP address:\t" + String(ipadd), true);
   
   sprintf(ipadd, "%d.%d.%d.%d", subnet[0], subnet[1], subnet[2], subnet[3]);
   printData(source, "Subnet mask:\t" + String(ipadd), true);
   
   sprintf(ipadd, "%d.%d.%d.%d", gateway[0], gateway[1], gateway[2], gateway[3]);
   printData(source, "Gateway:\t" + String(ipadd), true);
   
   sprintf(ipadd, "%d.%d.%d.%d", dns[0], dns[1], dns[2], dns[3]);
   printData(source, "DNS server:\t" + String(ipadd), true);
  }
  else
  {
    printData(source, "Comando no reconocido.", true);
  }
  textBuff = "";
}

