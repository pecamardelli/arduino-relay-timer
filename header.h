#include <Wire.h>                        // Librería para el protocolo I2C
#include <RTClib.h>                      // Librería para el reloj de tiempo real
#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>
#include <EEPROM.h>

// ----------- DEFINICION DE FUNCIONES ----------- //
void loadSystemData();
String getDate();
void getReceivedText(String source);
void parseReceivedText(String source);
void printPrompt();
void printData(String source, String data, bool rc);
void closeConnection();
void setParam(String param, String source);
void saveData(String source);
String arrayToString(byte array[], unsigned int len);
void checkRelays();

// ----------- DEFINICION DE TIPOS DE PLACA ----------- //

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

// ----------- DEFINICION DE VARIABLES ----------- //

RTC_DS1307 RTC;

// ----------- VARIABLES DE LOS RELÉS ----------- //

int   relayQuantity     = 0;
int   relayMaxQuantity  = 50;   // No permitir más de 50 relés... Por cantidad de EEPROM y pines.

typedef struct relayData
{
  byte  type;
  byte  pin;
  bool  enabled;
  char  desc[32];
  byte  startHour;
  byte  startMin;
  byte  endHour;
  byte  endMin;
  bool  deleted;
};

typedef struct node {
    struct  relayData relay;
    byte    memPos;
    bool    changeFlag;
    bool    overrided;
    struct node * next;
} node_t;

node_t *first = NULL, *last = NULL, *aux = NULL;

// ----------- VARIABLES DE SISTEMA ----------- //

typedef struct systemData
{
  char   hostName[32];
  byte   mac[6];
  byte   ip[4];
  byte   subnet[4];
  byte   gateway[4];
  byte   dns[4];
};

struct  systemData sys;

int     eeAddress       = 0;
bool    sysChangeFlag   = false;
String  dias[]          = { "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado" };
String  sysVersion      = "1.5";
String  estados[]       = { "ON", "OFF" };
int     resetPin        = 9;
unsigned long tstamp    = 0;
int     unusablePins[]  = { 9, 10, 11, 12, 13 };

// ----------- VARIABLES DEL SERVIDOR TELNET ----------- //

String textBuff;
int charsReceived = 0;
int textBuffSize  = 64;

// we'll use a flag separate from client.connected
// so we can recognize when a new connection has been created
boolean connectFlag = 0;                    

//time in milliseconds of last activity
unsigned long timeOfLastActivity;

//five minutes
unsigned long allowedConnectTime = 300000;

EthernetServer server(23); // Telnet listens on port 23
EthernetClient client = 0; // Client needs to have global scope so it can be called
       // from functions outside of loop, but we don't know
       // what client is yet, so creating an empty object
/* -------------------------------------------------------------- */

