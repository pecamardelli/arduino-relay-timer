void closeConnection() {
  client.println("\nBye.\n");
  client.stop();
  connectFlag = 0;
}
