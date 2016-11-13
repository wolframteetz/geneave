#include <Servo.h>

String readString, servoString[16];
Servo servo[16]; // Servo #0...15
int servoPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
 
bool readComplete;

void setup() {
  Serial.begin(9600);
  readComplete = false;
  for (int cx=0; cx<16; cx++) {
    servo[cx].attach(servoPins[cx]);  // pin for servo control. servo 0 pin 2 - servo 1 pin 3 etc.
    servo[cx].write(0);      // reset servos
  }
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
    for (int i=0; i<16; i++)
    {
      servoString[i] = readString.substring(i*2, (i+1)*2); //get the first three characters
      int n;
      char carray[6]; // string to char array
      servoString[i].toCharArray(carray, sizeof(carray));
      n = atoi(carray);
      servo[i].write(n); //set servo positions
      Serial.println(String("Servo ")+(i+1)+String(" to ")+n);
    }
    readString = ""; readComplete = false;
  }
}
