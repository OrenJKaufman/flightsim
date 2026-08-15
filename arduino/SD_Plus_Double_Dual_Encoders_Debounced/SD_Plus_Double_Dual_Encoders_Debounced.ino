// For a SPAD.neXt compatible Arduino library import CmdMessenger (4.0) into the Arduino IDE (recommended)

//#define DEBUG

#define DEBOUNCER_TIMEOUT 2
#include "./lib/debouncer/debouncer.h"

#include <CmdMessenger.h>
CmdMessenger messenger(Serial);

//----- ROTARIES -------

#define NUMROTARIES 4

struct rotariesdef {
  byte pin1;
  byte pin2;
  int ccwchar;
  int cwchar;
  volatile unsigned char state;
};

rotariesdef rotaries[NUMROTARIES]{
  { 7, 8, 2, 3, 0 },
  { 6, 5, 0, 1, 0 },
  { 20, 21, 6, 7, 0 },
  { 19, 18, 4, 5, 0 },
};

struct KnobDebouncers {
    Debouncer<int> pin1;
    Debouncer<int> pin2;

    KnobDebouncers(unsigned long timeout)
        : pin1(timeout), pin2(timeout) {}
};

KnobDebouncers knobs[4] = {
    { DEBOUNCER_TIMEOUT }, // knob A outer
    { DEBOUNCER_TIMEOUT }, // knob A inner
    { DEBOUNCER_TIMEOUT }, // knob B outer
    { DEBOUNCER_TIMEOUT }  // knob B inner
};

#define DIR_CCW 0x10
#define DIR_CW 0x20
#define R_START 0x0

// Use the half-step state table (emits a code at 00 and 11)
#define R_CCW_BEGIN 0x1
#define R_CW_BEGIN 0x2
#define R_START_M 0x3
#define R_CW_BEGIN_M 0x4
#define R_CCW_BEGIN_M 0x5
const unsigned char ttable[6][4] = {
  // R_START (00)
  { R_START_M, R_CW_BEGIN, R_CCW_BEGIN, R_START },
  // R_CCW_BEGIN
  { R_START_M | DIR_CCW, R_START, R_CCW_BEGIN, R_START },
  // R_CW_BEGIN
  { R_START_M | DIR_CW, R_CW_BEGIN, R_START, R_START },
  // R_START_M (11)
  { R_START_M, R_CCW_BEGIN_M, R_CW_BEGIN_M, R_START },
  // R_CW_BEGIN_M
  { R_START_M, R_START_M, R_CW_BEGIN_M, R_START | DIR_CW },
  // R_CCW_BEGIN_M
  { R_START_M, R_CCW_BEGIN_M, R_START_M, R_START | DIR_CCW },
};

/* Call this once in setup(). */
void rotary_init() {
  for (int i = 0; i < NUMROTARIES; i++) {
    pinMode(rotaries[i].pin1, INPUT_PULLUP);
    pinMode(rotaries[i].pin2, INPUT_PULLUP);
  }
}

/* Read input pins and process for events. Call this either from a
 * loop or an interrupt (eg pin change or timer).
 *
 * Returns 0 on no event, otherwise 0x80 or 0x40 depending on the direction.
 */
unsigned char rotary_process(int _i) {
  Debouncer<int> *pin1Debouncer = &knobs[_i].pin1;
  Debouncer<int> *pin2Debouncer = &knobs[_i].pin2;
  pin1Debouncer->debounce(digitalRead(rotaries[_i].pin1));
  pin2Debouncer->debounce(digitalRead(rotaries[_i].pin2));
  unsigned char pinstate = (pin2Debouncer->getDebounced() << 1) | pin1Debouncer->getDebounced();
  rotaries[_i].state = ttable[rotaries[_i].state & 0xf][pinstate];
  return (rotaries[_i].state & 0x30);
}

//----- ROTARIES -------
#ifdef DEBUG
void knobRotate(int id, int value) {
  Serial.print("Rotate ID:");
  Serial.print(id);
  Serial.print(" VALUE: ");
  Serial.println(value);
}
#else
void knobRotate(int id, int value) {
  messenger.sendCmdStart(8);  // Channel no for sending input state to SPAD NEXT
  messenger.sendCmdArg(id);   // Input ID
  messenger.sendCmdArg(value);
  messenger.sendCmdEnd();
}
#endif

