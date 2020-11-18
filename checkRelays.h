void checkRelays() {
  DateTime now = RTC.now();
  int startMins, endMins, currentMins;

  aux = first;
  while(aux != NULL) {
    if(aux->relay.enabled && !aux->relay.deleted && !aux->overrided) {
      startMins   = aux->relay.startHour * 60 + aux->relay.startMin;
      endMins     = aux->relay.endHour * 60 + aux->relay.endMin;
      currentMins  = now.hour() * 60 + now.minute();

      if(startMins <= currentMins) {
        if(endMins > startMins) {
          if(endMins < currentMins) {
            digitalWrite(aux->relay.pin, HIGH);
          }
          else {
            digitalWrite(aux->relay.pin, LOW);
          }
        }
        else {
          digitalWrite(aux->relay.pin, LOW);
        }
      }
      else {
        if(startMins < endMins) {
          digitalWrite(aux->relay.pin, HIGH);
        }
        else {          
          if(endMins > currentMins) {
            digitalWrite(aux->relay.pin, LOW);
          }
          else {
            digitalWrite(aux->relay.pin, HIGH);
          }
        }
      }
    }
    else if(!aux->overrided) {
      digitalWrite(aux->relay.pin, HIGH);
    }
    
    aux = aux->next;
  }
}
