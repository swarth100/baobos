#include <Servo.h>
#include <time.h>
#include <stdlib.h>

#define servoNum 5
#define maxRunningServos 5

struct servoObject {
  Servo servo;
  int maxAngle = 90;
  int minAngle = 45;
  int curAngle = 60;
  bool increasing = true;
  bool done = true;
};

struct servoObject servoArray[servoNum];

// twelve servo objects can be created on most boards

int pos = 0;
int randomIndex;

int startingPin = 2;

void setup() {
  for(int i = 0; i < servoNum; i++) {
    servoArray[i].servo.attach(startingPin);
    servoArray[i].servo.write(servoArray[i].minAngle);

    startingPin ++;
  }

  /* Manual settings */
  servoArray[0].minAngle = 60;
  servoArray[0].maxAngle = 95;
  
  servoArray[2].maxAngle = 95;

  servoArray[3].minAngle = 35;
  servoArray[3].maxAngle = 70;

  servoArray[4].minAngle = 45;

  /* Random number generation */
  srand(time(NULL));

  Serial.begin(9600);
}

void runServo (int index) {

  struct servoObject* curServo = &servoArray[index];
  curServo->done = false;

  if (curServo->increasing) {
    curServo->curAngle = curServo->curAngle + 1;
  } else {
    curServo->curAngle = curServo->curAngle - 1;
  }

  if (curServo->curAngle >= curServo->maxAngle) {
    curServo->increasing = false;
  } else if (curServo->curAngle <= curServo->minAngle) {
    curServo->done = true;
    curServo->increasing = true;
  }

  curServo->servo.write(curServo->curAngle);

  // Serial.println(curServo->increasing);
  // Serial.println(curServo->curAngle);
}

void loop() {
  for(int i = 0; i < servoNum; i++) {
    runServo(i);
  }

  delay(150);
}
