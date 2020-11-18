/*
 * ARDUINO RELAY TIMER - 25 de septiembre de 2018
 */

#include "header.h"
#include "loadSystemData.h"
#include "getDate.h"
#include "getReceivedText.h"
#include "parseReceivedText.h"
#include "printPrompt.h"
#include "printData.h"
#include "closeConnection.h"
#include "setParam.h"
#include "saveData.h"
#include "arrayToString.h"
#include "checkRelays.h"

void setup() {
  
  // Es necesario conectar una resistencia de 1k desde el pin elegido
  // hasta el pin RESET, de lo contrario la placa entra en un loop de
  // reinicios interminable. Esto pasa porque al arrancar, todos los
  // pines se ponen como OUTPUT y LOW por un momento.
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, HIGH);
  
  Serial.begin(9600);

  Serial.print("Iniciando el puerto I2C: ");
  Wire.begin();                         // Inicia el puerto I2C
  Serial.println("hecho.\n");

  Serial.print("Iniciando el reloj RTC: ");
  RTC.begin();                          // Inicia la comunicación con el RTC
  Serial.println("hecho.\n");

  // Iniciamos el servidor de telnet
  Serial.print("Iniciando servidor: ");
  server.begin();
  Serial.println("hecho.\n");
  
  loadSystemData();
}

void loop() {
  // put your main code here, to run repeatedly:

  if(server.available() && !connectFlag)
  {
    connectFlag = 1;
    client = server.available();
    client.flush();
    client.println("ARDUINO RELAY TIMER");
    client.print(BOARD);
    client.println(" - " + String(sys.hostName));
    client.println("Version " + sysVersion);
    client.println("'help' para mostrar comandos");
    client.flush();
    printPrompt();
  }

  // check to see if text received
  if (client.connected() && client.available()) getReceivedText("telnet");

  // check to see if connection has timed out
  if(connectFlag) checkConnectionTimeout();

  if(Serial.available()) getReceivedText("serial");
    
  if(millis() - tstamp >= 5000)
  {
    checkRelays();
    tstamp = millis();
  }
}

void checkConnectionTimeout()
{
  if(millis() - timeOfLastActivity > allowedConnectTime) {
    client.println();
    client.println("Timeout disconnect.");
    client.stop();
    connectFlag = 0;
  }
}
