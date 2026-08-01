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

String guid = "{d1bdd0fb-a648-49dd-98a5-f4e311cfbdf1}";

// TYPE IN YOUR ARDUINO PINS FOR A LED AND A BUTTON HERE

const int signal_pin = A9;  // Pin Connected to Sig pin of CD74HC4067

const int MUX_APU = 1;
const int MUX_ENGINE_START = 2;
const int MUX_POSITION = 0;
const int MUX_ENGINE_START_LEFT = 15;
const int MUX_WHEEL_WELL = 9;
const int MUX_WING = 8;
const int MUX_ANTI_COLLISION = 7;
const int MUX_LOGO = 6;
const int MUX_TAXI = 14;
const int MUX_TURNOFF_RIGHT = 13;
const int MUX_TURNOFF_LEFT = 12;
const int MUX_LANDING_RIGHT = 11;
const int MUX_LANDING_LEFT = 10;
const int MUX_ENGINE_START_RIGHT = 3;


const int SPAD_ID_LANDING_LEFT = 1000;
const int SPAD_ID_LANDING_RIGHT = 2000;
const int SPAD_ID_TURNOFF_LEFT = 3000;
const int SPAD_ID_TURNOFF_RIGHT = 4000;
const int SPAD_ID_TAXI = 5000;
const int SPAD_ID_APU = 6000;
const int SPAD_ID_ENGINE_START_LEFT = 7000;
const int SPAD_ID_ENGINE_START = 8000;
const int SPAD_ID_ENGINE_START_RIGHT = 9000;
const int SPAD_ID_LOGO = 10000;
const int SPAD_ID_POSITION = 11000;
const int SPAD_ID_ANTI_COLLISION = 12000;
const int SPAD_ID_WING = 13000;
const int SPAD_ID_WHEEL_WELL = 14000;


const int MULTI_STATE_1 = 340;
const int MULTI_STATE_2 = 203;
const int MULTI_STATE_3 = 144;
const int MULTI_STATE_RANGE = 25;


const int DEBOUNCE_COUNT = 1;


int MuxMultiStateRead(int muxChannel) {
  mux.channel(muxChannel);
  delay(10);
  int data = analogRead(signal_pin);

  if (data > MULTI_STATE_1 + MULTI_STATE_RANGE) return 1;
  if (data > MULTI_STATE_2 + MULTI_STATE_RANGE) return 2;
  if (data > MULTI_STATE_3 + MULTI_STATE_RANGE) return 3;
  return 0;
}

int MuxDigitalRead(int muxChannel) {
  mux.channel(muxChannel);
  delay(10);
  return digitalRead(signal_pin);
}

//---------- VARIABLES ---------------

int last_landing_left, last_landing_right, last_turnoff_left, last_turnoff_right, last_taxi, last_apu, last_engine_start_left, last_engine_start, last_engine_start_right, last_logo, last_position, last_anti_collision, last_wing, last_wheel_well = -1;
int debounce_last_engine_start_left, debounce_last_engine_start_right = -1;
int debounce_engine_start_left, debounce_engine_start_right = 0;

bool isStarted = false;

//------------------- CALLBACKS and SPAD CONFIG ------------------------

