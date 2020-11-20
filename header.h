#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>
#include <EEPROM.h>

// ----------- MISC ------------- //

#define VERSION             F("1.7.0")
#define BAUD_RATE           9600
#define MAX_COMMAND_LEN     64
#define MAX_COMMAND_ARGS    10
#define MAX_HOSTNAME_LEN    32
#define MAX_RELAY_NUMBER    50
#define RELAY_DESC_LEN      32

#define SWITCH_OFF          0
#define SWITCH_ON           1

#define TYPE_START_HOUR     10
#define TYPE_END_HOUR       20
#define TYPE_START_MIN      30
#define TYPE_END_MIN        40

// Communication types
#define COMM_SERIAL      100
#define COMM_TELNET      200

// Type of address passed to setAddress function
#define IP_ADDRESS       0
#define SUBNET_MASK      1
#define DEFAULT_GATEWAY  2
#define DNS_SERVER       3

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
  char  desc[RELAY_DESC_LEN];
  byte  startHour;
  byte  startMin;
  byte  endHour;
  byte  endMin;
  unsigned long startTime;
  bool  deleted;
};

typedef struct node {
    struct  relayData relay;
    byte    memPos;
    bool    changeFlag;
    bool    overrided;
    struct  node * next;
} node_t;

node_t *first = NULL;
node_t *last  = NULL;

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

int           eeAddress       = 0;
bool          sysChangeFlag   = false;
String        days[]          = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
byte          output          = COMM_SERIAL;
char          *statuses[]     = { "ON", "OFF" };
unsigned long tstamp          = 0;
const byte    unusablePins[]  = { 9, 10, 11, 12, 13 };

// Array of special chars allowed in inputs
const byte    specialChars[]  = { 0x20, 0x2d, 0x2e, 0x2f, 0x3a };

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

// ----------- FUNCTION DECLARATIONS ----------- //

void parser(char *command);
void setParam(String param);
void saveData();

// Relay functions - Defined in relayFunctions.h
void setRelayStatus(byte pin, bool _status);
void resumeRelay(byte pin);
node_t *searchRelay(byte pin);
void createRelay(byte pin);
void deleteRelay(byte pin);
void checkRelays();
byte checkPin(char *_pin);
void setRelayParams(byte pin, char *_param, byte type);
void setRelayDesc(byte pin, char *_desc);
void switchRelay(byte pin, uint8_t state, bool overrided);
void changeRelayPin(byte currentPin, char *newPin);

// Getter functions - Defined in getters.h //
void getRelayInfo();
void getReceivedText();
String getDate();

// Setter functions - Defined in setters.h //
void setHostname(char *_name);
void setAddress(char *_address, char *_type);
void setDateTime(char *_date, char *_time);

// Utilities - Defined in utils.h
String arrayToString(byte array[], unsigned int len);
bool charAllowed(char c);
void clearArgs(char **args);
void loadSystemData();
void checkConnectionTimeout();
void closeConnection();
String parseUpTime(unsigned long startTime);

// Data printing - Defined in printFunctions.h
void printPrompt();
void printData(String data, bool rc);
void printHelp();
void setCommandHelp();
void showCommandHelp();
void relayCommandHelp();
void printIpAddress();
