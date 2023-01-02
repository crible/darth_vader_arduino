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

void shiftWrite(int desiredPin, boolean desiredState)
{
  bitWrite(Data, desiredPin, desiredState);
 
  // Now we'll actually send that data to the shift register
  // shiftOut() function does all the hard work of
  // manipulating data and clock pins to move data
  // into shift register
  shiftOut(DataPin, ClockPin, MSBFIRST, Data);

  // Once data is in shift register, we still need to
  // make it appear at the outputs. We'll toggle the state of
  // latchPin, which will signal shift register to "latch"
  // data to outputs
  // latch activates on high-to-low transition
  digitalWrite(LatchPin, HIGH);
  digitalWrite(LatchPin, LOW);
}


void oneOnAtATime()
{
  int index;
  for (index = 0; index <= 7; index++)
  {
    shiftWrite(index, HIGH);    // turn LED on
    delay(500);        
    shiftWrite(index, LOW);    // turn LED off
  }
}

void oneAfterAnother()
{
  int index;
  // Turn all LEDs on
  for (index = 0; index <= 7; index++)
  {
    shiftWrite(index, HIGH);
    delay(500);                
  }

  // Turn all LEDs off
  for (index = 7; index >= 0; index--)
  {
    shiftWrite(index, LOW);
    delay(500);
  }
}

// Off all LEDs for 5 sec
void allOff()
{
  int index;
  for (index = 0; index <= 7; index++)
  {
    shiftWrite(index, LOW);    // Turn a LED off
  }
  delay(500*5);
}



// On all LEDs for 5 sec
void allOn()
{
  int index;
  for (index = 0; index <= 7; index++)
  {
    shiftWrite(index, HIGH);    // Turn a LED on
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

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(25);  //Set volume value. From 0 to 30

  Data = 0;

  for (int i = 0; i < 8; i++)
  {
    bitSet(Data, i);
    updateShiftRegister();
    delay(300);
  }
  updateShiftRegister();
  // delay(500);
  

  myDFPlayer.play(5);

  delay(3000);

  allOn();

}

void loop()
{

  // myDFPlayer.play(1);
}


void updateShiftRegister()
{
   digitalWrite(LatchPin, LOW);
   shiftOut(DataPin, ClockPin, LSBFIRST, Data);
   digitalWrite(LatchPin, HIGH);
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