void CheckAllEncoders(void) {
  for (int i = 0; i < NUMROTARIES; i++) {
    int rotaryId = i >= 2 ? 1000 : 2000;
    unsigned char result = rotary_process(i);
    if (result == DIR_CCW) {
      knobRotate(rotaryId, i % 2 ? -1 : -100);
    };
    if (result == DIR_CW) {
      knobRotate(rotaryId, i % 2 ? 1 : 100);
    };
  }
}

//----- DEFINITIONS ------

#ifndef DEBUG
// AUTHOR ID - You need to request your unique authorkey and type in instead of XXXs below
// To receive your unique authorkey, issue the command "!deviceinfo" on the SPAD.neXt discord.
// This key will identify the device author and enable author-only functions like e.g. editing the device UI or
// device configuration database, if the current SPAD.neXt user is the device author.

String authkey = "AUTH KEY";

// GUID -  Unique Identifier identifying the device. It should follow the GUID-Registryformat e.g. "{A8AA15C5-7BB6-4AC6-A558-A88CAFB78729}".
// To create a GUID you can use this site: https://www.guidgenerator.com/online-guid-generator.aspx (enable braces and hyphens)

String guid = "{0091149E-1C2E-4988-B0E4-F90B5457838C}";
#endif

// TYPE IN YOUR ARDUINO PINS FOR A LED AND A BUTTON HERE

int knob1button = 10;
int knob2button = 9;


//---------- VARIABLES ---------------

int knob1buttonlast = -1;
int knob2buttonlast = -1;

bool isReady = false;
bool isStarted = false;

//------------------- CALLBACKS and SPAD CONFIG ------------------------

#ifdef DEBUG
void attachCommandCallbacks() {
  isStarted = true;
}
#else
void attachCommandCallbacks() {
  messenger.sendCmd(3, "ATTACHING CALLBACKS!");
  messenger.attach(0, onIdentifyRequest);
  messenger.attach(2, onSpadEvent);
  messenger.attach(3, onUnknownCommand);
}

void onUnknownCommand() {
  messenger.sendCmd(3, "UNKNOWN COMMAND");
}


void onSpadEvent() {
  char *szEvent = messenger.readStringArg();
  if (strcmp(szEvent, "START") == 0) {  // SPAD tells device, it's ok to send input now
    isStarted = 1;
    return;
  }
  if (strcmp(szEvent, "END") == 0) {  // SPAD tells device it will exit now
    isStarted = 0;
    return;
  }
}


