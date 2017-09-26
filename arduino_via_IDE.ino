/* 
 * rosserial Subscriber 
 * manages Sabertooth V1.0
 */

// Sabertooth Serial Init
#include <SabertoothSimplified.h>

// Mixed mode is for tank-style diff-drive robots.
// Only Packet Serial actually has mixed mode, so this Simplified Serial library
// emulates it (to allow easy switching between the two libraries).

SabertoothSimplified ST; // We'll name the Sabertooth object ST.
                         // For how to configure the Sabertooth, see the DIP Switch Wizard for
                         //   http://www.dimensionengineering.com/datasheets/SabertoothDIPWizard/start.htm
                         // Be sure to select Simplified Serial Mode for use with this library.
                         // This sample uses a baud rate of 9600.
                         //
                         // Connections to make:
                         //   Arduino TX->1  ->  Sabertooth S1
                         //   Arduino GND    ->  Sabertooth 0V
                         //   Arduino VIN    ->  Sabertooth 5V (OPTIONAL, if you want the Sabertooth to power the Arduino)
                         //
                         // If you want to use a pin other than TX->1, see the SoftwareSerial example.

// Local Vars (char==signed 8bit int (-127,128))
volatile char speed;
volatile char turn;

void inc_Speed( char new_speed){
  speed = new_speed;
  ST.drive(speed);
}

void inc_Turn( char new_turn){
  turn = new_turn;
  ST.turn(turn);
}

void setup()
{ 
  //IDE Serial connection
  Serial.begin(9600);
  
  // Sabertooth
  SabertoothTXPinSerial.begin(9600); // This is the baud rate you chose with the DIP switches.             
  
  ST.drive(0); // The Sabertooth won't act on mixed mode until
  ST.turn(0);  // it has received power levels for BOTH throttle and turning, since it
               // mixes the two together to get diff-drive power levels for both motors.
               // So, we set both to zero initially.
}

void loop()
{  
  delay(1);
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
    if(stringComplete) {
      switch (inputString[0]) {
        case 't':
          int t = int(inputString[1:]);
          inc_Turn(t);
          break;
        case 's':
          int s = int(inputString[1:]);
          inc_Speed(s);
          break;
      }
      stringComplete = false;
    }
  }
}

