// For a SPAD.neXt compatible Arduino library import CmdMessenger (4.0) into the Arduino IDE (recommended)

#include <CmdMessenger.h>
CmdMessenger messenger(Serial);

//----- ROTARIES -------

#define NUMROTARIES 2


struct rotariesdef {
  byte pin1;
  byte pin2;
  int ccwchar;
  int cwchar;
  volatile unsigned char state;
};

rotariesdef rotaries[NUMROTARIES] {
  {0,1,0,1,0},
  {3,2,2,3,0},
};


#define DIR_CCW 0x10
#define DIR_CW 0x20
#define R_START 0x0

#ifdef HALF_STEP
// Use the half-step state table (emits a code at 00 and 11)
#define R_CCW_BEGIN 0x1
#define R_CW_BEGIN 0x2
#define R_START_M 0x3
#define R_CW_BEGIN_M 0x4
#define R_CCW_BEGIN_M 0x5
const unsigned char ttable[6][4] = {
  // R_START (00)
  {R_START_M,            R_CW_BEGIN,     R_CCW_BEGIN,  R_START},
  // R_CCW_BEGIN
  {R_START_M | DIR_CCW, R_START,        R_CCW_BEGIN,  R_START},
  // R_CW_BEGIN
  {R_START_M | DIR_CW,  R_CW_BEGIN,     R_START,      R_START},
  // R_START_M (11)
  {R_START_M,            R_CCW_BEGIN_M,  R_CW_BEGIN_M, R_START},
  // R_CW_BEGIN_M
  {R_START_M,            R_START_M,      R_CW_BEGIN_M, R_START | DIR_CW},
  // R_CCW_BEGIN_M
  {R_START_M,            R_CCW_BEGIN_M,  R_START_M,    R_START | DIR_CCW},
};
#else
// Use the full-step state table (emits a code at 00 only)
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6

const unsigned char ttable[7][4] = {
  // R_START
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
  // R_CW_FINAL
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
  // R_CW_BEGIN
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
  // R_CW_NEXT
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
  // R_CCW_BEGIN
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
  // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
  // R_CCW_NEXT
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};
#endif

/* Call this once in setup(). */
void rotary_init() {
  for (int i=0;i<NUMROTARIES;i++) {
    pinMode(rotaries[i].pin1, INPUT);
    pinMode(rotaries[i].pin2, INPUT);
    #ifdef ENABLE_PULLUPS
      digitalWrite(rotaries[i].pin1, HIGH);
      digitalWrite(rotaries[i].pin2, HIGH);
    #endif
  }
}

/* Read input pins and process for events. Call this either from a
 * loop or an interrupt (eg pin change or timer).
 *
 * Returns 0 on no event, otherwise 0x80 or 0x40 depending on the direction.
 */
unsigned char rotary_process(int _i) {
   unsigned char pinstate = (digitalRead(rotaries[_i].pin2) << 1) | digitalRead(rotaries[_i].pin1);
  rotaries[_i].state = ttable[rotaries[_i].state & 0xf][pinstate];
  return (rotaries[_i].state & 0x30);
}

