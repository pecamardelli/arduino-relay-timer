# arduino-relay-timer

Tiny relay timer with an embedded telnet server. I use this extremely large program to turn on and off my house's lights.

- It's based on `Arduino Mega` and uses a four relay module.
- All data is stored on the EEPROM
- Time is kept in a RTC DS1302 module

Relays can be created and assigned to a pin.

All data is saved to EEPROM. No other storage media is needed.

