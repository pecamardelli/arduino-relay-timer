void closeConnection() {
  client.println(F("\nBye.\n"));
  client.stop();
  connectFlag = 0;
}
