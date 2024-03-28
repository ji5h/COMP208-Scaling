#include <Encoder.h>

//code for encoders
Encoder motorBackL (2,4);
Encoder motorBackR (3,5);

Encoder motorFrontL (21,6);
Encoder motorFrontR (20,7);

long int posBackL = 0;
long int posBackR = 0;

int gearingBackL = 45;
int ticksPerRevolutionBackL = 2791;

int gearingBackR = 45;
int ticksPerRevolutionBackR = 2767;

float wheelRadius = 35;
float wheelCircumference = 2 * 3.14 * wheelRadius;

//distance to move the robot
int distanceBackL = 1000/5;
int distanceBackR = 1000/5;

//code for powering motors
//right
int in1 = 12;
int in2 = 13;
int enA = 6;

//left
int in3 = 10;
int in4 = 11;
int enB = 9;

float calcTicksPerDistance(int distance, int ticksPerRev) {
  float numTicks = (distance / wheelCircumference) * ticksPerRev;
  return numTicks;
}

void rightMotorDrive(int power) {
  // check the input value is in the right range
  if (power > 255) {
    power = 255;
  }
  if (power < -255) {
    power = -255;
  }

  // set the h-bridge signals depending on the sign of the input value
  if (power == 0) {
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
  if (power > 0) {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  if (power < 0) {
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }

  // write the motor power to the enable pin
  analogWrite(enA, abs(power));
}

void leftMotorDrive(int power) {
  // check the input value is in the right range
  if (power > 255) {
    power = 255;
  }
  if (power < -255) {
    power = -255;
  }

  // set the h-bridge signals depending on the sign of the input value
  if (power == 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
  if (power > 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  if (power < 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }

  // write the motor power to the enable pin
  analogWrite(enB, abs(power));
}



void forward(int leftTicks,int rightTicks) {
  if(posL < leftTicks) {
    leftMotorDrive(255);
  } 
  else {
    leftMotorDrive(0);
    motorL.write(0);
  }

  if(posR < rightTicks) {
    rightMotorDrive(255);
  } 
  else {
    rightMotorDrive(0);
    motorR.write(0);
  }
}

void left() {
  
}

void setup() {
  Serial.begin(9600);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
  float leftTicks = calcTicksPerDistance(distanceL,ticksPerRevolutionL);
  float rightTicks = calcTicksPerDistance(distanceR,ticksPerRevolutionR);

  posL = motorL.read();
  posR = motorR.read();
  Serial.println(posL);

  forward(leftTicks,rightTicks);
}

