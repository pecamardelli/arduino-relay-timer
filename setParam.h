void setParam(String param, String source)
{
  //printData(source, "-" + param.substring(0,5) + "-" + param.substring(6,13) + "-", true);
  if(param.substring(0,5) == "relay")
  {
    int num = param.substring(5, 6).toInt();

    if(num < 0 || num > 3)
    {
      printData(source, "Numero de rele invalido: " + String(num), true);
      return;
    }

    String par = param.substring(6,12);

    if(par == "enable")
    {
      int val = param.substring(13, 14).toInt();
      switch(val)
      {
        case 0: relayEnabled[num] = 0; printData(source, "Valor seteado a " + String(val), true); break;
        case 1: relayEnabled[num] = 1; printData(source, "Valor seteado a " + String(val), true); break;
        default: printData(source, "Valor invalido: " + String(val), true); return;
      }
    }
    else if(par == "descrp")
    {
      String des = param.substring(13);
      relayDesc[num] = des;
    }
    else if(par == "fromhr")
    {
      int val = param.substring(13).toInt();
      if(val >= 0 && val <= 23)
      {
        relayStartHour[num] = val;
        printData(source, "Hora de inicio del rele " + String(num) + " seteada a " + String(val), true);
      }
      else
      {
        printData(source, "Hora de inicio invalida -" + String(val) + "-", true);
        return;
      }
    }
    else if(par == "frommn")
    {
      int val = param.substring(13).toInt();
      if(val >= 0 && val <= 59)
      {
        relayStartMin[num] = val;
        printData(source, "Minuto de inicio del rele " + String(num) + " seteado a " + String(val), true);
      }
      else
      {
        printData(source, "Minuto de inicio invalido -" + String(val) + "-", true);
        return;
      }
    }
    else if(par == "thruhr")
    {
      int val = param.substring(13).toInt();
      if(val >= 0 && val <= 23)
      {
        relayEndHour[num] = val;
        printData(source, "Hora de apagado del rele " + String(num) + " seteada a " + String(val), true);
      }
      else
      {
        printData(source, "Hora de apagado invalida -" + String(val) + "-", true);
        return;
      }
    }
    else if(par == "thrumn")
    {
      int val = param.substring(13).toInt();
      if(val >= 0 && val <= 59)
      {
        relayEndMin[num] = val;
        printData(source, "Minuto de apagado del rele " + String(num) + " seteado a " + String(val), true);
      }
      else
      {
        printData(source, "Minuto de apagado invalido -" + String(val) + "-", true);
        return;
      }
    }
    else if(par == "resume")
    {
      overrided[num] = false;
    }
    else
    {
      printData(source, "Parametro desconocido: -" + par + "-", true);
    }
    //printData(source, "-" + String(num) + "-" + par + "-" + String(val) + "-", true);
  }
  else if(param.substring(0,4) == "help")
  {
    printData(source, "Opciones del comando 'set':", true);
    printData(source, "\trelay(0-3)enable (0-1)\t-> Cambia el estado del rele especificado", true);
    printData(source, "\trelay(0-3)descrp (descripcion)\t-> Establece la descripcion del rele.", true);
    printData(source, "\trelay(0-3)fromhr (-23)\t-> Establece la hora de encendido del rele.", true);
    printData(source, "\trelay(0-3)frommn (-59)\t-> Establece el minuto de encendido del rele.", true);
    printData(source, "\trelay(0-3)thruhr (-23)\t-> Establece la hora de apagado del rele.", true);
    printData(source, "\trelay(0-3)thrumn (-59)\t-> Establece el minuto de apagado del rele.", true);
    printData(source, "\thostname\t\t-> Establece nombre del sistema.", true);
  }
  else if(param.substring(0,8) == "hostname")
  {
    hostName = param.substring(9);
  }
  else
  {
    printData(source, "Parametro desconocido: -" + param + "-", true);
  }
}