#ifndef DEBUG
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
    messenger.sendCmdArg(guid);                 // GUID
    messenger.sendCmdArg("737 Panel 1");         // DEVICE DISPLAY NAME
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


    //----- CREATE LANDING SWITCH LEFT -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(SPAD_ID_LANDING_LEFT);                   // This is the button ID
    messenger.sendCmdArg("LANDING_LEFT");                          //SPAD GUI Display name
    messenger.sendCmdArg("SWITCH");                           // Type
    messenger.sendCmdEnd();

    //----- CREATE LANDING SWITCH RIGHT -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(SPAD_ID_LANDING_RIGHT);                   // This is the button ID
    messenger.sendCmdArg("LANDING_RIGHT");                          //SPAD GUI Display name
    messenger.sendCmdArg("SWITCH");                           // Type
    messenger.sendCmdEnd();

    //----- CREATE TURNOFF LEFT -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(SPAD_ID_TURNOFF_LEFT);                   // This is the button ID
    messenger.sendCmdArg("TURNOFF_LEFT");                          //SPAD GUI Display name
    messenger.sendCmdArg("SWITCH");                           // Type
    messenger.sendCmdEnd();

    //----- CREATE TURNOFF RIGHT -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(SPAD_ID_TURNOFF_RIGHT);                   // This is the button ID
    messenger.sendCmdArg("TURNOFF_RIGHT");                          //SPAD GUI Display name
    messenger.sendCmdArg("SWITCH");                           // Type
    messenger.sendCmdEnd();

    //----- CREATE TAXI -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(SPAD_ID_TAXI);                   // This is the button ID
    messenger.sendCmdArg("TAXI");                          //SPAD GUI Display name
    messenger.sendCmdArg("SWITCH");                           // Type
    messenger.sendCmdEnd();

    //----- CREATE APU -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(SPAD_ID_APU);                   // This is the button ID
    messenger.sendCmdArg("APU");                          //SPAD GUI Display name
    messenger.sendCmdArg("SWITCH3");                           // Type
    messenger.sendCmdArg("");                      // Behaviour
    messenger.sendCmdArg("POS_NAMES=ON#START#OFF");
    messenger.sendCmdEnd();

    //----- CREATE ENGINE START LEFT -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(SPAD_ID_ENGINE_START_LEFT);  // This is the button ID
    messenger.sendCmdArg("ENGINE_START_LEFT");       //SPAD GUI Display name
    messenger.sendCmdArg("ROTARY");       // Type
    messenger.sendCmdArg("");             // Behaviour
    messenger.sendCmdArg("POS_NAMES=FLT#GRD#OFF#CONT");
    messenger.sendCmdArg("POS_VALUES=0#1#2#3");
    messenger.sendCmdEnd();

    //----- CREATE ENGINE START -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(SPAD_ID_ENGINE_START);                   // This is the button ID
    messenger.sendCmdArg("ENGINE_START");                          //SPAD GUI Display name
    messenger.sendCmdArg("SWITCH3");                           // Type
    messenger.sendCmdArg("");                      // Behaviour
    messenger.sendCmdArg("POS_NAMES=BOTH#LEFT#RIGHT");
    messenger.sendCmdEnd();
    
    //----- CREATE ENGINE START RIGHT -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(SPAD_ID_ENGINE_START_RIGHT);  // This is the button ID
    messenger.sendCmdArg("ENGINE_START_RIGHT");       //SPAD GUI Display name
    messenger.sendCmdArg("ROTARY");       // Type
    messenger.sendCmdArg("");             // Behaviour
    messenger.sendCmdArg("POS_NAMES=FLT#GRD#OFF#CONT");
    messenger.sendCmdArg("POS_VALUES=0#1#2#3");
    messenger.sendCmdEnd();

    //----- CREATE LOGO -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(SPAD_ID_LOGO);                   // This is the button ID
    messenger.sendCmdArg("LOGO");                          //SPAD GUI Display name
    messenger.sendCmdArg("SWITCH");                           // Type
    messenger.sendCmdEnd();

    //----- CREATE POSITION -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(SPAD_ID_POSITION);                   // This is the button ID
    messenger.sendCmdArg("POSITION");                          //SPAD GUI Display name
    messenger.sendCmdArg("SWITCH3");                           // Type
    messenger.sendCmdArg("");                      // Behaviour
    messenger.sendCmdArg("POS_NAMES=OFF#STEADY#STROBE");
    messenger.sendCmdEnd();

    //----- CREATE ANTI COLLISION -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(SPAD_ID_ANTI_COLLISION);                   // This is the button ID
    messenger.sendCmdArg("ANTI_COLLISION");                          //SPAD GUI Display name
    messenger.sendCmdArg("SWITCH");                           // Type
    messenger.sendCmdEnd();

    //----- CREATE WING -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(SPAD_ID_WING);                   // This is the button ID
    messenger.sendCmdArg("WING");                          //SPAD GUI Display name
    messenger.sendCmdArg("SWITCH");                           // Type
    messenger.sendCmdEnd();

    //----- CREATE WHEEL WELL -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(SPAD_ID_WHEEL_WELL);                   // This is the button ID
    messenger.sendCmdArg("WHEEL_WELL");                          //SPAD GUI Display name
    messenger.sendCmdArg("SWITCH");                           // Type
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

