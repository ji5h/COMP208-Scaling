#include "Adafruit_VL53L0X.h"
// #include <Encoder.h>
enum State {STATIONARY, FORWARD, ROTATE_RIGHT, ROTATE_LEFT, RAISE, LOWER, CLIMB};

State currentState = STATIONARY;


// //code for encoders
// Encoder motorFrontL (2,30);
// Encoder motorFrontR (3,32);

// long int posFrontL = 0;
// long int posFrontR = 0;

int gearingFrontL = 45;
int ticksPerRevolutionFrontL = 6000;

int gearingFrontR = 47;
int ticksPerRevolutionFrontR = 6067;

float wheelRadius = 29;
float wheelCircumference = 2 * 3.14 * wheelRadius;

//distance to move the robot
int distanceFrontL = 1000/5;
int distanceFrontR = 1000/5;

//-------------------------------------------------------------------------------------------------------------
int motEn1 = 13; // motor 1 enable pin for setting power
int motEn2 = 12;
int motEn3 = 11;
int motEn4 = 10;

//pins for the middle motors
int middleMotEnA = 9;
int middleMotEnB = 8;

int middleMotIn1 = 31;
int middleMotIn2 = 33;
int middleMotIn3 = 35;
int middleMotIn4 = 37;

int mot1a = 22; // mot1a and mot 1b to set direction for motor 1
int mot1b = 24;
int mot2a = 26;
int mot2b = 28;
int mot3a = 34;
int mot3b = 36;
int mot4a = 38;
int mot4b = 40;

//-------------------------------------------------------------------------------------------------------------

int linearActuator4a= 39; //1a and 1b for 1st linear actuator to set direction
int linearActuator4b= 41;
int linearActuator3a= 43;
int linearActuator3b= 45;
int linearActuator1a= 47;
int linearActuator1b= 49;
int linearActuator2a= 51;
int linearActuator2b= 53;

///linear actuator enable pins (pwm)
int linearAcuatorEn4 = 6;
int linearAcuatorEn3 = 7;
int linearAcuatorEn2 = 4;
int linearAcuatorEn1 = 5;

//-----------------------------------------------------------------------------------------------------------------------
//defining pins for sensors

int whiskerActuator1 = 52;
int whiskerActuator2 = 50;
int whiskerActuator3 = 48;
int whiskerActuator4 = 46;

int frontLDR = 44;

int leftUltraSonicEcho = 29;
int leftUltraSonicTrig = 27;

int rightUltraSonicEcho = 25;
int rightUltraSonicTrig = 23;

//-------------------------------------------------------------------------------------------------------------------------


//laser sensing code

// Objects for the VL53L0X sensors
Adafruit_VL53L0X tof = Adafruit_VL53L0X();

// Measurement data 
VL53L0X_RangingMeasurementData_t measure;

void setup() { 
  Serial.begin(115200);
  Serial.println("Initialising...");

  //initialising outputs to motors
  pinMode(motEn1, OUTPUT);
  pinMode(motEn2, OUTPUT);
  pinMode(motEn3, OUTPUT);
  pinMode(motEn4, OUTPUT);

  pinMode(middleMotEnA, OUTPUT);
  pinMode(middleMotEnB, OUTPUT);

  pinMode(middleMotIn1, OUTPUT);
  pinMode(middleMotIn2, OUTPUT);
  pinMode(middleMotIn3, OUTPUT);
  pinMode(middleMotIn4, OUTPUT);

  pinMode(mot1a, OUTPUT);
  pinMode(mot1b, OUTPUT);
  pinMode(mot2a, OUTPUT);
  pinMode(mot2b, OUTPUT);
  pinMode(mot3a, OUTPUT);
  pinMode(mot3b, OUTPUT);
  pinMode(mot4a, OUTPUT);
  pinMode(mot4b, OUTPUT);


  //initialising outputs to linear actuators
  pinMode(linearActuator1a, OUTPUT);
  pinMode(linearActuator1b, OUTPUT);
  pinMode(linearActuator2a, OUTPUT);
  pinMode(linearActuator2b, OUTPUT);
  pinMode(linearActuator3a, OUTPUT);
  pinMode(linearActuator3b, OUTPUT);
  pinMode(linearActuator4a, OUTPUT);
  pinMode(linearActuator4b, OUTPUT);

  //initialising sensors as input pins
  pinMode(whiskerActuator1, INPUT_PULLUP);
  pinMode(whiskerActuator2, INPUT_PULLUP);
  pinMode(whiskerActuator3, INPUT_PULLUP);
  pinMode(whiskerActuator4, INPUT_PULLUP);

  pinMode(frontLDR,INPUT);
  //initialise tof
  if(!tof.begin()) {
    Serial.println("Failed to initiate tof");
  }
  Serial.println("Initialisation done!");
}

