// VERSA Homeschool 2019 
// Avoidance Bot 02 Add Sensors
// This program adds sensors to your build. 
// Some code samples are from the letscodeblacksburg.org project.

// Setup FS90R Servos 
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

const int turnDelay = 540;  // Delay for turnL() turnR() to achive close to 90 degrees (at full throttle)
const int backDelay = 200;  // Delay for slight back up before turning out of an obstacle

int paused = true;        // Program starts off in paused mode
int waspaused = true;     // Tracks state if we were previously paused (or not)

int accel = 45;              // acceleration figure for turns (if needed)
const int conaccel = accel;  // constant (if needed)

int slowed = false;

int obstacleL = false;
int obstacleR = false;
int obstacleC = false;

int run = true;

Servo servoL;  // create servo object to control a servo
Servo servoR;  // create servo object to control a servo

// Setup Sensors 
// https://www.makerguides.com/tutorials/hc-sr04-arduino/ 
// This is set up to direct connect power and ground for the HC-SR04 ping sensor to be
// on pins 10(5v) 11(trigger) 12(echo) 13(low/gnd)
//
//               ping
// HC-SR04 \--5v-trig-echo-GND--/
//             |   |   |    |
// Arduino     10  11  12   13

const int powrPin = 10;     // providing power to ping sensor from pin 10
const int pingPin = 11;     // pin we're sending the trigger/ping on
const int echoPin = 12;     // pin we're reading back the echo on
const int gnd1Pin = 13;     // simulated ground so we can safely plug the module into our arduinos

long dist = 0;

void setup() {
  // Attach Sensor 
  pinMode(gnd1Pin, OUTPUT);      // sets up ping module's inner GND pin on a low output, and
  digitalWrite(gnd1Pin, LOW);     // the outter GND pin to hard GND (on most arduinos)
  pinMode(powrPin, OUTPUT);
  digitalWrite(powrPin, HIGH);  // try to power the module from pin
  // SENSOR I/O PINS
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  pinMode(echoPin, OUTPUT);    // just to make sure
  digitalWrite(echoPin, LOW);  // we clear any previous settings
  pinMode(echoPin, INPUT);     // and then use it as INPUT
  delay(500);
  
  // Attach Servos
  servoL.attach(6);  // attaches the right servo on pin 6
  servoR.attach(4);  // attaches the left servo on pin 4
  
  Serial.begin(9600);
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
