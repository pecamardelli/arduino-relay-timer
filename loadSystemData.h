void loadSystemData()
{
  File f;
  String c = "", fileName;

  if(!SD.exists(sysFolder))
  {
    Serial.print("La carpeta de sistema " + sysFolder + " no existe... Creandola: ");
    SD.mkdir(sysFolder);
    Serial.println("hecho.\n");
  }
  
  Serial.print("Seteando los pines de reles: ");
  pinMode(relayPin[0], OUTPUT);              // Setear el pin del relé como salida
  digitalWrite(relayPin[0], HIGH);            // Relé abierto por defecto
  pinMode(relayPin[1], OUTPUT);
  digitalWrite(relayPin[1], HIGH);
  pinMode(relayPin[2], OUTPUT);
  digitalWrite(relayPin[2], HIGH);
  pinMode(relayPin[3], OUTPUT);
  digitalWrite(relayPin[3], HIGH);
  Serial.println("hecho.\n");
  
  //------ ARCHIVO HOSTNAME -------//

  fileName = sysFolder + "hostname.txt";
  c = "";
  
  if(SD.exists(fileName))
  {
    f = SD.open(fileName); 
    if(f)
    {
      while (f.available()) {
        c += (char)f.read();
      }
      
      if(c == "")
      {
        hostName = "ARDUINO";
      }
      else
      {
        hostName = c;
      }
      
      f.close();
    }
  }
  else
  {
    f = SD.open(fileName, FILE_WRITE);
    if(f)
    {
      f.print("ARDUINO");
      f.close();
    }
    else
    {
      Serial.println("No se pudo crear " + fileName);
    }
    hostName = "ARDUINO";
  }
    
  for(int i=0;i<4;i++)
  {
    //------ ARCHIVO ENABLED DE LOS RELÉS -------//
    
    fileName = sysFolder + "r" + String(i) + "enab.txt";
    c = "";
    
    if(SD.exists(fileName))
    {
      f = SD.open(fileName); 
      if(f)
      {
        while (f.available()) {
          c += (char)f.read();
        }
        
        if(c.substring(0,1) == "1")
        {
          relayEnabled[i] = 1;
        }
        else
        {
          relayEnabled[i] = 0;
        }
        f.close();
      }
    }
    else
    {
      f = SD.open(fileName, FILE_WRITE);
      if(f)
      {
        f.print("0");
        f.close();
      }
      else
      {
        Serial.println("No se pudo abrir " + fileName);
      }
      relayEnabled[i] = 0;
    }

    //------ ARCHIVO DESCRIPTION DE LOS RELÉS -------//
    
    fileName = sysFolder + "r" + String(i) + "desc.txt";
    c = "";
    
    if(SD.exists(fileName))
    {
      f = SD.open(fileName); 
      if(f)
      {
        while (f.available())
        {
          c += (char)f.read();
          //Serial.write(f.read());
        }
        
        relayDesc[i]  = c;
        
        f.close();
      }
    }
    else
    {
      f = SD.open(fileName, FILE_WRITE);
      if(f)
      {
        f.print("Sin descripcion");
        f.close();
      }
      else
      {
        Serial.println("No se pudo abrir " + fileName);
      }
      relayDesc[i] = "Sin descripcion";
    }

    //------ ARCHIVO STARTHOUR DE LOS RELÉS -------//
    
    fileName = sysFolder + "r" + String(i) + "starth.txt";
    c = "";
    
    if(SD.exists(fileName))
    {
      f = SD.open(fileName); 
      if(f)
      {
        while (f.available()) {
          c += (char)f.read();
        }

        int h = c.toInt();

        if(h >= 0 && h <= 23)
        {
          relayStartHour[i] = h;
        }
        else
        {
          relayStartHour[i] = 0;
        }
        
        f.close();
      }
    }
    else
    {
      f = SD.open(fileName, FILE_WRITE);
      if(f)
      {
        f.print("0");
        f.close();
      }
      else
      {
        Serial.println("No se pudo abrir " + fileName);
      }
      relayStartHour[i] = 0;
    }

    //------ ARCHIVO STARTMIN DE LOS RELÉS -------//
    
    fileName = sysFolder + "r" + String(i) + "startm.txt";
    c = "";
    
    if(SD.exists(fileName))
    {
      f = SD.open(fileName); 
      if(f)
      {
        while (f.available()) {
          c += (char)f.read();
        }

        int m = c.toInt();

        if(m >= 0 && m <= 59)
        {
          relayStartMin[i] = m;
        }
        else
        {
          relayStartMin[i] = 0;
        }
        
        f.close();
      }
    }
    else
    {
      f = SD.open(fileName, FILE_WRITE);
      if(f)
      {
        f.print("0");
        f.close();
      }
      else
      {
        Serial.println("No se pudo abrir " + fileName);
      }
      relayStartMin[i] = 0;
    }

    //------ ARCHIVO ENDHOUR DE LOS RELÉS -------//
    
    fileName = sysFolder + "r" + String(i) + "endh.txt";
    c = "";
    
    if(SD.exists(fileName))
    {
      f = SD.open(fileName); 
      if(f)
      {
        while (f.available()) {
          c += (char)f.read();
        }

        int h = c.toInt();

        if(h >= 0 && h <= 23)
        {
          relayEndHour[i] = h;
        }
        else
        {
          relayEndHour[i] = 0;
        }
        
        f.close();
      }
    }
    else
    {
      f = SD.open(fileName, FILE_WRITE);
      if(f)
      {
        f.print("0");
        f.close();
      }
      else
      {
        Serial.println("No se pudo abrir " + fileName);
      }
      relayEndHour[i] = 0;
    }

    //------ ARCHIVO ENDMIN DE LOS RELÉS -------//
    
    fileName = sysFolder + "r" + String(i) + "endm.txt";
    c = "";
    
    if(SD.exists(fileName))
    {
      f = SD.open(fileName); 
      if(f)
      {
        while (f.available()) {
          c += (char)f.read();
        }

        int m = c.toInt();

        if(m >= 0 && m <= 59)
        {
          relayEndMin[i] = m;
        }
        else
        {
          relayEndMin[i] = 0;
        }
        
        f.close();
      }
    }
    else
    {
      f = SD.open(fileName, FILE_WRITE);
      if(f)
      {
        f.print("0");
        f.close();
      }
      else
      {
        Serial.println("No se pudo abrir " + fileName);
      }
      relayEndMin[i] = 0;
    }

  }
}
