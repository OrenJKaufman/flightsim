//#define DEBUG

#include <CmdMessenger.h>
#include <light_CD74HC4067.h>

#ifndef DEBUG
CmdMessenger messenger(Serial);
#endif
CD74HC4067 mux(5, 6, 7, 8);  // mux(s0,s1,s2,s3), where s3 is the highest select line (MSB)

//----- DEFINITIONS ------

// AUTHOR ID - You need to request your unique authorkey and type in instead of XXXs below
// To receive your unique authorkey, issue the command "!deviceinfo" on the SPAD.neXt discord.
// This key will identify the device author and enable author-only functions like e.g. editing the device UI or
// device configuration database, if the current SPAD.neXt user is the device author.

String authkey = "AUTH KEY";

// GUID -  Unique Identifier identifying the device. It should follow the GUID-Registryformat e.g. "{A8AA15C5-7BB6-4AC6-A558-A88CAFB78729}".
// To create a GUID you can use this site: https://www.guidgenerator.com/online-guid-generator.aspx (enable braces and hyphens)

String guid = "{4e538f4a-ad53-475f-8724-182ecc80b1f8}";

// TYPE IN YOUR ARDUINO PINS FOR A LED AND A BUTTON HERE

const int signal_pin = A9;  // Pin Connected to Sig pin of CD74HC4067
const int led_pin = 3;

const int knob1_mux_min = 0;
const int knob1_mux_max = 6;
const int button1_mux = 7;
const int switch1_mux_min = 8;
const int switch1_mux_max = 9;
const int switch1_mux_down = 8;
const int switch1_mux_up = 9;

const int knob1_spad_id = 1000;
const int switch1_base_spad_id = 2000;
const int button1_spad_id = 3000;

//---------- VARIABLES ---------------

int knob1_last = -1;
int switch1_last = 0;
int button1_last = 0;

bool isStarted = false;

//------------------- CALLBACKS and SPAD CONFIG ------------------------

#ifndef DEBUG
void attachCommandCallbacks() {
  messenger.sendCmd(3, "ATTACHING CALLBACKS!");
  messenger.attach(0, onIdentifyRequest);
  messenger.attach(2, onSpadEvent);
  messenger.attach(3, onUnknownCommand);
  messenger.attach(6, onSpadLedUpdate);
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
    messenger.sendCmdArg(guid);                 // GUID
    messenger.sendCmdArg("Misc Panel");         // DEVICE DISPLAY NAME
    messenger.sendCmdArg(2);                    // SPAD SERIAL VERSION, DON'T CHANGE
    messenger.sendCmdArg("1");                  // DEVICE VERSION NUMBER
    messenger.sendCmdArg("AUTHOR=" + authkey);  // AUTHOR ID - Edit the variable on top part of the code and remove // at the beginning of this line to activate your  auth code.
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


    //----- CREATE KNOB 1 -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(knob1_spad_id);  // This is the button ID
    messenger.sendCmdArg("KNOB_A");       //SPAD GUI Display name
    messenger.sendCmdArg("ROTARY");       // Type
    messenger.sendCmdArg("");             // Behaviour
    messenger.sendCmdArg("POS_NAMES=1#2#3#4#5#6#7");
    messenger.sendCmdArg("POS_VALUES=0#1#2#3#4#5#6");
    messenger.sendCmdEnd();

    //----- CREATE SWITCH 1 -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(switch1_base_spad_id + switch1_mux_up);  // This is the button ID
    messenger.sendCmdArg("SWITCH_A_UP");                          //SPAD GUI Display name
    messenger.sendCmdArg("PUSHBUTTON");                           // Type
    messenger.sendCmdArg("SPAD_PUSHBUTTON");                      // Behaviour
    messenger.sendCmdEnd();

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(switch1_base_spad_id + switch1_mux_down);  // This is the button ID
    messenger.sendCmdArg("SWITCH_A_DOWN");                          //SPAD GUI Display name
    messenger.sendCmdArg("PUSHBUTTON");                             // Type
    messenger.sendCmdArg("SPAD_PUSHBUTTON");                        // Behaviour
    messenger.sendCmdEnd();

    //----- CREATE BUTTON 1 LED -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("OUTPUT");
    messenger.sendCmdArg(led_pin);         // This is the button ID
    messenger.sendCmdArg("BUTTON_A_LED");  //SPAD GUI Display name
    messenger.sendCmdArg("LED");           // Type
    messenger.sendCmdArg("SPAD_LED");      // Behaviour
    messenger.sendCmdArg("HIDDEN=1");
    messenger.sendCmdEnd();

    //----- CREATE BUTTON 1 -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(button1_spad_id);    // This is the button ID
    messenger.sendCmdArg("BUTTON_A");         //SPAD GUI Display name
    messenger.sendCmdArg("PUSHBUTTON");       // Type
    messenger.sendCmdArg("SPAD_PUSHBUTTON");  // Behaviour
    messenger.sendCmdArg("LED=BUTTON_A_LED");
    messenger.sendCmdEnd();

    //-----  END CONFIG ----

    messenger.sendCmd(0, "CONFIG");
    return;
  }
}
#endif

#ifdef DEBUG
void SendSpadEvent(int id, byte val) {
  Serial.print(id);
  Serial.print(":");
  Serial.println(val);
}
#else
void SendSpadEvent(int id, byte val) {
  messenger.sendCmdStart(8);  // Channel no for sending input state to SPAD NEXT
  messenger.sendCmdArg(id);   // Input ID
  messenger.sendCmdArg(val);  // Value for off state
  messenger.sendCmdEnd();
}
#endif

void onSpadLedUpdate() {
  int ledIndex = messenger.readInt32Arg();
  int onOff = messenger.readInt32Arg();

  digitalWrite(ledIndex, onOff);
}

void CheckKnob() {
  for (byte i = knob1_mux_min; i <= knob1_mux_max; i++) {
    mux.channel(i);
    delay(10);
    if (digitalRead(signal_pin)) {
      if (i != knob1_last) {
        knob1_last = i;
        SendSpadEvent(knob1_spad_id, i);
        break;
      }
    }
  }
}

void CheckSwitch() {
  byte switch_val = 0;

  for (byte i = switch1_mux_min; i <= switch1_mux_max; i++) {
    mux.channel(i);
    delay(10);

    if (digitalRead(signal_pin)) {
      switch_val = i;
      break;
    }
  }

  if (switch_val != switch1_last) {
    if (switch_val == 0) {
      SendSpadEvent(switch1_base_spad_id + switch1_last, 0);
    } else {
      SendSpadEvent(switch1_base_spad_id + switch_val, 1);
    }
    switch1_last = switch_val;
  }
}

void CheckButton() {
  mux.channel(button1_mux);
  delay(10);

  if (digitalRead(signal_pin) != button1_last) {
    button1_last = !button1_last;
    SendSpadEvent(button1_spad_id, button1_last);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(signal_pin, INPUT);

#ifdef DEBUG
  isStarted = true;
#else
  attachCommandCallbacks();
#endif
}

void loop() {
  if (isStarted) {
    CheckKnob();
    CheckSwitch();
    CheckButton();
  }

#ifndef DEBUG
  messenger.feedinSerialData();
#endif
}
