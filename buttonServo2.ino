#include <SoftwareSerial.h>       // import the serial library
#include <Servo.h>                // import the servo library

SoftwareSerial blueTooth(10, 11); // RX, TX
Servo myservo;                    // creates a servo object? not sure part of the sorce code for using servos
const int ledpin=13;              // led on D13 will show blink on / off
const int buttonPin=2;            // button attatched to pin 2
const int servoPin=7;             // servo attatched to pin 7
const int locked = 45;           // position of servo when door is locked
const int unlocked = 145;          // position of servo when door is unlocked
boolean unlock = false;           // is it time to unlock the door?
int pos = locked;                 // initalizes the position of the servo to the locked position
char a;                           // char input from bluetooth

void setup() {
   blueTooth.begin(9600);         // set the data rate for the SoftwareSerial port
   pinMode(ledpin,OUTPUT);        // sets the light on the aruino to be an output, isnt used right now
   pinMode(buttonPin, INPUT);     // sets the button's pin number to take input
   myservo.attach(servoPin);      // attatches a servo to the servo pin so it can be controlled
   blueTooth.println("hello");
}

void loop() {
   if (blueTooth.available()) {                      // if info arrived from blueTooth device
      a = (blueTooth.read());                        // gets info from the bluetooth device and sets that to one
      //int blues = (int) a;
      //blueTooth.println(blues);
      if (a == '0') {                                // if that info was '1'
         //blueTooth.println("unlock now");            // tell what your doing
         unlock = true;                              // set unlock to false
      }
      else if (a == '1') {                           // if that info was '2'
         //blueTooth.println("lock now");              // tell what your doing
         unlock = false;                             // set unlock to true
      }
   }
   if (digitalRead(buttonPin)==HIGH) {               // if the button is being pressed
      if (unlock) {                                  // and if unlock is already true
         unlock = false;                             // set unlock to false
      }
      else {                                         // if its not true
         unlock = true;                              // it is now
      }
   }
   while(pos >= locked && !unlock){                  // while position is less than the locked position and unlock is true
      myservo.write(pos);                            // tell servo to go to position in variable 'pos'
      delay(15);                                     // waits 15ms for the servo to reach the position
      if (pos==locked) {                           // if its on the last run through the loop
         blueTooth.println("L");        // send info for door is locked
      }
      pos--;                                         // increment the position
   }
   while(pos <= unlocked && unlock){                 // while the position is greater than the unlocked position and unlock is false
      myservo.write(pos);                            // tell servo to go to position in variable 'pos'
      delay(15);                                     // waits 15ms for the servo to reach the position
      if (pos==unlocked) {                             // if its on the last run through the loop
         blueTooth.println("U");    // send info for door is not locked
      }
      pos++;                                         // increment the position

   }
}

