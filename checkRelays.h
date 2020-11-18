void checkRelays() {
  DateTime now = RTC.now();
  int startmins, endmins, actualmins;

  aux = first;
  
  while(aux != NULL) {
    if(aux->relay.enabled && !aux->relay.deleted && !aux->overrided) {
      startmins   = aux->relay.startHour * 60 + aux->relay.startMin;
      endmins     = aux->relay.endHour * 60 + aux->relay.endMin;
      actualmins  = now.hour() * 60 + now.minute();

      if(startmins <= actualmins) {
        if(endmins > startmins) {
          if(endmins < actualmins) {
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
        if(startmins < endmins) {
          digitalWrite(aux->relay.pin, HIGH);
        }
        else {          
          if(endmins > actualmins) {
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
