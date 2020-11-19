void setHostname(String source, char *_name) {
  if(strlen(_name) == 0)
    return printData(source, F("Hostname cannot be an empty string."), true);
    
  if(strlen(_name) > MAX_HOSTNAME_LEN)
    return printData(source, F("Hostname exceeds maximum length."), true);

  // Looks like we're good...
  printData(source, _name, true);
  strcpy(sys.hostname, _name);

  // Inform that changes were made.
  sysChangeFlag = true;
  
  printData(source, F("Hostname set to: "), false);
  printData(source, String(sys.hostname), true);
}

void setAddress(String source, char *_address, int _type) {
  IPAddress addr;
  // Check validity
  if (!addr.fromString(_address)) {
    printData(source, F("Invalid address: "), false);
    printData(source, _address, true);
    return;
  }

  switch(_type) {
    case IP_ADDRESS:
      sys.ip[0]        = addr[0];
      sys.ip[1]        = addr[1];
      sys.ip[2]        = addr[2];
      sys.ip[3]        = addr[3];
      break;
    case SUBNET_MASK:
      sys.subnet[0] = addr[0];
      sys.subnet[1] = addr[1];
      sys.subnet[2] = addr[2];
      sys.subnet[3] = addr[3];
      break;
    case DEFAULT_GATEWAY:
      sys.gateway[0]  = addr[0];
      sys.gateway[1]  = addr[1];
      sys.gateway[2]  = addr[2];
      sys.gateway[3]  = addr[3];
      break;
    case DNS_SERVER:
      sys.dns[0]    = addr[0];
      sys.dns[1]    = addr[1];
      sys.dns[2]    = addr[2];
      sys.dns[3]    = addr[3];
      break;
  }
  // it was a valid address, do something with it 
  printData(source, F("Parameter successfully updated: "), false);
  printData(source, String(addr[0], DEC) + "." +
                    String(addr[1], DEC) + "." +
                    String(addr[2], DEC) + "." +
                    String(addr[3], DEC), true);
  printData(source, F("Save & reboot to apply changes."), true);
  sysChangeFlag = true;
}

void setDateTime(String source, char *_datetime) {
  byte  _hour;
  byte  _min;
  byte  _sec;
  int   _year;
  byte  _month;
  byte  _day;
  
  if (sscanf(_datetime, "%d/%d/%d %d:%d:%d", &_year, &_month, &_day, &_hour, &_min, &_sec) == 6) {
    if(_hour < 0 || _hour > 23) {
      printData(source, F("ERROR! Hour out of range: "), false);
      printData(source, String(_hour), true); 
      return;
    }

    if(_min < 0 || _min > 59) {
      printData(source, F("ERROR! Minute out of range: "), false);
      printData(source, String(_min), true); 
      return;
    }

    if(_sec < 0 || _sec > 59) {
      printData(source, F("ERROR! Second out of range: "), false);
      printData(source, String(_sec), true); 
      return;
    }

    if(_day >= 0 || _day <= 31) {
      if(_day == 31 &&
         (_month == 2 ||
         _month == 4 ||
         _month == 6 ||
         _month == 9 ||
         _month == 11)) {
        printData(source, F("ERROR! Month "), false);
        printData(source, String(_month), false);
        printData(source, F(" does not have 31 days."), true);
        return;
      }

      if(_month == 2 && _day == 29) {
        int x = _year % 4;
        if(x != 0) {
          printData(source, F("ERROR! Year "), false);
          printData(source, String(_year), false);
          printData(source, F(" is not a leap-year.\n February cannot have 29 days."), true);
          return;
        }
      }        
    }
    else {
      printData(source, F("ERROR! The day is out of range: "), false);
      printData(source, String(_day), true); 
      return;
    }

    if(_month < 0 || _month > 12) {
      printData(source, F("ERROR! The month is out of range: "), false);
      printData(source, String(_month), true); 
      return;
    }

    if(_year < 2018) {
      printData(source, F("ERROR! Unless you'v travelled back in time, you cannot be in the year "), false);
      printData(source, String(_year) + ".", true); 
      return;
    }

    if(_year > 2099) {
      printData(source, F("ERROR! I don't think this device will be still alive beyond the year 2099."), true); 
      return;
    }
    
    RTC.adjust(DateTime(_year,_month,_day,_hour,_min,_sec));
    printData(source, F("Date successfully updated: "), false);
    printData(source, String(getDate()), true);
  }
  else {
    printData(source, F("Invalid date: "), false);
    printData(source, String(_hour) + ":" +
              String(_min) + ":" +
              String(_sec) + " " +
              String(_day) + "/" +
              String(_month) + "/" +
              String(_year), true);
  }
}
