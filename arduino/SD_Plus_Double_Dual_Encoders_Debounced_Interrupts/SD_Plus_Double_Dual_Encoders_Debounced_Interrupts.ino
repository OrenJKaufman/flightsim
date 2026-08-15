//#define DEBUG
#define DEBOUNCER_TIMEOUT 2

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include "./lib/debouncer/debouncer.h"

// For a SPAD.neXt compatible Arduino library import CmdMessenger (4.0) into the Arduino IDE (recommended)

#include <CmdMessenger.h>
CmdMessenger messenger(Serial);

Encoder knobAOuter(20, 21);
Encoder knobAInner(19, 18);
Encoder knobBOuter(7, 8);
Encoder knobBInner(6, 5);


Debouncer<long> knobAOuterDebouncer(DEBOUNCER_TIMEOUT);
Debouncer<long> knobAInnerDebouncer(DEBOUNCER_TIMEOUT);
Debouncer<long> knobBOuterDebouncer(DEBOUNCER_TIMEOUT);
Debouncer<long> knobBInnerDebouncer(DEBOUNCER_TIMEOUT);

#define KNOB_A_PIN 10
#define KNOB_B_PIN 9
DebouncedSwitch knobAButtonDebouncer(KNOB_A_PIN, 1);
DebouncedSwitch knobBButtonDebouncer(KNOB_B_PIN, 1);

long lastKnobAOuter, lastKnobAInner, lastKnobBOuter, lastKnobBInner = 0;
bool lastKnobAButtonPressed, lastKnobBButtonPressed = false;

#ifdef DEBUG
void DebugPrint(int id, int value) {
  Serial.print(id);
  Serial.print(" ");
  Serial.print(value);
}
#endif

//----- ROTARIES -------
void knobRotate(int id, int value) {
#ifdef DEBUG
  DebugPrint(id, value);
#else
  messenger.sendCmdStart(8);  // Channel no for sending input state to SPAD NEXT
  messenger.sendCmdArg(id);   // Input ID
  messenger.sendCmdArg(value);
  messenger.sendCmdEnd();
#endif
}

void buttonChange(int id, bool isPressed) {
#ifdef DEBUG
  DebugPrint(id, isPressed ? 1 : 0);
#else
  messenger.sendCmdStart(8);   // Channel no for sending input state to SPAD NEXT
  messenger.sendCmdArg(id);  // Input ID
  messenger.sendCmdArg(isPressed ? 1 : 0);     // Value for off state
  messenger.sendCmdEnd();
#endif
}

void ProcessEncoder(Debouncer<long> *debouncer, Encoder *knob, long *lastVal, int id, int multiplier) {
  debouncer->debounce(knob->read()/2);
  if (debouncer->hasChanged()) {
    long currentVal = debouncer->getDebounced();
    if (currentVal > *lastVal) {
      knobRotate(id, multiplier);
    }
    else if (currentVal < *lastVal) {
      knobRotate(id, -multiplier);
    }
    *lastVal = currentVal;
  }
}

void ProcessButton(DebouncedSwitch *debouncer, bool *lastPressed, int id) {
  debouncer->debounce();
  if (debouncer->isClosed() != *lastPressed) {
    *lastPressed = !*lastPressed;
    buttonChange(id, *lastPressed);
  }
}

//----- DEFINITIONS ------

// AUTHOR ID - You need to request your unique authorkey and type in instead of XXXs below
// To receive your unique authorkey, issue the command "!deviceinfo" on the SPAD.neXt discord.
// This key will identify the device author and enable author-only functions like e.g. editing the device UI or
// device configuration database, if the current SPAD.neXt user is the device author.

String authkey = "AUTH KEY";

// GUID -  Unique Identifier identifying the device. It should follow the GUID-Registryformat e.g. "{A8AA15C5-7BB6-4AC6-A558-A88CAFB78729}".
// To create a GUID you can use this site: https://www.guidgenerator.com/online-guid-generator.aspx (enable braces and hyphens)

String guid = "{0091149E-1C2E-4988-B0E4-F90B5457838C}";


// TYPE IN YOUR ARDUINO PINS FOR A LED AND A BUTTON HERE



//---------- VARIABLES ---------------

bool isReady = false;
bool isStarted = false;

//------------------- CALLBACKS and SPAD CONFIG ------------------------

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

// ------------------------ PROCESS FUNCTIONS--------------------

void CheckAllEncoders() {
  ProcessEncoder(&knobAOuterDebouncer, &knobAOuter, &lastKnobAOuter, 1000, 100);
  ProcessEncoder(&knobAInnerDebouncer, &knobAInner, &lastKnobAInner, 1000, 1);
  ProcessEncoder(&knobBOuterDebouncer, &knobBOuter, &lastKnobBOuter, 2000, 100);
  ProcessEncoder(&knobBInnerDebouncer, &knobBInner, &lastKnobBInner, 2000, 1);
}

void CheckAllButtons() {
  ProcessButton(&knobAButtonDebouncer, &lastKnobAButtonPressed, KNOB_A_PIN);
  ProcessButton(&knobBButtonDebouncer, &lastKnobBButtonPressed, KNOB_B_PIN);
}

// ------------------ M A I N  --------------------------------

void setup() {

  Serial.begin(115200);

  pinMode(KNOB_A_PIN, INPUT_PULLUP);
  pinMode(KNOB_B_PIN, INPUT_PULLUP);

#ifdef DEBUG
  isStarted = true;
#else
  attachCommandCallbacks();
#endif
}

void loop() {
  if (isStarted) {
    CheckAllEncoders();
    CheckAllButtons();
  }

#ifndef DEBUG
  messenger.feedinSerialData();
#endif
}
