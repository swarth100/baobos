#include <Servo.h>
#include <time.h>
#include <stdlib.h>

/* threshold for updates.
   Applies to both servo Motor communiction ans Serial port. */
#define updateMilliThreshold 100

/* Total number of servo motors */
#define servoNum 5

/* General global value definitions */
#define BAOBOS_ID 201
#define BAOBUI_ID 200
#define ANALOG_PRESSURE_PAD_THRESHOLD 100
#define BUFFER_DATA_LENGTH 6
#define BAUDRATE 115200

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

/* PINS available for servo motors, in a specific order */
int servoPins[] = {3, 2, 4, 5, 6};

/* If a given servo is available fro movement, it makes it step closer to its target angle */
void runServo (int index) {

  /* Get the current servo by reference in order to be able 
     to change the servoObject's fields */
  struct servoObject* curServo = &servoArray[index];

  /* Servo must not be in a completed state */
  if (!curServo->done) {
    if (curServo->targetAngle > curServo->curAngle) {
      curServo->curAngle = curServo->curAngle + 1;
    } else if (curServo->targetAngle < curServo->curAngle)  {
      curServo->curAngle = curServo->curAngle - 1;
    } else {
      curServo->done = true;
    }
  }

  /* Writes the new position to the servo motor.
     The given position could also be unchanged should the servo have been in
     a completed state.
     millis() % 2 is necessary to attempt to randomise the output so that the
     servo is actually at the correct position. */
  curServo->servo.write(curServo->curAngle + millis() % 2);
}

/* Sets a given servoMotor's angle as percentage of its movement range.
   Movement percentages are in range 0%-100%, where a 0% movement will
   force the servo to reach its minAngle. 100% movement will instead reach
   maxAngle.
   All percentages inbetween are scaled accordingly.*/
int setTargetAngle(int percentage, int index) {
  /* Retrieve the servo object corresponding to the correct index */
  struct servoObject* curServo = &servoArray[index];

  /* Determine the angle offset determined by the percentage.
     0 offset at 0%, (maxAngle - minAngle) offset at 100%. */
  float baseAngle = (curServo->maxAngle - curServo->minAngle) * ((float) percentage / 100);

  /* Add the given offset to minAngle to determine the correct new
     position of the servo Motor. */
  int newAngle = (int) (baseAngle + curServo->minAngle);

  /* Set the angle and allow movement on the servo */
  servoArray[index].targetAngle = newAngle;
  servoArray[index].done = false;
}

/* Forces all servo motors to reach a position expressed by the
   percentages p0-p4. 
   Should the percentages not differ from previous ones, the motors
   will not move. */
void reachTarget(int p0, int p1, int p2, int p3, int p4) {
  setTargetAngle(p0, 0);
  setTargetAngle(p1, 1);
  setTargetAngle(p2, 2);
  setTargetAngle(p3, 3);
  setTargetAngle(p4, 4);
}

/* Main setup loop for BAoBOS. */
void setup() {
  /* Iterate and initialise the appropriate number of servo Structs */
  for(int i = 0; i < servoNum; i++) {
    servoArray[i].servo.attach(servoPins[i]);
    /* It is necessary to write a default angle to each servo upon 
       initialistion to prevent undefined behavuiour */
    servoArray[i].servo.write(servoArray[i].minAngle);
  }

  /* Manual setting of servoMotor angles */

  /* Magnet servo */
  servoArray[0].minAngle = 40;
  servoArray[0].maxAngle = 95;

  /* Head z-axis servo */
  servoArray[1].maxAngle = 80;

  /* Upper arm servo */
  servoArray[2].minAngle = 35;
  servoArray[2].maxAngle = 95;

  /* Middle arm servo */
  servoArray[3].minAngle = 25;
  servoArray[3].maxAngle = 65;

  /* Base rotation servo */
  servoArray[4].minAngle = 40;
  servoArray[4].maxAngle = 110;

  /* Random number generation */
  srand(time(NULL));

  /* Begin communication over Serial Port with a baud
     rate of 115200*/
  Serial.begin(BAUDRATE);

  /* Sets a general position to reach should serial
     communication be slow to setup. This position 
     will roughly center the arm. */
  reachTarget(45, 70, 90, 20, 45);
}

/* Main Arduino loop */
void loop() {

  /* Initialise temporary values for the analog input readings */
  int b0 = 0;
  int b1 = 0;
  int b2 = 0;
  int b3 = 0;
  int b4 = 0;

  /* Temporary bool to handle serial port writing. By default set to
     true, will be invalidated should the write speed be faster than a
     given threshold */
  bool writeToSerial = true;

  /* Switch through all the analog readings, setting the input reading
     fields accordingly. */
  if (analogRead(A0) >= ANALOG_PRESSURE_PAD_THRESHOLD) {
    b0 = 1;
  }
  if (analogRead(A1) >= ANALOG_PRESSURE_PAD_THRESHOLD) {
    b1 = 1;
  }
  if (analogRead(A2) >= ANALOG_PRESSURE_PAD_THRESHOLD) {
    b2 = 1;
  }
  if (analogRead(A3) >= ANALOG_PRESSURE_PAD_THRESHOLD) {
    b3 = 1;
  }
  if (analogRead(A4) >= ANALOG_PRESSURE_PAD_THRESHOLD) {
    b4 = 1;
  }

  /* Retrieve the mmilliseconds elapsed since the beginning of the
     sessioon. */
  long curMillis = millis();

  /* Trigger every updateMilliThreshold */
  if (curMillis - updateTime >= updateMilliThreshold) {
    /* Loop through each servo and trigger motion */
    for(int i = 0; i < servoNum; i++) {
      runServo(i);
    }

    updateTime = curMillis;
  } else {
    /* Prevent writing to serial to avoid serial port data overflow */
    writeToSerial = false;
  }

  /* Wait for 6 bytes of data to have been sent over serial */
  if (Serial.available() >= BUFFER_DATA_LENGTH) {
    /* Peek at the first byte to determine if it's a valid read ID */
    uint8_t peekVal = Serial.peek();

    /* ID of 200 is used by BAoBUI */
    if (peekVal == BAOBUI_ID) {
      /* Read the servoAngles */
      Serial.read();
      int a0 = (uint8_t) Serial.read();
      int a1 = (uint8_t) Serial.read();
      int a2 = (uint8_t) Serial.read();
      int a3 = (uint8_t) Serial.read();
      int a4 = (uint8_t) Serial.read();

      /* Force an update on the servos with the new angles */
      reachTarget(a0, a1, a2, a3, a4);

      /* Force a reply */
      writeToSerial = true;      
    } else {
      Serial.read();
    }
  }

  /* When it is possible to reply to serial */
  if (writeToSerial) {
    /* Identification byte */
    Serial.write(BAOBOS_ID);

    /* Bytes of data with the statuses of the preassure pads. */
    Serial.write(b0);
    Serial.write(b1);
    Serial.write(b2);
    Serial.write(b3);
    Serial.write(b4);
  }
}
