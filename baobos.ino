#include <Servo.h>
#include <time.h>
#include <stdlib.h>

#define updateMilliThreshold 100

/* Total number of servo motors */
#define servoNum 5

/* ServoObject struct holds servo motor instances together with their default angles */
struct servoObject {
  Servo servo;
  int maxAngle = 90;
  int minAngle = 45;
  int curAngle = 60;
  int targetAngle = 60;
  bool done = true;
};

/* Array of initialised servoObjects. Populated in setup */
struct servoObject servoArray[servoNum];

long updateTime = 0;

int servoPins[] = {3, 2, 4, 5, 6};

void runServo (int index) {

  /* Get the current servo by reference in order to be able 
     to change the servoObject's fields */
  struct servoObject* curServo = &servoArray[index];

  if (!curServo->done) {
    if (curServo->targetAngle > curServo->curAngle) {
      curServo->curAngle = curServo->curAngle + 1;
    } else if (curServo->targetAngle < curServo->curAngle)  {
      curServo->curAngle = curServo->curAngle - 1;
    } else {
      curServo->done = true;
    }
  }

  curServo->servo.write(curServo->curAngle + millis() % 2);
}

int setTargetAngle(int percentage, int index) {
  struct servoObject* curServo = &servoArray[index];

  float baseAngle = (curServo->maxAngle - curServo->minAngle) * ((float) percentage / 100);

  int newAngle = (int) (baseAngle + curServo->minAngle);

  servoArray[index].targetAngle = newAngle;
  servoArray[index].done = false;
}

void reachTarget(int p1, int p2, int p3, int p4) {
  setTargetAngle(p1, 1);
  setTargetAngle(p2, 2);
  setTargetAngle(p3, 3);
  setTargetAngle(p4, 4);
}

void setup() {
  for(int i = 0; i < servoNum; i++) {
    servoArray[i].servo.attach(servoPins[i]);
    servoArray[i].servo.write(servoArray[i].minAngle);
  }

  /* Manual settings */
  servoArray[0].minAngle = 40;
  servoArray[0].maxAngle = 95;

  servoArray[1].maxAngle = 80;

  servoArray[2].minAngle = 35;
  servoArray[2].maxAngle = 95;

  servoArray[3].minAngle = 25;
  servoArray[3].maxAngle = 65;

  servoArray[4].minAngle = 45;
  servoArray[4].maxAngle = 110;

  /* Random number generation */
  srand(time(NULL));

  Serial.begin(115200);

  reachTarget(90, 140, 20, 45);
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

  if (Serial.available() >= 6) {
    uint8_t peekVal = Serial.peek();
    if (peekVal == 200) {
      Serial.write(201);
      Serial.read();
      Serial.write((uint8_t) Serial.read());
      Serial.write((uint8_t) Serial.read());
      Serial.write((uint8_t) Serial.read());
      Serial.write((uint8_t) Serial.read());
      Serial.write((uint8_t) Serial.read());
    } else {
      // Serial.write(Serial.read() + 1);
      Serial.read();
    }
  }
}