void loop() {
  // allActuatorsUp();
  Serial.println("yo");
  tof.rangingTest(&measure, false);
  Serial.println(digitalRead(frontLDR));
  if (digitalRead(frontLDR) == 0) {
    //change state to the climb state
        Serial.println("Climbing");
        allMotStop();
        currentState = CLIMB;
  } else {
      //check to see if atleast one actuator is not completely lowered
      while (digitalRead(whiskerActuator1) == 0 || digitalRead(whiskerActuator2) == 0 || digitalRead(whiskerActuator3) == 0 || digitalRead(whiskerActuator4) == 0){
        allActuatorsDown();
        Serial.print(digitalRead(whiskerActuator1));
        Serial.print(digitalRead(whiskerActuator2));
        Serial.print(digitalRead(whiskerActuator3));
        Serial.print(digitalRead(whiskerActuator4));
        Serial.println("actuators are lowering");
      }
      allActuatorsStop();
      //code if whiskers dont work  
      // allActuatorsDown();
      // delay(20000);
      // allActuatorsStop();
      Serial.println("all actuators lowered");
      //check to see if there is a front wall
      
      while (measure.RangeMilliMeter > 1000) {
        currentState = FORWARD;
        tof.rangingTest(&measure, false); 
        Serial.print("forward ");
        Serial.println(measure.RangeMilliMeter);
        allMotStop();
        delay(1);
        allMotForward();
        //if front wall present and no stairs present turn right
      } 
      allMotStop();

      while (measure.RangeMilliMeter < 600 && digitalRead(frontLDR) != 0) {
        currentState = ROTATE_RIGHT;
        Serial.print("rotate right ");
        tof.rangingTest(&measure, false);
        Serial.println(measure.RangeMilliMeter);
        allMotStop();
        delay(1);
        motFrontLeftFwd();
        motMiddleLeftFwd();
        motRearLeftFwd();
        motFrontRightReverse();
        motMiddleRightReverse();
        motRearRightReverse();
      } 
      allMotStop();

    }
    
  
  
  
  

  



  switch (currentState) {
    case STATIONARY:
     allMotStop();
     allActuatorsStop();
     break;
    
    case FORWARD:
      
      break;
    
    case ROTATE_RIGHT:
      
      break;

    case ROTATE_LEFT:
      motFrontRightFwd();
      motMiddleRightFwd();
      motRearRightFwd();
      break;
    
    case RAISE:
      allActuatorsDown();
      break;

    case LOWER:
      allActuatorsUp();
      break;

    case CLIMB:
      //raising first actuator above stairs and lowering till it hits it
      actuatorOneUp();
      delay(30000);
      allMotForward();
      delay(10000);
      allMotStop();
      
      //check to see if the whisker sensor is pressed down
      while (digitalRead(whiskerActuator1) == 0) {
        actuatorOneDown();
        Serial.println(digitalRead(whiskerActuator1));
      } 
      actuatorOneStop();
      Serial.println("actuator one on stair");
      //moving the middle actuators above stairs and lowering them until each whisker detects ground
      middleActuatorsUp();
      actuatorFourDown(); // for some reason back two actuators only work if both are being sent power by hbridge
      delay(30000);
      allMotForward();
      delay(20000);
      allMotStop();
      while(digitalRead(whiskerActuator2) == LOW || digitalRead(whiskerActuator3) == LOW) {
        actuatorTwoDown();
        actuatorThreeDown();
        actuatorFourDown();
      }
      actuatorTwoStop();
      actuatorThreeStop();
      actuatorFourStop(); 
      Serial.println("actuator two on stair");
      Serial.println("actuator three on stair");

      //raise all legs to lift fourth leg over
      allActuatorsDown();
      delay(30000);
      allMotForward();
      delay(10000);
      allMotStop();
      Serial.println("actuator four on stair");
  }

}

void actuatorOneUp()
{   
  digitalWrite(linearActuator1a,HIGH);
  digitalWrite(linearActuator1b,LOW);
  analogWrite(linearAcuatorEn1, 230);
} 
  
void actuatorOneDown() {   
  digitalWrite(linearActuator1a,LOW);
  digitalWrite(linearActuator1b,HIGH);
  analogWrite(linearAcuatorEn1, 255); 
}

void actuatorOneStop () {
  digitalWrite(linearActuator1a,LOW);
  digitalWrite(linearActuator1b,LOW);  
}

void actuatorTwoUp() {
  digitalWrite(linearActuator2a,HIGH);
  digitalWrite(linearActuator2b,LOW);
  analogWrite(linearAcuatorEn2, 235);
}
     
void actuatorTwoDown() {
  digitalWrite(linearActuator2a,LOW);
  digitalWrite(linearActuator2b,HIGH);
  analogWrite(linearAcuatorEn2, 225);
}

