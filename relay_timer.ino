#include "header.h"
#include "loadSystemData.h"
#include "getDate.h"
#include "getReceivedText.h"
#include "parseReceivedText.h"
#include "printPrompt.h"
#include "printData.h"
#include "closeConnection.h"
#include "printDirectory.h"
#include "printFile.h"
#include "deleteFile.h"
#include "setParam.h"
#include "saveData.h"
#include "arrayToString.h"

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);

  Serial.print("Iniciando el puerto I2C: ");
  Wire.begin();                         // Inicia el puerto I2C
  Serial.println("hecho.\n");

  Serial.print("Iniciando el reloj RTC: ");
  RTC.begin();                          // Inicia la comunicación con el RTC
  Serial.println("hecho.\n");

  Serial.print("Iniciando ethernet shield: ");
  Ethernet.begin(mac, ip, dns, gateway, subnet);
  Serial.println("hecho.\n");

  Serial.print("Iniciando servidor: ");
  server.begin();
  Serial.println("hecho.\n");

  Serial.print("Iniciando tarjeta SD: ");

  if (!SD.begin(4)) {
    Serial.println("ERROR!\n");
  }
  else
  {
    Serial.println("hecho.\n");
  }
  
  loadSystemData();
}

void loop() {
  // put your main code here, to run repeatedly:

  if(server.available() && !connectFlag)
  {
    connectFlag = 1;
    client = server.available();
    client.println("ARDUINO RELAY TIMER");
    client.print(BOARD);
    client.println(" - " + hostName);
    client.println("Version " + sysVersion);
    client.println("'help' para mostrar comandos");
    client.flush();
    printPrompt();
  }

  // check to see if text received
  if (client.connected() && client.available()) getReceivedText();

  // check to see if connection has timed out
  if(connectFlag) checkConnectionTimeout();

  int rcv = 0;
  String cad;

  while(Serial.available())
  {
    cad += (char)Serial.read();
    rcv = 1;
    delay(5);
  }

  if(rcv == 1)
  {
    textBuff += " ";    // Se le agrega un caracter al principio porque el telnet lo hace... Así se usa la misma funcion para parsear los comandos
    textBuff += cad;
    parseReceivedText("serial");
  }

  checkRelays();
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

void checkRelays()
{
  DateTime now = RTC.now();
  int startmins, endmins, actualmins;
  
  for(int i=0;i<3;i++)
  {
    if(relayEnabled[i] && !overrided[i])
    {
      startmins   = relayStartHour[i] * 60 + relayStartMin[i];
      endmins     = relayEndHour[i] * 60 + relayEndMin[i];
      actualmins  = now.hour() * 60 + now.minute();

      if(startmins <= actualmins)
      {
        if(endmins > startmins)
        {
          if(endmins < actualmins)
          {
            digitalWrite(relayPin[i], HIGH);
          }
          else
          {
            digitalWrite(relayPin[i], LOW);
          }
        }
        else
        {
          digitalWrite(relayPin[i], LOW);
        }
      }
      else
      {
        if(startmins < endmins)
        {
          digitalWrite(relayPin[i], HIGH);
        }
        else
        {          
          if(endmins > actualmins)
          {
            digitalWrite(relayPin[i], LOW);
          }
          else
          {
            digitalWrite(relayPin[i], HIGH);
          }
        }
      }
    }
    else
    {
      digitalWrite(relayPin[i], HIGH);
    }
  }
}
/*
void logger(String cat, String source, String data)
{
  String s;

  if(source == "serial")
  {
    s = "[SERIAL] ";
  }
  else if(source == "telnet")
  {
    s = "[TELNET](" + String(client.remoteIP()) + ") ";
  }
  else
  {
    s = "";
  }
  
  String logfile = sysFolder + "syslog.txt";
  
  DateTime now = RTC.now();

  File entry = SD.open(logfile, FILE_WRITE);

  if(entry)
  {
    entry.println(String(now.day()) + " de " +
                  meses[now.month()] + " de " +
                  String(now.year()) + " " + 
                  String(now.hour()) + ":" + 
                  String(now.minute()) + ":" + 
                  String(now.second()) + " " + 
                  s + data);
    entry.close();
  }
  else
  {
    Serial.println("No se pudo abrir " + logfile + ".");
  }
}
*/


