#include <Servo.h>

String readString, servo1, servo2;
Servo myservo1;
Servo myservo2;

bool readComplete;

void setup() {
  Serial.begin(9600);
  readComplete = false;
  myservo1.attach(2);  // pin for servo control
  myservo2.attach(3);
  myservo1.write(0);   // reset servos
  myservo2.write(0);
}

void loop() {
  while ((!readComplete) && Serial.available() > 0) {
    delay(2);
    char c = Serial.read();  //gets one byte from serial buffer
    if (c != 'X') {
      readString += c; //makes the string readString
    } else {
      readComplete = true;
    }
  }

  if (readComplete) {
    Serial.println(readString); //see what was received
    servo1 = readString.substring(0, 2); //get the first three characters
    servo2 = readString.substring(2, 4); //get the next three characters
    int n1;
    int n2;
    char carray1[6]; // string to char array
    servo1.toCharArray(carray1, sizeof(carray1));
    n1 = atoi(carray1);
    char carray2[6]; //
    servo2.toCharArray(carray2, sizeof(carray1));
    n2 = atoi(carray2);
    myservo1.write(n1); //set servo positions
    myservo2.write(n2);
    Serial.println(String("Servo 1 to ")+n1+String(", Servo 2 to ")+n2);
    readString = ""; readComplete = false;
  }
}