void actuatorTwoStop () {
  digitalWrite(linearActuator2a,LOW);
  digitalWrite(linearActuator2b,LOW);  
}

void actuatorThreeUp() {
  digitalWrite(linearActuator3a,HIGH);
  digitalWrite(linearActuator3b,LOW);
  analogWrite(linearAcuatorEn3, 190);
}
     
void actuatorThreeDown() {
  digitalWrite(linearActuator3a,LOW);
  digitalWrite(linearActuator3b,HIGH);
  analogWrite(linearAcuatorEn3, 195);
}

void actuatorThreeStop () {
  digitalWrite(linearActuator3a,LOW);
  digitalWrite(linearActuator3b,LOW);  
}

void actuatorFourUp() {
  digitalWrite(linearActuator4a,LOW);
  digitalWrite(linearActuator4b,HIGH);
  analogWrite(linearAcuatorEn4, 100);
}
     
void actuatorFourDown() {
  digitalWrite(linearActuator4a,HIGH);
  digitalWrite(linearActuator4b,LOW);
  analogWrite(linearAcuatorEn4, 130);
}

void actuatorFourStop () {
  digitalWrite(linearActuator4a,LOW);
  digitalWrite(linearActuator4b,LOW);  
}


void allActuatorsDown(){
  actuatorOneDown();
  actuatorTwoDown();
  actuatorThreeDown();
  actuatorFourDown();
}

void allActuatorsUp(){
  actuatorOneUp();
  actuatorTwoUp();
  actuatorThreeUp();
  actuatorFourUp();
}

void allActuatorsStop(){
  actuatorOneStop();
  actuatorTwoStop();
  actuatorThreeStop();
  actuatorFourStop();
}

void middleActuatorsUp(){
  actuatorTwoUp();
  actuatorThreeUp();
}

void middleActuatorsDown(){
  actuatorTwoDown();
  actuatorThreeDown();
}

//functions for left control of the motors
void motFrontLeftFwd () {
  digitalWrite(mot1a, LOW);
  digitalWrite(mot1b, HIGH);
  analogWrite(motEn1, 175);
}

void motMiddleLeftFwd () {
  digitalWrite(middleMotIn3, HIGH);
  digitalWrite(middleMotIn4, LOW);
  analogWrite(middleMotEnB, 175);
}

void motRearLeftFwd () {
  digitalWrite(mot2a, HIGH);
  digitalWrite(mot2b, LOW);
  analogWrite(motEn2, 175);
}

//functions for right control of the motors
void motFrontRightFwd () {
  digitalWrite(mot4a, LOW);
  digitalWrite(mot4b, HIGH);
  analogWrite(motEn4, 175);
}

void motMiddleRightFwd () {
  digitalWrite(middleMotIn1, HIGH);
  digitalWrite(middleMotIn2, LOW);
  analogWrite(middleMotEnA, 175);
}

void motRearRightFwd () {
  digitalWrite(mot3a, HIGH);
  digitalWrite(mot3b, LOW);
  analogWrite(motEn3, 175);
}

//control for right motors to reverse
void motFrontRightReverse () {
  digitalWrite(mot4a, HIGH);
  digitalWrite(mot4b, LOW);
  analogWrite(motEn4, 175);
}

void motMiddleRightReverse () {
  digitalWrite(middleMotIn1, LOW);
  digitalWrite(middleMotIn2, HIGH);
  analogWrite(middleMotEnA, 175);
}

void motRearRightReverse () {
  digitalWrite(mot3a, LOW);
  digitalWrite(mot3b, HIGH);
  analogWrite(motEn3, 175);
}

void frontMotForward(){
  motFrontRightFwd();
  motFrontLeftFwd();
}

void middleMotForward(){
  motMiddleRightFwd();
  motMiddleLeftFwd();
}

void rearMotForward(){
  motRearRightFwd();
  motRearLeftFwd();
}

void allMotForward(){
  frontMotForward();
  middleMotForward();
  rearMotForward();
}

void allMotStop () {
  digitalWrite(mot1a, LOW);
  digitalWrite(mot1b, LOW);
  analogWrite(motEn1, 0);
  digitalWrite(mot2a, LOW);
  digitalWrite(mot2b, LOW);
  analogWrite(motEn2, 0);
  digitalWrite(mot3a, LOW);
  digitalWrite(mot3b, LOW);
  analogWrite(motEn3, 0);
  digitalWrite(mot4a, LOW);
  digitalWrite(mot4b, LOW);
  analogWrite(motEn4, 0);

  digitalWrite(middleMotIn1, LOW);
  digitalWrite(middleMotIn2, LOW);
  analogWrite(middleMotEnA, 0);
  digitalWrite(middleMotIn3, LOW);
  digitalWrite(middleMotIn4, LOW);
  analogWrite(middleMotEnB, 0);
}
