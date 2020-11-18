/*
 * ARDUINO RELAY TIMER - 26 de septiembre de 2018
 * Version 1.5.1
 * 
 * Cambios con respecto a la versión 1.5
 * - Se pueden prender los reles manualmente sin que la función checkRelays() modifique un rele con
 *   la variable overrided en true.
 * - Se corrigieron un par de errores de ortografía.
 * - Se modificó el orden de inicio de la función Server.begin(). Se puso después de Ethernet.begin().
 * - Se eliminó la función array_to_string() con su respectivo archivo.
 * - Agregado un contador de uptime de los relés y la entrada correspondiente en relayinfo.
 * - Reemplazados los comandos hostname e ip por sysinfo. Además muestra el uptime del sistema.
 * - Se agregó una línea en blanco antes de imprimir la salida de los comandos.
 * - No se parsea un comando en blanco. Antes al apretar enter tiraba error de comándo no reconocido.
 * - Se cambió la salida del comando save.
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
    client.println("Desconectado por inactividad.");
    client.stop();
    connectFlag = 0;
  }
}
