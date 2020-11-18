void setParam(String param, String source) {
  //printData(source, "-" + param.substring(0,5) + "-" + param.substring(6,13) + "-", true);
  if(param.substring(0,5) == "relay") {
    bool match  = false;
    int num     = param.substring(5, 7).toInt();
    //printData(source, String(num) + "-" + param, true);
    
    aux = first;
    while(aux != NULL) {
      if(aux->relay.pin == num) {
        match = true;
        break;
      }
      aux = aux->next;
    }

    if(!match) {
      printData(source, "No relay found at pin " + String(num), true);
      return;
    }

    //printData(source, "Success! " + String(num), true); return;

    if(param.substring(7,13) == "enable") {
      int val = param.substring(14, 15).toInt();
      switch(val) {
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
        default: printData(source, "ERROR! Invalid value: " + String(val), true); return;
      }
    }
    else if(param.substring(7,11) == "desc") {
      param.substring(12).toCharArray(aux->relay.desc, 32);
      aux->changeFlag = true;
      printData(source, "Pin " + String(aux->relay.pin) + " - Description: " + String(aux->relay.desc) + ".", true);
    }
    else if(param.substring(7,13) == "starth") {
      int val = param.substring(14).toInt();
      if(val >= 0 && val <= 23) {
        aux->relay.startHour = val;
        aux->changeFlag = true;
        printData(source, "Pin " + String(aux->relay.pin) + " - Starting hour: " + String(val), true);
      }
      else {
        printData(source, "ERROR! Invalid starting hour -" + String(val) + "-", true);
        return;
      }
    }
    else if(param.substring(7,13) == "startm") {
      int val = param.substring(14).toInt();
      if(val >= 0 && val <= 59) {
        aux->relay.startMin = val;
        aux->changeFlag = true;
        printData(source, "Pin " + String(aux->relay.pin) + " - Starting minute: " + String(val), true);
      }
      else {
        printData(source, "ERROR! Invalid starting minute -" + String(val) + "-", true);
        return;
      }
    }
    else if(param.substring(7,11) == "endh") {
      int val = param.substring(12).toInt();
      if(val >= 0 && val <= 23) {
        aux->relay.endHour = val;
        aux->changeFlag = true;
        printData(source, "Pin " + String(aux->relay.pin) + " - Stopping hour: " + String(val), true);
      }
      else {
        printData(source, "ERROR! Invalid stopping hour -" + String(val) + "-", true);
        return;
      }
    }
    else if(param.substring(7,11) == "endm") {
      int val = param.substring(12).toInt();
      if(val >= 0 && val <= 59) {
        aux->relay.endMin = val;
        aux->changeFlag = true;
        printData(source, "Pin " + String(aux->relay.pin) + " - Stopping minute: " + String(val), true);
      }
      else {
        printData(source, "ERROR! Invalid stopping minute -" + String(val) + "-", true);
        return;
      }
    }
    else if(param.substring(7,13) == "resume") {
      aux->overrided  = false;
      printData(source, "Pin " +
              String(aux->relay.pin) +
              "(" + String(aux->relay.desc) +
              ") normalizado.", true);
    }
    else if(param.substring(7,10) == "pin") {
      int pin = param.substring(11,13).toInt();
      //printData(source, "PIN :" + String(pin), true);
      
      bool match = false;
  
      for(int i=0;i<sizeof(unusablePins)/sizeof(int);i++) {
        if(pin == unusablePins[i]) {
          match = true;
          break;
        }
      }
  
      if(match) {
        printData(source, "Pin " + String(pin) + " is unusable.", true);
        return;
      }

      if(pin < 0 || pin > 53) {
        printData(source, "ERROR! Pin number out of range: " + String(pin) + " (0-53).", true);
        return;
      }

      node_t * aux2;

      aux2  = first;
      match = false;
      
      while(aux2 != NULL) {
        if(aux2->relay.pin == pin) {
          match = true;
          break;
        }
        aux2 = aux2->next;
      }

      if(match) {
        printData(source, "ERROR! Pin " + String(pin) + " is already in use.", true);
        return;
      }

      aux->relay.pin  = pin;
      aux->changeFlag = true;
      printData(source, "Pin " + String(pin) + " successfully updated (" + String(aux->relay.desc) + ")", true);
      
    }
    else if(param.substring(7,9) == "on") {
      if(!aux->relay.deleted) {
        digitalWrite(aux->relay.pin, LOW);
        aux->overrided  = true;
      }
      else {
        printData(source, "ERROR! Relay at pin " + String(num) + " was deleted.", true);
      }
    }
    else if(param.substring(7,10) == "off") {
      if(!aux->relay.deleted) {
        digitalWrite(aux->relay.pin, HIGH);
        aux->overrided  = true;
      }
      else {
        printData(source, "ERROR! Relay at pin " + String(num) + " was deleted.", true);
      }
    }
    else {
      printData(source, "ERROR! Unknown parameter: -" + param.substring(7) + "-", true);
    }
  }
  else if(param.substring(0,4) == "help"){
    printData(source, "'set' command arguments:", true);
    printData(source, "\trelay(0-50)pin (0-50)\t\t-> Change the pin number of the given relay.", true);
    printData(source, "\trelay(0-50)enable (0-1)\t\t-> Enable the given relay.", true);
    printData(source, "\trelay(0-50)desc (descripcion)\t-> Set relay description.", true);
    printData(source, "\trelay(0-50)starth (-23)\t\t-> Set the starting hour of the given relay.", true);
    printData(source, "\trelay(0-50)startm (-59)\t\t-> Set the starting minute of the given relay.", true);
    printData(source, "\trelay(0-50)endh (-23)\t\t-> Set the ending hour of the given relay.", true);
    printData(source, "\trelay(0-50)endm (-59)\t\t-> Set the ending minute of the given relay.", true);
    printData(source, "\trelay(0-50)resume\t\t-> Resumes the given relay.", true);
    printData(source, "\thostname\t\t\t-> Set the device name.", true);
    printData(source, "\tipadrress\t\t\t-> Set IP address.", true);
    printData(source, "\tsubnet\t\t\t\t-> Set subnet mask.", true);
    printData(source, "\tgateway\t\t\t\t-> Set default gateway.", true);
    printData(source, "\tdns\t\t\t\t-> Set the DNS server.", true);
    printData(source, "\tdatetime (xx:xx:xx xx/xx/xxxx)\t-> Set date and time.", true);
  }
  else if(param.substring(0,8) == "hostname") {
    param.substring(9).toCharArray(sys.hostName, 32);
    sysChangeFlag = true;
    printData(source, "\nHostname set to: " + String(sys.hostName), true);
  }
  else if(param.substring(0,9) == "ipaddress") {
    //char ipa[32];
    //param.substring(10).toCharArray(ipa, 32);
    String ipp;
    ipp = param.substring(10);
    
    IPAddress addr;
    if (addr.fromString(ipp)) {
      // it was a valid address, do something with it 
      printData(source, "IP successfully updated : " +
                        String(addr[0], DEC) + "." +
                        String(addr[1], DEC) + "." +
                        String(addr[2], DEC) + "." +
                        String(addr[3], DEC), true);
      printData(source, "save & reboot to apply changes.", true);
      sys.ip[0]        = addr[0];
      sys.ip[1]        = addr[1];
      sys.ip[2]        = addr[2];
      sys.ip[3]        = addr[3];
      sysChangeFlag = true;      
    }
    else {
      printData(source, "Invalid IP address : -" + ipp + "-", true);
    }
  }
  else if(param.substring(0,6) == "subnet") {
    char ipa[32];
    param.substring(7).toCharArray(ipa, 32);
    
    IPAddress addr;
    if (addr.fromString(ipa)) {
      // it was a valid address, do something with it 
      printData(source, "Subnet mask successfully updated : " +
                        String(addr[0], DEC) + "." +
                        String(addr[1], DEC) + "." +
                        String(addr[2], DEC) + "." +
                        String(addr[3], DEC), true);
      printData(source, "save & reboot to apply changes.", true);
      sys.subnet[0] = addr[0];
      sys.subnet[1] = addr[1];
      sys.subnet[2] = addr[2];
      sys.subnet[3] = addr[3];
      sysChangeFlag = true;      
    }
    else {
      printData(source, "Invalid subnet mask: " + String(ipa), true);
    }
  }
  else if(param.substring(0,7) == "gateway") {
    char ipa[32];
    param.substring(8).toCharArray(ipa, 32);
    
    IPAddress addr;
    if (addr.fromString(ipa)) {
      // it was a valid address, do something with it 
      printData(source, "Default gateway successfully updated: " +
                        String(addr[0], DEC) + "." +
                        String(addr[1], DEC) + "." +
                        String(addr[2], DEC) + "." +
                        String(addr[3], DEC), true);
      printData(source, "save & reboot to apply changes.", true);
      sys.gateway[0]  = addr[0];
      sys.gateway[1]  = addr[1];
      sys.gateway[2]  = addr[2];
      sys.gateway[3]  = addr[3];
      sysChangeFlag   = true;      
    }
    else {
      printData(source, "Invalid default gateway: " + String(ipa), true);
    }
  }
  else if(param.substring(0,3) == "dns") {
    char ipa[32];
    param.substring(4).toCharArray(ipa, 32);
    
    IPAddress addr;
    if (addr.fromString(ipa)) {
      // it was a valid address, do something with it 
      printData(source, "DNS server successfully updated: " +
                        String(addr[0], DEC) + "." +
                        String(addr[1], DEC) + "." +
                        String(addr[2], DEC) + "." +
                        String(addr[3], DEC), true);
      printData(source, "save & reboot to apply changes.", true);
      sys.dns[0]    = addr[0];
      sys.dns[1]    = addr[1];
      sys.dns[2]    = addr[2];
      sys.dns[3]    = addr[3];
      sysChangeFlag = true;      
    }
    else {
      printData(source, "Invalid DNS server: " + String(ipa), true);
    }
  }
  else if(param.substring(0,8) == "datetime") {
    int date[6];
    char str[32];
    param.substring(9, 41).toCharArray(str, 32);
    
    if (sscanf(str, "%d:%d:%d %d/%d/%d", &date[0], &date[1], &date[2], &date[3], &date[4], &date[5]) == 6) {
      if(date[0] < 0 || date[0] > 23) {
        printData(source, "ERROR! Hour out of range: " + String(date[0]), true); 
        return;
      }

      if(date[1] < 0 || date[1] > 59) {
        printData(source, "ERROR! Minute out of range: " + String(date[1]), true); 
        return;
      }

      if(date[2] < 0 || date[2] > 59) {
        printData(source, "ERROR! Second out of range: " + String(date[2]), true); 
        return;
      }

      if(date[3] >= 0 || date[3] <= 31) {
        if(date[3] == 31 &&
           (date[4] == 2 ||
           date[4] == 4 ||
           date[4] == 6 ||
           date[4] == 9 ||
           date[4] == 11)) {
          printData(source, "ERROR! Month " + String(date[4]) + " do not have 31 days.", true);
          return;
        }

        if(date[4] == 2 && date[3] == 29) {
          int x = date[5] % 4;
          if(x != 0) {
            printData(source, "ERROR! Year " + String(date[5]) + " is not a leap-year.\n February cannot have 29 days.", true);
            return;
          }
        }        
      }
      else {
        printData(source, "ERROR! The day is out of range: " + String(date[3]), true); 
        return;
      }

      if(date[4] < 0 || date[4] > 12) {
        printData(source, "ERROR! The month is out of range: " + String(date[4]), true); 
        return;
      }

      if(date[5] < 2018) {
        printData(source, "ERROR! Unless you'v travelled back in time, you cannot be in the year " + String(date[5]) + ".", true); 
        return;
      }

      if(date[5] > 2099) {
        printData(source, "ERROR! I don't think this device will be still alive beyond the year 2099.", true); 
        return;
      }
      
      //setTime(date[0],date[1],date[2],date[3],date[4],date[5]);
      RTC.adjust(DateTime(date[5],date[4],date[3],date[0],date[1],date[2]));
      printData(source, "Date successfully updated: " + String(getDate()), true);
    }
    else {
      printData(source, "Invalid date: " + 
                String(date[0]) + ":" +
                String(date[1]) + ":" +
                String(date[2]) + " " +
                String(date[3]) + "/" +
                String(date[4]) + "/" +
                String(date[5]), true);
    }    
  }
  else {
    printData(source, "Unknown parameter: -" + param + "-", true);
  }
}
