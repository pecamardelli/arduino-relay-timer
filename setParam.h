void setParam(String param, String source)
{
  //printData(source, "-" + param.substring(0,5) + "-" + param.substring(6,13) + "-", true);
  if(param.substring(0,5) == "relay")
  {
    bool match  = false;
    int num     = param.substring(5, 7).toInt();
    //printData(source, String(num) + "-" + param, true);
    aux = first;
  
    while(aux != NULL)
    {
      if(aux->relay.pin == num)
      {
        match = true;
        break;
      }
      aux = aux->next;
    }

    if(!match)
    {
      printData(source, "No se encontro un rele con pin " + String(num), true); return;
    }

    //printData(source, "Exito! " + String(num), true); return;

    if(param.substring(7,13) == "enable")
    {
      int val = param.substring(14, 15).toInt();
      switch(val)
      {
        case 0:
          aux->relay.enabled = 0;
          printData(source, "Pin " +
              String(aux->relay.pin) +
              "(" + String(aux->relay.desc) +
              ") desactivado.", true);
          aux->changeFlag = true;
          break;
        case 1:
          aux->relay.enabled = 1; 
          printData(source, "Pin " +
              String(aux->relay.pin) +
              "(" + String(aux->relay.desc) +
              ") activado.", true);
          aux->changeFlag = true;
          break;
        default: printData(source, "ERROR! Valor invalido: " + String(val), true); return;
      }
    }
    else if(param.substring(7,11) == "desc")
    {
      param.substring(12).toCharArray(aux->relay.desc, 32);
      aux->changeFlag = true;
      printData(source, "Pin " + String(aux->relay.pin) + " - Descripcion: " + String(aux->relay.desc) + ".", true);
    }
    else if(param.substring(7,13) == "starth")
    {
      int val = param.substring(14).toInt();
      if(val >= 0 && val <= 23)
      {
        aux->relay.startHour = val;
        aux->changeFlag = true;
        printData(source, "Pin " + String(aux->relay.pin) + " - Hora de inicio: " + String(val), true);
      }
      else
      {
        printData(source, "ERROR! Hora de inicio invalida -" + String(val) + "-", true);
        return;
      }
    }
    else if(param.substring(7,13) == "startm")
    {
      int val = param.substring(14).toInt();
      if(val >= 0 && val <= 59)
      {
        aux->relay.startMin = val;
        aux->changeFlag = true;
        printData(source, "Pin " + String(aux->relay.pin) + " - Minuto de inicio: " + String(val), true);
      }
      else
      {
        printData(source, "ERROR! Minuto de inicio invalido -" + String(val) + "-", true);
        return;
      }
    }
    else if(param.substring(7,11) == "endh")
    {
      int val = param.substring(12).toInt();
      if(val >= 0 && val <= 23)
      {
        aux->relay.endHour = val;
        aux->changeFlag = true;
        printData(source, "Pin " + String(aux->relay.pin) + " - Hora de apagado: " + String(val), true);
      }
      else
      {
        printData(source, "ERROR! Hora de apagado invalida -" + String(val) + "-", true);
        return;
      }
    }
    else if(param.substring(7,11) == "endm")
    {
      int val = param.substring(12).toInt();
      if(val >= 0 && val <= 59)
      {
        aux->relay.endMin = val;
        aux->changeFlag = true;
        printData(source, "Pin " + String(aux->relay.pin) + " - Minuto de apagado: " + String(val), true);
      }
      else
      {
        printData(source, "ERROR! Minuto de apagado invalido -" + String(val) + "-", true);
        return;
      }
    }
    else if(param.substring(7,13) == "resume")
    {
      aux->overrided  = false;
      printData(source, "Pin " +
              String(aux->relay.pin) +
              "(" + String(aux->relay.desc) +
              ") normalizado.", true);
    }
    else if(param.substring(7,10) == "pin")
    {
      int pin = param.substring(11,13).toInt();
      //printData(source, "PIN :" + String(pin), true);
      
      bool match = false;
  
      for(int i=0;i<sizeof(unusablePins)/sizeof(int);i++)
      {
        if(pin == unusablePins[i])
        {
          match = true;
          break;
        }
      }
  
      if(match)
      {
        printData(source, "El pin " + String(pin) + " es inusable.", true);
        return;
      }

      if(pin < 0 || pin > 53)
      {
        printData(source, "ERROR! Numero de pin fuera de rango: " + String(pin) + " (0-53).", true);
        return;
      }

      node_t * aux2;

      aux2  = first;
      match = false;
      
      while(aux2 != NULL)
      {
        if(aux2->relay.pin == pin)
        {
          match = true;
          break;
        }
        aux2 = aux2->next;
      }

      if(match)
      {
        printData(source, "ERROR! El pin " + String(pin) + " ya esta utilizado.", true);
        return;
      }

      aux->relay.pin  = pin;
      aux->changeFlag = true;
      printData(source, "Pin " + String(pin) + " cambiado correctamente (" + String(aux->relay.desc) + ")", true);
      
    }
    else if(param.substring(7,9) == "on")
    {
      if(!aux->relay.deleted)
      {
        if(digitalRead(aux->relay.pin) == HIGH)
        {
          digitalWrite(aux->relay.pin, LOW);
          aux->overrided  = true;
          aux->uptime = millis();
        }
        else
        {
          printData(source, "El rele " + String(num) + " esta encendido.", true);
        }
      }
      else
      {
        printData(source, "ERROR! El rele con pin " + String(num) + " fue eliminado.", true);
      }
    }
    else if(param.substring(7,10) == "off")
    {
      if(!aux->relay.deleted)
      {
        if(digitalRead(aux->relay.pin) == LOW)
        {
          digitalWrite(aux->relay.pin, HIGH);
          aux->overrided  = true;
          aux->uptime = 0;
        }
        else
        {
          printData(source, "El rele " + String(num) + " esta apagado.", true);
        }
      }
      else
      {
        printData(source, "ERROR! El rele con pin " + String(num) + " fue eliminado.", true);
      }
    }
    else
    {
      printData(source, "ERROR! Parametro desconocido: -" + param.substring(7) + "-", true);
    }
  }
  else if(param.substring(0,4) == "help")
  {
    printData(source, "Opciones del comando 'set':", true);
    printData(source, "\trelay(0-50)pin (0-50)\t\t-> Cambia el pin del rele especificado.", true);
    printData(source, "\trelay(0-50)enable (0-1)\t\t-> Cambia el estado del rele especificado.", true);
    printData(source, "\trelay(0-50)desc (descripcion)\t-> Establece la descripcion del rele.", true);
    printData(source, "\trelay(0-50)starth (-23)\t\t-> Establece la hora de encendido del rele.", true);
    printData(source, "\trelay(0-50)startm (-59)\t\t-> Establece el minuto de encendido del rele.", true);
    printData(source, "\trelay(0-50)endh (-23)\t\t-> Establece la hora de apagado del rele.", true);
    printData(source, "\trelay(0-50)endm (-59)\t\t-> Establece el minuto de apagado del rele.", true);
    printData(source, "\trelay(0-50)resume (-59)\t\t-> Normaliza el funcionamiento del rele especificado.", true);
    printData(source, "\thostname\t\t\t-> Establece nombre del sistema.", true);
    printData(source, "\tipadrress\t\t\t-> Establece la direccion IP del sistema.", true);
    printData(source, "\tsubnet\t\t\t\t-> Establece la mascara de red.", true);
    printData(source, "\tgateway\t\t\t\t-> Establece la puerta de enlace predeterminada.", true);
    printData(source, "\tdns\t\t\t\t-> Establece la direccion IP del servidor de DNS.", true);
    printData(source, "\tdatetime (xx:xx:xx xx/xx/xxxx)\t-> Establece la fecha y hora del sistema.", true);
  }
  else if(param.substring(0,8) == "hostname")
  {
    param.substring(9).toCharArray(sys.hostName, 32);
    sysChangeFlag = true;
    printData(source, "\nNombre de equipo seteado a: " + String(sys.hostName), true);
  }
  else if(param.substring(0,9) == "ipaddress")
  {
    //char ipa[32];
    //param.substring(10).toCharArray(ipa, 32);
    String ipp;
    ipp = param.substring(10);
    
    IPAddress addr;
    if (addr.fromString(ipp)) {
      // it was a valid address, do something with it 
      printData(source, "IP cambiada correctamente : " +
                        String(addr[0], DEC) + "." +
                        String(addr[1], DEC) + "." +
                        String(addr[2], DEC) + "." +
                        String(addr[3], DEC), true);
      printData(source, "save & reboot para aplicar los cambios.", true);
      sys.ip[0]        = addr[0];
      sys.ip[1]        = addr[1];
      sys.ip[2]        = addr[2];
      sys.ip[3]        = addr[3];
      sysChangeFlag = true;      
    }
    else
    {
      printData(source, "IP no valida : -" + ipp + "-", true);
    }
  }
  else if(param.substring(0,6) == "subnet")
  {
    char ipa[32];
    param.substring(7).toCharArray(ipa, 32);
    
    IPAddress addr;
    if (addr.fromString(ipa)) {
      // it was a valid address, do something with it 
      printData(source, "Mascara cambiada correctamente : " +
                        String(addr[0], DEC) + "." +
                        String(addr[1], DEC) + "." +
                        String(addr[2], DEC) + "." +
                        String(addr[3], DEC), true);
      printData(source, "save & reboot para aplicar los cambios.", true);
      sys.subnet[0] = addr[0];
      sys.subnet[1] = addr[1];
      sys.subnet[2] = addr[2];
      sys.subnet[3] = addr[3];
      sysChangeFlag = true;      
    }
    else
    {
      printData(source, "Mascara no valida : " + String(ipa), true);
    }
  }
  else if(param.substring(0,7) == "gateway")
  {
    char ipa[32];
    param.substring(8).toCharArray(ipa, 32);
    
    IPAddress addr;
    if (addr.fromString(ipa)) {
      // it was a valid address, do something with it 
      printData(source, "Puera de enlace cambiada correctamente : " +
                        String(addr[0], DEC) + "." +
                        String(addr[1], DEC) + "." +
                        String(addr[2], DEC) + "." +
                        String(addr[3], DEC), true);
      printData(source, "save & reboot para aplicar los cambios.", true);
      sys.gateway[0]  = addr[0];
      sys.gateway[1]  = addr[1];
      sys.gateway[2]  = addr[2];
      sys.gateway[3]  = addr[3];
      sysChangeFlag   = true;      
    }
    else
    {
      printData(source, "Puera de enlace no valida : " + String(ipa), true);
    }
  }
  else if(param.substring(0,3) == "dns")
  {
    char ipa[32];
    param.substring(4).toCharArray(ipa, 32);
    
    IPAddress addr;
    if (addr.fromString(ipa)) {
      // it was a valid address, do something with it 
      printData(source, "Servidor DNS cambiado correctamente : " +
                        String(addr[0], DEC) + "." +
                        String(addr[1], DEC) + "." +
                        String(addr[2], DEC) + "." +
                        String(addr[3], DEC), true);
      printData(source, "save & reboot para aplicar los cambios.", true);
      sys.dns[0]    = addr[0];
      sys.dns[1]    = addr[1];
      sys.dns[2]    = addr[2];
      sys.dns[3]    = addr[3];
      sysChangeFlag = true;      
    }
    else
    {
      printData(source, "Servidor DNS no valido : " + String(ipa), true);
    }
  }
  else if(param.substring(0,8) == "datetime")
  {
    int date[6];
    char str[32];
    param.substring(9, 41).toCharArray(str, 32);
    
    if (sscanf(str, "%d:%d:%d %d/%d/%d", &date[0], &date[1], &date[2], &date[3], &date[4], &date[5]) == 6)
    {
      if(date[0] < 0 || date[0] > 23)
      {
        printData(source, "ERROR! Hora fuera de rango: " + String(date[0]), true); 
        return;
      }

      if(date[1] < 0 || date[1] > 59)
      {
        printData(source, "ERROR! Minuto fuera de rango: " + String(date[1]), true); 
        return;
      }

      if(date[2] < 0 || date[2] > 59)
      {
        printData(source, "ERROR! Segundo fuera de rango: " + String(date[2]), true); 
        return;
      }

      if(date[3] >= 0 || date[3] <= 31)
      {
        if(date[3] == 31 &&
           (date[4] == 2 ||
           date[4] == 4 ||
           date[4] == 6 ||
           date[4] == 9 ||
           date[4] == 11))
        {
          printData(source, "ERROR! El mes " + String(date[4]) + " no tiene 31 dias.", true);
          return;
        }

        if(date[4] == 2 && date[3] == 29)
        {
          int x = date[5] % 4;
          if(x != 0)
          {
            printData(source, "ERROR! El anio " + String(date[5]) + " no es bisiesto.\n Febrero no puede tener 29 dias.", true);
            return;
          }
        }        
      }
      else
      {
        printData(source, "ERROR! Dia fuera de rango: " + String(date[3]), true); 
        return;
      }

      if(date[4] < 0 || date[4] > 12)
      {
        printData(source, "ERROR! Mes fuera de rango: " + String(date[4]), true); 
        return;
      }

      if(date[5] < 2018)
      {
        printData(source, "ERROR! Salvo que hayas viajado en el tiempo, no puedes estar en el anio " + String(date[5]) + ".", true); 
        return;
      }

      if(date[5] > 2099)
      {
        printData(source, "ERROR! No creo que el Arduino dure hasta mas alla del anio 2099.", true); 
        return;
      }
      
      //setTime(date[0],date[1],date[2],date[3],date[4],date[5]);
      RTC.adjust(DateTime(date[5],date[4],date[3],date[0],date[1],date[2]));

      printData(source, "Se actualizo correctamente la fecha: " + String(getDate()), true);
    }
    else
    {
      printData(source, "Fecha invalida: " + 
                String(date[0]) + ":" +
                String(date[1]) + ":" +
                String(date[2]) + " " +
                String(date[3]) + "/" +
                String(date[4]) + "/" +
                String(date[5]), true);
    }    
  }
  else
  {
    printData(source, "Parametro desconocido: -" + param + "-", true);
  }
}
