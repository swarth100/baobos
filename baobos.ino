#include <Servo.h>
#include <time.h>
#include <stdlib.h>

/* Exit pin where the first servo motor's control can be found */
#define startingPin 2

#define updateMilliThreshold 100

/* Total number of servo motors */
#define servoNum 5

/* ServoObject struct holds servo motor instances together with their default angles */
struct servoObject {
  Servo servo;
  int maxAngle = 90;
  int minAngle = 45;
  int curAngle = 60;
  bool increasing = true;
  bool done = true;
};

/* Array of initialised servoObjects. Populated in setup */
struct servoObject servoArray[servoNum];

int pinNumber = startingPin;

long updateTime = 0;

void setup() {
  for(int i = 0; i < servoNum; i++) {
    servoArray[i].servo.attach(pinNumber);
    servoArray[i].servo.write(servoArray[i].minAngle);

    pinNumber ++;
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

  /* Get the current servo by reference in order to be able 
     to change the servoObject's fields */
  struct servoObject* curServo = &servoArray[index];
  curServo->done = false;

  /* Accordingly to the direction of motion, increase or decrease the servo's
     angle */
  if (curServo->increasing) {
    curServo->curAngle = curServo->curAngle + 1;
  } else {
    curServo->curAngle = curServo->curAngle - 1;
  }

  /* When the max of min angles are reached, invert the direction of motion */
  if (curServo->curAngle >= curServo->maxAngle) {
    curServo->increasing = false;
  } else if (curServo->curAngle <= curServo->minAngle) {
    curServo->done = true;
    curServo->increasing = true;
  }

  /* Write the newly determined angle to the servo */
  curServo->servo.write(curServo->curAngle);

  // Serial.println(curServo->increasing);
  // Serial.println(curServo->curAngle);
}

void loop() {

  long curMillis = millis();

  /* Trigger every updateMilliThreshold */
  if (curMillis - updateTime >= updateMilliThreshold) {
    /* Loop through each servo and trigger motion */
    for(int i = 0; i < servoNum; i++) {
      runServo(i);
    }

    updateTime = curMillis;
  }
}
