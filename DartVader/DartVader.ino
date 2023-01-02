#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(5, 6); // RX, TX

DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

// ST_CP pin 12
const int LatchPin = 4;
// SH_CP pin 11
const int ClockPin = 3;
// DS pin 14
const int DataPin = 2;

byte Data = 0; 

void increment()   //LEDs increment start from 0 - 5 
{
  int PinNo = 0;
  int Delay = 100; 
  
  for(PinNo = 0; PinNo < 4; PinNo++)
  {
    shiftWrite(PinNo, HIGH);
    delay(Delay);                
  }
  for(PinNo = 3; PinNo >= 0; PinNo--)
  {
    shiftWrite(PinNo, LOW);
    delay(Delay);                
  }
}

void OneByOne()  // LEDs Glow one by one from 0 to 5
{
  int PinNo = 0;
  int Delay = 1000; 
  
  for(PinNo = 0; PinNo < 8; PinNo++)
  {
    shiftWrite(PinNo, HIGH);
    delay(Delay); 
    shiftWrite(PinNo, LOW);    
  }
  for(PinNo = 7; PinNo >=0 ; PinNo--)
  {
    shiftWrite(PinNo, HIGH);
    delay(Delay); 
    shiftWrite(PinNo, LOW);    
  }

}

void shiftWrite(int Pin, boolean State) // Function is similar to digitalWrite 
{                                       // State-0/1 | Pin - Pin No.
  bitWrite(Data,Pin,State);             // Making Pin(Bit) 0 or 1
  shiftOut(DataPin, ClockPin, MSBFIRST, Data); // Data out at DataPin
  digitalWrite(LatchPin, HIGH);                // Latching Data
  digitalWrite(LatchPin, LOW);
}

void AllHigh()   // sets all High
{
  int PinNo = 0;
  for(PinNo = 0; PinNo < 8; PinNo++)
  {
   shiftWrite(PinNo, HIGH);  
  }
}

void AllLow()   // Sets all low
{
  int PinNo = 0;
  for(PinNo = 0; PinNo < 8; PinNo++)
  {
   shiftWrite(PinNo, LOW);  
  }
}

void SOS(){                  // All LEDs ON and OFF 10 times
  for (int x=0; x<3; x++){    
    AllHigh();
    delay(500);
    AllLow();
    delay(500);
  }
}

void setup()
{
   // Setup pins as Outputs
  pinMode(LatchPin, OUTPUT);
  pinMode(ClockPin, OUTPUT);
  pinMode(DataPin, OUTPUT);

  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  // Serial.println();
  // Serial.println(F("DFRobot DFPlayer Mini Demo"));
  // Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  // if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
  //   Serial.println(F("Unable to begin:"));
  //   Serial.println(F("1.Please recheck the connection!"));
  //   Serial.println(F("2.Please insert the SD card!"));
  //   while(true);
  // }
  // Serial.println(F("DFPlayer Mini online."));

  // myDFPlayer.volume(25);  //Set volume value. From 0 to 30
  SOS();
  AllHigh();

  //myDFPlayer.play(5);
}

void loop()
{

}


void updateShiftRegister()
{
   digitalWrite(LatchPin, LOW);
   shiftOut(DataPin, ClockPin, LSBFIRST, Data);
   digitalWrite(LatchPin, HIGH);
}

// this second function is to turn them off
void updateShiftRegister2() 
{ 
   digitalWrite(LatchPin, HIGH);
   shiftOut(DataPin, ClockPin, LSBFIRST, Data); //if we start with MSBFIRST in this function, then it would start from the most significant, that is the 1st pinout.
   digitalWrite(LatchPin, LOW);
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;

        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}