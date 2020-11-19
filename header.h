#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>
#include <EEPROM.h>

// ----------- MISC ------------- //

#define BAUD_RATE           9600
#define MAX_COMMAND_LEN     64
#define MAX_COMMAND_ARGS    10
#define MAX_HOSTNAME_LEN    32
#define MAX_RELAY_NUMBER    50

// Communication types
#define COMM_SERIAL      0
#define COMM_TELNET      1

// Type of address passed to setAddress function
#define IP_ADDRESS       0
#define SUBNET_MASK      1
#define DEFAULT_GATEWAY  2
#define DNS_SERVER       3

// ----------- FUNCTION DECLARATIONS ----------- //

void parser(String source, char *command);
void setParam(String param, String source);
void saveData(String source);
void checkRelays();
void addRelay(String source, char *_pin);
void deleteRelay(String source, char *_pin);

// Getter functions - Defined in getters.h //
void getRelayInfo(String source);
void getReceivedText(String source);
String getDate();

// Setter functions - Defined in setters.h //
void setHostname(String source, char *_name);
void setAddress(String source, char *_address, char *_type);
void setDateTime(String source, char *_datetime);

// Utilities - Defined in utils.h
String arrayToString(byte array[], unsigned int len);
void clearArgs(char **args);
void loadSystemData();
void checkConnectionTimeout();
void closeConnection();

// Data printing - Defined in printFunctions.h
void printPrompt();
void printData(String source, String data, bool rc);
void printHelp(String source);
void printSetHelp(String source);
void printIpAddress(String source);

// ----------- BOARD TYPES ----------- //

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

// ----------- VARIABLE DECLARATION ----------- //

RTC_DS1307 RTC;

// ----------- RELAYS ----------- //

byte   relayQuantity     = 0;

typedef struct relayData {
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
    struct  node * next;
} node_t;

node_t *first = NULL, *last = NULL, *aux = NULL;

// ----------- SYSTEM ----------- //

typedef struct systemData {
  char   hostname[32];
  byte   mac[6];
  byte   ip[4];
  byte   subnet[4];
  byte   gateway[4];
  byte   dns[4];
  byte   resetPin;
};

struct  systemData sys;

int     eeAddress       = 0;
bool    sysChangeFlag   = false;
String  days[]          = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
String  sysVersion      = "1.7.0";
String  statuses[]      = { "ON", "OFF" };
unsigned long tstamp    = 0;
const int     unusablePins[]  = { 9, 10, 11, 12, 13 };

// ----------- TELNET SERVER ----------- //

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
