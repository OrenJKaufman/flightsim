#include <light_CD74HC4067.h>
#include <CmdMessenger.h>
CmdMessenger messenger(Serial);

CD74HC4067 mux(5, 6, 7, 8); // mux(s0,s1,s2,s3), where s3 is the highest select line (MSB)
const int signal_pin = A9; // Pin Connected to Sig pin of CD74HC4067
unsigned short s[4];
short lastSpad[] = { -1, -1, -1 };
unsigned short lastKnob;

//----- DEFINITIONS ------

// AUTHOR ID - You need to request your unique authorkey and type in instead of XXXs below
// To receive your unique authorkey, issue the command "!deviceinfo" on the SPAD.neXt discord.
// This key will identify the device author and enable author-only functions like e.g. editing the device UI or
// device configuration database, if the current SPAD.neXt user is the device author.

String authkey = "AUTH KEY";

// GUID -  Unique Identifier identifying the device. It should follow the GUID-Registryformat e.g. "{A8AA15C5-7BB6-4AC6-A558-A88CAFB78729}".
// To create a GUID you can use this site: https://www.guidgenerator.com/online-guid-generator.aspx (enable braces and hyphens)

String guid = "{486B2EA0-7929-42D2-86E0-428F9FD0CE0D}";

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
    messenger.sendCmdArg("MUX PoC");  // DEVICE DISPLAY NAME
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
    messenger.sendCmdEnd();

    //----- CREATE KNOB -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(1000);                   // This is the button ID
    messenger.sendCmdArg("KNOB_A");               //SPAD GUI Display name
    messenger.sendCmdArg("AXIS");              // Type
    messenger.sendCmdArg("SPAD_AXIS");  // Behaviour
    messenger.sendCmdArg("AXIS_MIN=0");
    messenger.sendCmdArg("AXIS_MAX=100");
    messenger.sendCmdEnd();

    //----- CREATE SWITCH A -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(2000);                   // This is the button ID
    messenger.sendCmdArg("SWITCH_A");               //SPAD GUI Display name
    messenger.sendCmdArg("SWITCH3");              // Type
    messenger.sendCmdArg("");
    messenger.sendCmdArg("POS_NAMES=DOWN#UP#OFF");
    messenger.sendCmdEnd();

    //----- CREATE SWITCH B -----

    messenger.sendCmdStart(0);
    messenger.sendCmdArg("INPUT");
    messenger.sendCmdArg(3000);                   // This is the button ID
    messenger.sendCmdArg("SWITCH_B");               //SPAD GUI Display name
    messenger.sendCmdArg("SWITCH");              // Type
    messenger.sendCmdEnd();

    //-----  END CONFIG ----

    messenger.sendCmd(0, "CONFIG");
    return;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(signal_pin, INPUT);

  attachCommandCallbacks();
}

void loop() {
  if (isStarted) {
    bool useVal;
    // put your main code here, to run repeatedly:
    // loop through channels 0 - 15
      for (byte i = 0; i < 4; i++) {
          useVal = true;
          mux.channel(i);
          int val = analogRead(signal_pin);
          if (i == 0) {
            if (abs(lastKnob - val) > 5) {
              lastKnob = val;
            }
            else {
              useVal = false;
            }
          }
          if (useVal) {
            s[i] = val;
          }
      }

      int spadValue;

      // knob
      spadValue = map(s[0], 0, 1023, 0, 100);
      if (spadValue != lastSpad[0]) {
        lastSpad[0] = spadValue;
        messenger.sendCmdStart(8);   // Channel no for sending input state to SPAD NEXT
        messenger.sendCmdArg(1000);  // Input ID
        messenger.sendCmdArg(spadValue);
        messenger.sendCmdEnd();
      }

      // switch 3
      spadValue = 2;
      int switch3A = map(s[1], 0, 1023, 0, 1);
      int switch3B = map(s[2], 0, 1023, 0, 1);
      if (switch3A == 1) {
        spadValue = 0;
      }
      else if (switch3B == 1) {
        spadValue = 1;
      }

      if (spadValue != lastSpad[1]) {
        lastSpad[1] = spadValue;
        messenger.sendCmdStart(8);   // Channel no for sending input state to SPAD NEXT
        messenger.sendCmdArg(2000);  // Input ID
        messenger.sendCmdArg(spadValue);
        messenger.sendCmdEnd();
      }

      // switch
      spadValue = map(s[3], 0, 1023, 0, 1);
      if (spadValue != lastSpad[2]) {
        lastSpad[2] = spadValue;
        messenger.sendCmdStart(8);   // Channel no for sending input state to SPAD NEXT
        messenger.sendCmdArg(3000);  // Input ID
        messenger.sendCmdArg(spadValue);
        messenger.sendCmdEnd();
      }

      delay(100);
  }
  messenger.feedinSerialData();
}
