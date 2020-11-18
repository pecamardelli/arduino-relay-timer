#include <Wire.h>                        // Librería para el protocolo I2C
#include "RTClib.h"                      // Librería para el reloj de tiempo real
#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>
#include <EEPROM.h>

void loadSystemData();
String getDate();
void getReceivedText();
void parseReceivedText(String source);
void printPrompt();
void printData(String source, String data, bool rc);
void closeConnection();
void printDirectory(File dir, int numTabs, String source);
void printFile(String file, String source);
void deleteFile(String file, String source);
void setParam(String param, String source);
void saveData();
String arrayToString(byte array[], unsigned int len);

#if   defined(ARDUINO_AVR_ADK)       
    #define BOARD "Mega Adk"
#elif defined(ARDUINO_AVR_BT)    // Bluetooth
    #define BOARD "Bt"
#elif defined(ARDUINO_AVR_DUEMILANOVE)       
    #define BOARD "Duemilanove"
#elif defined(ARDUINO_AVR_ESPLORA)       
    #define BOARD "Esplora"
#elif defined(ARDUINO_AVR_ETHERNET)       
    #define BOARD "Ethernet"
#elif defined(ARDUINO_AVR_FIO)       
    #define BOARD "Fio"
#elif defined(ARDUINO_AVR_GEMMA)
    #define BOARD "Gemma"
#elif defined(ARDUINO_AVR_LEONARDO)       
    #define BOARD "Leonardo"
#elif defined(ARDUINO_AVR_LILYPAD)
    #define BOARD "Lilypad"
#elif defined(ARDUINO_AVR_LILYPAD_USB)
    #define BOARD "Lilypad Usb"
#elif defined(ARDUINO_AVR_MEGA)       
    #define BOARD "Mega"
#elif defined(ARDUINO_AVR_MEGA2560)       
    #define BOARD "Mega 2560"
#elif defined(ARDUINO_AVR_MICRO)       
    #define BOARD "Micro"
#elif defined(ARDUINO_AVR_MINI)       
    #define BOARD "Mini"
#elif defined(ARDUINO_AVR_NANO)       
    #define BOARD "Nano"
#elif defined(ARDUINO_AVR_NG)       
    #define BOARD "NG"
#elif defined(ARDUINO_AVR_PRO)       
    #define BOARD "Pro"
#elif defined(ARDUINO_AVR_ROBOT_CONTROL)       
    #define BOARD "Robot Ctrl"
#elif defined(ARDUINO_AVR_ROBOT_MOTOR)       
    #define BOARD "Robot Motor"
#elif defined(ARDUINO_AVR_UNO)       
    #define BOARD "Uno"
#elif defined(ARDUINO_AVR_YUN)       
    #define BOARD "Yun"

// These boards must be installed separately:
#elif defined(ARDUINO_SAM_DUE)       
    #define BOARD "Due"
#elif defined(ARDUINO_SAMD_ZERO)       
    #define BOARD "Zero"
#elif defined(ARDUINO_ARC32_TOOLS)       
    #define BOARD "101"
#else
   #error "Unknown board"
#endif

RTC_DS1307 RTC;

/* ---------------- VARIABLES DE LOS RELÉS ---------------------- */

String  hostName          = "ARDUINO";
int     relayPin[]        = { 22, 23, 24, 25 };
int     relayEnabled[]    = { 0, 0, 0, 0 };
String  relayDesc[]       = { "Relay 0", "Relay 1", "Relay 2", "Relay 3" };
int     relayStartHour[]  = { 0, 0, 0, 0 };
int     relayStartMin[]   = { 0, 0, 0, 0 };
int     relayEndHour[]    = { 0, 0, 0, 0 };
int     relayEndMin[]     = { 0, 0, 0, 0 };
bool    overrided[]       = { false, false, false, false };
String  estados[]         = { "encendido", "apagado" };

/* -------------------------------------------------------------- */

/* ------------------- VARIABLES GLOBALES ----------------------- */

String dias[]       = { "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado" };
String sysFolder    = "/system/";
String sysVersion   = "1.0.5";

/* -------------------------------------------------------------- */

/* ------------------ PARÁMETROS ETHERNET ----------------------- */

byte mac[] =     { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress   ip(192, 168, 40, 104);
IPAddress   gateway(192, 168, 40, 1);
IPAddress   subnet(255, 255, 255, 0);
IPAddress   dns(179,42,171,21);

String textBuff;
int charsReceived = 0;
int textBuffSize  = 64;

boolean connectFlag = 0; //we'll use a flag separate from client.connected
         //so we can recognize when a new connection has been created
unsigned long timeOfLastActivity; //time in milliseconds of last activity
unsigned long allowedConnectTime = 300000; //five minutes

EthernetServer server(23); // Telnet listens on port 23
EthernetClient client = 0; // Client needs to have global scope so it can be called
       // from functions outside of loop, but we don't know
       // what client is yet, so creating an empty object
/* -------------------------------------------------------------- */

