//#define DEBUG

#include <Encoder.h>

// For a SPAD.neXt compatible Arduino library import CmdMessenger (4.0) into the Arduino IDE (recommended)
#ifndef DEBUG
#include <CmdMessenger.h>
CmdMessenger messenger(Serial);
#endif

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
  delay(25);
}
#endif

void knobAOuterRotate(bool clockwise) {
  knobRotate(1000, clockwise ? 100 : -100);
}
void knobAInnerRotate(bool clockwise) {
  knobRotate(1000, clockwise ? 1 : -1);
}
void knobBOuterRotate(bool clockwise) {
  knobRotate(2000, clockwise ? 100 : -100);
}
void knobBInnerRotate(bool clockwise) {
  knobRotate(2000, clockwise ? 1 : -1);
}

Encoder knobAOuter(19, 18, knobAOuterRotate);
Encoder knobAInner(21, 20, knobAInnerRotate);
Encoder knobBOuter(6, 5, knobBOuterRotate);
Encoder knobBInner(8, 7, knobBInnerRotate);

void CheckAllEncoders() {
  knobAOuter.process();
  knobAInner.process();
  knobBOuter.process();
  knobBInner.process();
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
  Serial.print("Buggon ID:");
  Serial.print(id);
  Serial.print(" VALUE: ");
  Serial.println(value);
}
#else
void sendCmd(int id, int value) {
  messenger.sendCmdStart(8);   // Channel no for sending input state to SPAD NEXT
  messenger.sendCmdArg(id);  // Input ID
  messenger.sendCmdArg(value);     // Value for off state
  messenger.sendCmdEnd();
  delay(25);
}
#endif

void CheckAllButtons() {
  if (digitalRead(knob1button) != knob1buttonlast) {
    if (knob1buttonlast == 0) {
      knob1buttonlast = 1;
      sendCmd(1100, 0);
    } else {
      knob1buttonlast = 0;
      sendCmd(1100, 1);
    }
  }

  if (digitalRead(knob2button) != knob2buttonlast) {
    if (knob2buttonlast == 0) {
      knob2buttonlast = 1;
      sendCmd(2100, 0);
    } else {
      knob2buttonlast = 0;
      sendCmd(2100, 1);
    }
  }
}

// ------------------ M A I N  --------------------------------

void setup() {

  Serial.begin(115200);

  pinMode(knob1button, INPUT_PULLUP);
  pinMode(knob2button, INPUT_PULLUP);

  knobAOuter.begin();
  knobAInner.begin();
  knobBOuter.begin();
  knobBInner.begin();

  attachCommandCallbacks();
}

void loop() {
  if (isStarted) {
    CheckAllButtons();
    CheckAllEncoders();
  }
  #ifndef DEBUG
  messenger.feedinSerialData();
  #endif
}