void CheckAllEncoders(void) {
  for (int i=0;i<NUMROTARIES;i++) {
    int rotaryId = 2000 + (i * 100);
    unsigned char result = rotary_process(i);
    if (result == DIR_CCW) {
      messenger.sendCmdStart(8); // Channel no for sending input state to SPAD NEXT
      messenger.sendCmdArg(rotaryId); // Input ID
      messenger.sendCmdArg(-1);
      messenger.sendCmdEnd();
    };
    if (result == DIR_CW) {
      messenger.sendCmdStart(8); // Channel no for sending input state to SPAD NEXT
      messenger.sendCmdArg(rotaryId); // Input ID
      messenger.sendCmdArg(1);
      messenger.sendCmdEnd();
    };
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

String guid = "{60E2DB58-7EA1-4976-B7C0-75D7F9962309}";


// TYPE IN YOUR ARDUINO PINS FOR A LED AND A BUTTON HERE

int button1pin = 18;
int button2pin = 19;
int button3pin = 20;
int button4pin = 21;
int knob1button = 8;
int knob2button = 9;


//---------- VARIABLES ---------------

int button1last = -1;
int button2last = -1;
int button3last = -1;
int button4last = -1;
int knob1buttonlast = -1;
int knob2buttonlast = -1;

bool isReady = false;
bool isStarted = false;

//------------------- CALLBACKS and SPAD CONFIG ------------------------

void attachCommandCallbacks()
{
    messenger.sendCmd(3, "ATTACHING CALLBACKS!");
    messenger.attach(0, onIdentifyRequest); 
    messenger.attach(2, onSpadEvent);       
    messenger.attach(3, onUnknownCommand);
}

void onUnknownCommand()
{
  messenger.sendCmd(3, "UNKNOWN COMMAND"); 
}


void onSpadEvent()
{
  char *szEvent = messenger.readStringArg();
  if (strcmp(szEvent, "START") == 0) { // SPAD tells device, it's ok to send input now
    isStarted = 1;
    return;
  }
  if (strcmp(szEvent, "END") == 0) { // SPAD tells device it will exit now
    isStarted = 0;
    return;
  }
}


void onIdentifyRequest()
{
  char *szRequest = messenger.readStringArg();

  if (strcmp(szRequest, "INIT") == 0)
    {
        messenger.sendCmdStart(0);
        messenger.sendCmdArg("SPAD");
        messenger.sendCmdArg(guid); // GUID  
        messenger.sendCmdArg("AP Panel"); // DEVICE DISPLAY NAME
        messenger.sendCmdArg(2); // SPAD SERIAL VERSION, DON'T CHANGE
        messenger.sendCmdArg("1"); // DEVICE VERSION NUMBER        
        messenger.sendCmdArg("AUTHOR="+authkey); // AUTHOR ID - Edit the variable on top part of the code and remove // at the beginning of this line to activate your  auth code.     
        messenger.sendCmdEnd();
        return;
    }
    
  if (strcmp(szRequest, "SCANSTATE") == 0)
    {
        messenger.sendCmdStart(0);
        messenger.sendCmdArg("STATESCAN");
        messenger.sendCmdEnd();
        return;
    }

  if (strcmp(szRequest, "PING") == 0){
    messenger.sendCmdStart(0);
    messenger.sendCmdArg("PONG");
    messenger.sendCmdArg(messenger.readInt32Arg());
    messenger.sendCmdEnd();
    return;
  }

  if (strcmp(szRequest, "CONFIG") == 0){
        messenger.sendCmdStart(0);
        messenger.sendCmdArg("OPTION");
        messenger.sendCmdArg("ISGENERIC=1");
        messenger.sendCmdArg("PAGESUPPORT=1");
        messenger.sendCmdArg("NO_DISPLAY_CLEAR=1");
        messenger.sendCmdEnd();


//----- CREATE ENCODER 1 -----

        messenger.sendCmdStart(0);
        messenger.sendCmdArg("INPUT");
        messenger.sendCmdArg(2000);  // This is the button ID
        messenger.sendCmdArg("KNOB_A");  //SPAD GUI Display name
        messenger.sendCmdArg("ENCODER"); // Type
        messenger.sendCmdArg("SPAD_ENCODER"); // Behaviour
        messenger.sendCmdEnd();

//----- CREATE ENCODER 2 -----

        messenger.sendCmdStart(0);
        messenger.sendCmdArg("INPUT");
        messenger.sendCmdArg(2100);  // This is the button ID
        messenger.sendCmdArg("KNOB_B");  //SPAD GUI Display name
        messenger.sendCmdArg("ENCODER"); // Type
        messenger.sendCmdArg("SPAD_ENCODER"); // Behaviour
        messenger.sendCmdEnd();

//----- CREATE KNOB 1 BUTTON -----

        messenger.sendCmdStart(0);
        messenger.sendCmdArg("INPUT");
        messenger.sendCmdArg(1000);  // This is the button ID
        messenger.sendCmdArg("KNOB_A_PUSH");  //SPAD GUI Display name
        messenger.sendCmdArg("PUSHBUTTON"); // Type
        messenger.sendCmdArg("SPAD_PUSHBUTTON"); // Behaviour
        messenger.sendCmdArg("HIDDEN=1");
        messenger.sendCmdArg("ROUTETO=KNOB_A");
        messenger.sendCmdEnd();

//----- CREATE KNOB 2 BUTTON -----

        messenger.sendCmdStart(0);
        messenger.sendCmdArg("INPUT");
        messenger.sendCmdArg(1100);  // This is the button ID
        messenger.sendCmdArg("KNOB_B_PUSH");  //SPAD GUI Display name
        messenger.sendCmdArg("PUSHBUTTON"); // Type
        messenger.sendCmdArg("SPAD_PUSHBUTTON"); // Behaviour
        messenger.sendCmdArg("HIDDEN=1");
        messenger.sendCmdArg("ROUTETO=KNOB_B");
        messenger.sendCmdEnd();

//------ CREATE BUTTON 1 -----

        messenger.sendCmdStart(0);
        messenger.sendCmdArg("INPUT");
        messenger.sendCmdArg(3000);  // This is the button ID
        messenger.sendCmdArg("BUTTON_1");  //SPAD GUI Display name
        messenger.sendCmdArg("PUSHBUTTON"); // Type
        messenger.sendCmdArg("SPAD_PUSHBUTTON");
        messenger.sendCmdEnd();

//------ CREATE BUTTON 2 -----

        messenger.sendCmdStart(0);
        messenger.sendCmdArg("INPUT");
        messenger.sendCmdArg(3100);  // This is the button ID
        messenger.sendCmdArg("BUTTON_2");  //SPAD GUI Display name
        messenger.sendCmdArg("PUSHBUTTON"); // Type
        messenger.sendCmdArg("SPAD_PUSHBUTTON");
        messenger.sendCmdEnd();

//------ CREATE BUTTON 3 -----

        messenger.sendCmdStart(0);
        messenger.sendCmdArg("INPUT");
        messenger.sendCmdArg(3200);  // This is the button ID
        messenger.sendCmdArg("BUTTON_3");  //SPAD GUI Display name
        messenger.sendCmdArg("PUSHBUTTON"); // Type
        messenger.sendCmdArg("SPAD_PUSHBUTTON");
        messenger.sendCmdEnd();

//------ CREATE BUTTON 4 -----

        messenger.sendCmdStart(0);
        messenger.sendCmdArg("INPUT");
        messenger.sendCmdArg(3300);  // This is the button ID
        messenger.sendCmdArg("BUTTON_4");  //SPAD GUI Display name
        messenger.sendCmdArg("PUSHBUTTON"); // Type
        messenger.sendCmdArg("SPAD_PUSHBUTTON");
        messenger.sendCmdEnd();

//-----  END CONFIG ----

      messenger.sendCmd(0, "CONFIG");
      return;
    }
}


// ------------------------ PROCESS FUNCTIONS--------------------


void CheckAllButtons(){

     if (digitalRead(button1pin) != button1last) 
      {
        if (button1last == 0)
        {
          button1last = 1;
          messenger.sendCmdStart(8); // Channel no for sending input state to SPAD NEXT
          messenger.sendCmdArg(3000); // Input ID
          messenger.sendCmdArg(0); // Value for off state
          messenger.sendCmdEnd();
        }
        else 
        {
          button1last = 0;
          messenger.sendCmdStart(8); // Channel no for sending input state to SPAD NEXT
          messenger.sendCmdArg(3000); // Input ID
          messenger.sendCmdArg(1); // Value for on state
          messenger.sendCmdEnd();
        }
        delay(25);
      }

      if (digitalRead(button2pin) != button2last) 
      {
        if (button2last == 0)
        {
          button2last = 1;
          messenger.sendCmdStart(8); // Channel no for sending input state to SPAD NEXT
          messenger.sendCmdArg(3100); // Input ID
          messenger.sendCmdArg(0); // Value for off state
          messenger.sendCmdEnd();
        }
        else 
        {
          button2last = 0;
          messenger.sendCmdStart(8); // Channel no for sending input state to SPAD NEXT
          messenger.sendCmdArg(3100); // Input ID
          messenger.sendCmdArg(1); // Value for on state
          messenger.sendCmdEnd();
        }
        delay(25);
      }

      if (digitalRead(button3pin) != button3last) 
      {
        if (button3last == 0)
        {
          button3last = 1;
          messenger.sendCmdStart(8); // Channel no for sending input state to SPAD NEXT
          messenger.sendCmdArg(3200); // Input ID
          messenger.sendCmdArg(0); // Value for off state
          messenger.sendCmdEnd();
        }
        else 
        {
          button3last = 0;
          messenger.sendCmdStart(8); // Channel no for sending input state to SPAD NEXT
          messenger.sendCmdArg(3200); // Input ID
          messenger.sendCmdArg(1); // Value for on state
          messenger.sendCmdEnd();
        }
        delay(25);
      }

      if (digitalRead(button4pin) != button4last) 
      {
        if (button4last == 0)
        {
          button4last = 1;
          messenger.sendCmdStart(8); // Channel no for sending input state to SPAD NEXT
          messenger.sendCmdArg(3300); // Input ID
          messenger.sendCmdArg(0); // Value for off state
          messenger.sendCmdEnd();
        }
        else 
        {
          button4last = 0;
          messenger.sendCmdStart(8); // Channel no for sending input state to SPAD NEXT
          messenger.sendCmdArg(3300); // Input ID
          messenger.sendCmdArg(1); // Value for on state
          messenger.sendCmdEnd();
        }
        delay(25);
      }

      if (digitalRead(knob1button) != knob1buttonlast) 
      {
        if (knob1buttonlast == 0)
        {
          knob1buttonlast = 1;
          messenger.sendCmdStart(8); // Channel no for sending input state to SPAD NEXT
          messenger.sendCmdArg(1000); // Input ID
          messenger.sendCmdArg(0); // Value for off state
          messenger.sendCmdEnd();
        }
        else 
        {
          knob1buttonlast = 0;
          messenger.sendCmdStart(8); // Channel no for sending input state to SPAD NEXT
          messenger.sendCmdArg(1000); // Input ID
          messenger.sendCmdArg(1); // Value for on state
          messenger.sendCmdEnd();
        }
        delay(25);
      }

      if (digitalRead(knob2button) != knob2buttonlast) 
      {
        if (knob2buttonlast == 0)
        {
          knob2buttonlast = 1;
          messenger.sendCmdStart(8); // Channel no for sending input state to SPAD NEXT
          messenger.sendCmdArg(1100); // Input ID
          messenger.sendCmdArg(0); // Value for off state
          messenger.sendCmdEnd();
        }
        else 
        {
          knob2buttonlast = 0;
          messenger.sendCmdStart(8); // Channel no for sending input state to SPAD NEXT
          messenger.sendCmdArg(1100); // Input ID
          messenger.sendCmdArg(1); // Value for on state
          messenger.sendCmdEnd();
        }
        delay(25);
      }
  }
  
// ------------------ M A I N  --------------------------------

void setup(){  
  
Serial.begin(115200);
  
pinMode(button1pin, INPUT_PULLUP);
pinMode(button2pin, INPUT_PULLUP);
pinMode(button3pin, INPUT_PULLUP);
pinMode(button4pin, INPUT_PULLUP);
pinMode(knob1button, INPUT);
pinMode(knob2button, INPUT);

rotary_init();

attachCommandCallbacks();
}

void loop(){
  if (isStarted)
  {

    CheckAllButtons();   
    CheckAllEncoders();     
  
  } 
  messenger.feedinSerialData();
}
