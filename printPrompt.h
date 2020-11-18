void printPrompt() {
  timeOfLastActivity = millis();
  client.flush();
  charsReceived = 0; // Count of characters received
  client.print("\n>");
}