void CheckPanel() {
  int data = MuxDigitalRead(MUX_LANDING_LEFT);
  if (data != last_landing_left) {
    last_landing_left = data;
    SendSpadEvent(SPAD_ID_LANDING_LEFT, data);
  }

  data = MuxDigitalRead(MUX_LANDING_RIGHT);
  if (data != last_landing_right) {
    last_landing_right = data;
    SendSpadEvent(SPAD_ID_LANDING_RIGHT, data);
  }

  data = MuxDigitalRead(MUX_TURNOFF_LEFT);
  if (data != last_turnoff_left) {
    last_turnoff_left = data;
    SendSpadEvent(SPAD_ID_TURNOFF_LEFT, data);
  }

  data = MuxDigitalRead(MUX_TURNOFF_RIGHT);
  if (data != last_turnoff_right) {
    last_turnoff_right = data;
    SendSpadEvent(SPAD_ID_TURNOFF_RIGHT, data);
  }

  data = MuxDigitalRead(MUX_TAXI);
  if (data != last_taxi) {
    last_taxi = data;
    SendSpadEvent(SPAD_ID_TAXI, data);
  }

  data = MuxMultiStateRead(MUX_APU);
  if (data != last_apu) {
    last_apu = data;
    SendSpadEvent(SPAD_ID_APU, data);
  }

  data = MuxMultiStateRead(MUX_ENGINE_START_LEFT);
  if (data != debounce_last_engine_start_left) {
    debounce_last_engine_start_left = data;
    debounce_engine_start_left = 0;
  }
  if (data != last_engine_start_left) {
    if (debounce_engine_start_left < DEBOUNCE_COUNT) {
      debounce_engine_start_left++;
    }
    else {
      last_engine_start_left = data;
      SendSpadEvent(SPAD_ID_ENGINE_START_LEFT, data);
    }
  }

  data = MuxMultiStateRead(MUX_ENGINE_START);
  if (data != last_engine_start) {
    last_engine_start = data;
    SendSpadEvent(SPAD_ID_ENGINE_START, data);
  }

  data = MuxMultiStateRead(MUX_ENGINE_START_RIGHT);
  if (data != debounce_last_engine_start_right) {
    debounce_last_engine_start_right = data;
    debounce_engine_start_right = 0;
  }
  if (data != last_engine_start_right) {
    if (debounce_engine_start_right < DEBOUNCE_COUNT) {
      debounce_engine_start_right++;
    }
    else {
      last_engine_start_right = data;
      SendSpadEvent(SPAD_ID_ENGINE_START_RIGHT, data);
    }
  }

  data = MuxDigitalRead(MUX_LOGO);
  if (data != last_logo) {
    last_logo = data;
    SendSpadEvent(SPAD_ID_LOGO, data);
  }

  data = MuxMultiStateRead(MUX_POSITION);
  if (data != last_position) {
    last_position = data;
    SendSpadEvent(SPAD_ID_POSITION, data);
  }

  data = MuxDigitalRead(MUX_ANTI_COLLISION);
  if (data != last_anti_collision) {
    last_anti_collision = data;
    SendSpadEvent(SPAD_ID_ANTI_COLLISION, data);
  }

  data = MuxDigitalRead(MUX_WING);
  if (data != last_wing) {
    last_wing = data;
    SendSpadEvent(SPAD_ID_WING, data);
  }

  data = MuxDigitalRead(MUX_WHEEL_WELL);
  if (data != last_wheel_well) {
    last_wheel_well = data;
    SendSpadEvent(SPAD_ID_WHEEL_WELL, data);
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
    CheckPanel();
  }

#ifndef DEBUG
  messenger.feedinSerialData();
#endif
}
