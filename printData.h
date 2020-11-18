void printData(String source, String data, bool rc) {
  if(source == "serial") {
    if(rc) {
      Serial.println(data);
    }
    else {
      Serial.print(data);
    }
  }
  else if(source == "telnet") {
    if(rc) {
      client.println(data);
    }
    else  {
      client.print(data);
    }
  }
}