void onIdentifyRequest() {
  char *szRequest = messenger.readStringArg();

  if (strcmp(szRequest, "INIT") == 0) {
    messenger.sendCmdStart(0);
    messenger.sendCmdArg("SPAD");
    messenger.sendCmdArg(guid);                    // GUID
    messenger.sendCmdArg("Double Dual Encoders");  // DEVICE DISPLAY NAME
    messenger.sendCmdArg(2);                       // SPAD SERIAL VERSION, DON'T CHANGE
    messenger.sendCmdArg("1");                     // DEVICE VERSION NUMBER
    messenger.sendCmdArg("AUTHOR=" + authkey);     // AUTHOR ID - Edit the variable on top part of the code and remove // at the beginning of this line to activate your  auth code.
    messenger.sendCmdEnd();
    return;
  }

  if (strcmp(szRequest, "SCANSTATE") == 0) {
    messenger.sendCmdStart(0);
    messenger.sendCmdArg("STATESCAN");
    messenger.sendCmdEnd();
    return;
  }

  if (strcmp(szRequest, "PING") == 0) {
    messenger.sendCmdStart(0);
    messenger.sendCmdArg("PONG");
    messenger.sendCmdArg(messenger.readInt32Arg());
    messenger.sendCmdEnd();
    return;
  }

  if (strcmp(szRequest, "CONFIG") == 0) {
    messenger.sendCmdStart(0);
    messenger.sendCmdArg("OPTION");
    messenger.sendCmdArg("ISGENERIC=1");
    messenger.sendCmdArg("PAGESUPPORT=1");
    messenger.sendCmdArg("NO_DISPLAY_CLEAR=1");
    messenger.sendCmdEnd();


    //----- CREATE ENCODER 1 -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(1000);                   // This is the button ID
    messenger.sendCmdArg("KNOB_A");               //SPAD GUI Display name
    messenger.sendCmdArg("ENCODER");              // Type
    messenger.sendCmdArg("SPAD_DOUBLE_ENCODER");  // Behaviour
    messenger.sendCmdEnd();


    //----- CREATE KNOB 1 BUTTON -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(1100);               // This is the button ID
    messenger.sendCmdArg("KNOB_A_PUSH");      //SPAD GUI Display name
    messenger.sendCmdArg("PUSHBUTTON");       // Type
    messenger.sendCmdArg("SPAD_PUSHBUTTON");  // Behaviour
    messenger.sendCmdArg("HIDDEN=1");
    messenger.sendCmdArg("ROUTETO=KNOB_A");
    messenger.sendCmdEnd();


    //----- CREATE ENCODER 2 -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(2000);                   // This is the button ID
    messenger.sendCmdArg("KNOB_B");               //SPAD GUI Display name
    messenger.sendCmdArg("ENCODER");              // Type
    messenger.sendCmdArg("SPAD_DOUBLE_ENCODER");  // Behaviour
    messenger.sendCmdEnd();


    //----- CREATE KNOB 2 BUTTON -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(2100);               // This is the button ID
    messenger.sendCmdArg("KNOB_B_PUSH");      //SPAD GUI Display name
    messenger.sendCmdArg("PUSHBUTTON");       // Type
    messenger.sendCmdArg("SPAD_PUSHBUTTON");  // Behaviour
    messenger.sendCmdArg("HIDDEN=1");
    messenger.sendCmdArg("ROUTETO=KNOB_B");
    messenger.sendCmdEnd();

    //-----  END CONFIG ----

    messenger.sendCmd(0, "CONFIG");
    return;
  }
}
#endif


// ------------------------ PROCESS FUNCTIONS--------------------

#ifdef DEBUG
void sendCmd(int id, int value) {
  Serial.print("Button ID:");
  Serial.print(id);
  Serial.print(" VALUE: ");
  Serial.println(value);
}
#else
void sendCmd(int id, int value) {
  messenger.sendCmdStart(8);    // Channel no for sending input state to SPAD NEXT
  messenger.sendCmdArg(id);     // Input ID
  messenger.sendCmdArg(value);  // Value for off state
  messenger.sendCmdEnd();
}
#endif

unsigned long knob1ButtonLastMillis = 0;
unsigned long knob2ButtonLastMillis = 0;

void CheckAllButtons() {
  if (millis() - knob1ButtonLastMillis > 25) {
    if (digitalRead(knob1button) != knob1buttonlast) {
      if (knob1buttonlast == 0) {
        knob1buttonlast = 1;
        sendCmd(1100, 0);
      } else {
        knob1buttonlast = 0;
        sendCmd(1100, 1);
      }
    }
    knob1ButtonLastMillis = millis();
  }

  if (millis() - knob2ButtonLastMillis > 25) {
    if (digitalRead(knob2button) != knob2buttonlast) {
      if (knob2buttonlast == 0) {
        knob2buttonlast = 1;
        sendCmd(2100, 0);
      } else {
        knob2buttonlast = 0;
        sendCmd(2100, 1);
      }
    }
    knob2ButtonLastMillis = millis();
  }
}

// ------------------ M A I N  --------------------------------

void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  #else
  Serial.begin(115200);
  #endif

  pinMode(knob1button, INPUT_PULLUP);
  pinMode(knob2button, INPUT_PULLUP);

  rotary_init();

  attachCommandCallbacks();
}

void loop() {
  if (isStarted) {
    CheckAllButtons();
    CheckAllEncoders();
  }
  messenger.feedinSerialData();
}
