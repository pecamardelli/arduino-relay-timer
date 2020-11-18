void deleteRelay(String source) {
  int pin = textBuff.substring(9,11).toInt();

  // Check if pin number is out of range
  if(pin < 0 || pin > 53) {
    printData(source, "Pin number is out of range (0-53): " + String(pin), true);
    return;
  }
  
  aux = first;
  bool match = false;
  
  while(aux != NULL) {
    if((int)aux->relay.pin == pin) {
      aux->relay.deleted = true;
      printData(source, "Relay at pin " + String(pin) + " marked as deleted.", true);
      match = true;
      aux->changeFlag = true;
      return;
    }
    aux = aux->next;
  }

  if(!match) {
    printData(source, "No relay found at pin " + String(pin) + ".", true);
  }
}
