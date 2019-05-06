// VERSA Homeschool 2019 
// Avoidance Bot 01 Run Prototype
// This program allows you to verify that your build and servos are correct 
//   and functional. 
// Some code samples are from the letscodeblacksburg.org project.

// ***********************************************************
// ******* Setup FS90R Servos ********************************
// ***********************************************************
#include <Servo.h>
// FS90R 180 full speed forward, 0 full speed backwards, 90 stop
// 135 is Clockwise
// 45 is Counter Clockwise
const int forwardL = 135;
const int reverseL = 45;
const int forwardR = 45;
const int reverseR = 135;
// 90 (or around 90) is stopped, but you may need to fine tune it for dead stop.
const int stopL = 90;
const int stopR = 90;
int run = true;

Servo servoL;  // create servo object to control a servo
Servo servoR;  // create servo object to control a servo

void setup() {
  // Attach Servos
  servoL.attach(6);  // attaches the right servo on pin 6
  servoR.attach(4);  // attaches the left servo on pin 5
}

void forward(){
    Serial.println("forward()");
    servoL.write(forwardL);           // rotate L servo forward
    servoR.write(forwardR);           // rotate R servo forward
}

void loop() {
  // put your main code here, to run repeatedly:
  while (run == true) {
    forward();
  }
}
