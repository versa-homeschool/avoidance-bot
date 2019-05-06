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

// int run = true;

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

// ***********************************************************
// ******* MAIN LOOP *****************************************
// ***********************************************************
// Runs forever...
void loop() {

  pauseNgo();                    // This uses the sensor as a "pause/start" toggle switch

  dist = getdist();              // looks with hping sensor to measure distance to any objects


  //****** If object detected between 2-7 inches away (and the system is not paused), then run the slowDown() function.
  if ( (dist > 1 && dist < 8) && (paused == false) ) {
    slowed = false;
    slowDown();    //quickly slows down and sets state to "slowed = true"
  }


  //****** If Slowed (due to obstacle) in slowDown(), next scan L&R for obstacle position (L || R)
  if (slowed == true && paused == false) {
    Serial.println("Entered if-slowed test.. about to scan R & L");
    scanLR();     // scans for obstacle and records
    slowed = 0;
    Serial.print("obstacleR="); Serial.println(obstacleR);
    Serial.print("obstacleC="); Serial.println(obstacleC);
    Serial.print("obstacleL="); Serial.println(obstacleL);
  }

  //******  If obstacle is on the left, center or right...
  while ( (obstacleR == true || obstacleC == true || obstacleL == true ) && (paused == false) ){
      Serial.println("While (obstacle L||R)...");   

    //*** If obstacle in center (front), but none on the Right OR Left, then turn left and continue
    if (obstacleR == false && obstacleL == false && obstacleC == true && paused == false) {
      Serial.println("obstacleC only action");   
      slowed = 0;
      backward();         // back up a little
      delay(backDelay);   //
      turnL();            // if only something in front of us, just default turn left
      obstacleC = false;  // reset
      obstacleR = false;  // reset
      obstacleL = false;  // reset
      forward();          // go forward
      break;              // breaks out of while loop (skipping other checks)
    }

    //*** If obstacle is on the Right only (and system is not paused), turn left and continue
    if (obstacleR == true && paused == false) {
      Serial.println("obstacleR action");   
      slowed = 0;
      backward();         // back up a little
      delay(backDelay);   //
      turnL();            // turn left
      obstacleC = false;  // reset
      obstacleR = false;  // reset
      obstacleL = false;  // reset
      forward();          // go forward
      break;              // breaks out of while loop (skipping other checks)
    }
  
    //*** If obstacle is on the Left only (and system is not paused), turn right and continue
    if (obstacleL == true && paused == false) {
      Serial.println("obstacleL action");   
      slowed = 0;
      backward();         // back up a little
      delay(backDelay);   //
      turnR();            // turn right
      obstacleC = false;  // reset
      obstacleR = false;  // reset
      obstacleL = false;  // reset
      forward();          // go forward
      break;              // breaks out of while loop (skipping other checks)
    }    
  }

  delay(100);
}

////////////// Everything below this is program functions called from the main loop

// ***********************************************************
// ***** turnR() *********************************************
// ***********************************************************
void turnR(){
    Serial.println("turnR()");
    servoL.write(forwardL);           // rotate L servo forward
    servoR.write(reverseR);           // rotate R servo reverse (which turns us right)
    delay(turnDelay);                 // wait for full turn
    servoR.write(stopR);              // stop
    servoL.write(stopL);              // stop 
}


// ***********************************************************
// ***** turnL() *********************************************
// ***********************************************************
void turnL(){
    Serial.println("turnL()");
    servoL.write(forwardR);           // rotate R servo forward
    servoR.write(reverseL);           // rotate L servo reverse (which turns us left)
    delay(turnDelay);                       // wait for full turn
    servoR.write(stopR);              // stop
    servoL.write(stopL);              // stop
}


// ***********************************************************
// ***** forward() *********************************************
// ***********************************************************
void forward(){
    Serial.println("forward()");
    servoL.write(reverseR);           // rotate R servo forward
    servoR.write(reverseL);           // rotate L servo forward
}

// ***********************************************************
// ***** backward() *********************************************
// ***********************************************************
void backward(){
    Serial.println("backward()");
    servoL.write(forwardR);           // rotate R servo forward
    servoR.write(forwardL);           // rotate L servo forward
}



// ***********************************************************
// ***** scanLR() *******************************************
// ***********************************************************
void scanLR() {
  Serial.println("Entered scanLR() ");
  int obstacleThold = 10;
  obstacleC = false;
  obstacleR = false;
  obstacleL = false;
  const int scandelay = 850;

  // Scan forward
  Serial.println("scan center");
  dist = getdist();
  if (dist <= obstacleThold) {
    obstacleC = true;
    Serial.print(dist);
    Serial.println("in.   obstacleC = true");
  }
  Serial.println("--");

  // Scan Right
  Serial.println("scan right");
  servoR.write(reverseR);           // rotate R servo reverse
  delay(scandelay-100);             // wait for turn
  dist = getdist();
  if (dist <= obstacleThold) {
    obstacleR = true;
    Serial.print(dist);
    Serial.println("in.   obstacleR = true");
  }
  servoR.write(forwardR);           // return center
  delay(scandelay-100);             // 
  Serial.println("--");
  servoR.write(stopR);              // stop
  servoL.write(stopL);              // stop
  delay(250);


   // Scan Left
  Serial.println("scan left");
  servoL.write(reverseL);           // rotate L servo reverse
  delay(scandelay+100);             // turns further left (gets out of perfect perpendicular objects, getting out to the left)
  dist = getdist();
  if (dist <= obstacleThold) {
    obstacleL = true;
    Serial.print(dist);
    Serial.println("in.   obstacleL = true");
  }
  servoL.write(forwardL);           // return center
  delay(scandelay+100);             // turns further left (gets out of perfect perpendicular objects, getting out to the left)
  Serial.println("--");
  servoR.write(stopR);              // stop
  servoL.write(stopL);              // stop
  delay(250);
}

// ***********************************************************
// ***** getdist() *******************************************
// ***********************************************************
long getdist()
{
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(pingPin, HIGH);      // start the outgoing ping
  delayMicroseconds(10);            // do the ping for 10uS
  digitalWrite(pingPin, LOW);       // stop doing the ping
  duration = pulseIn(echoPin, HIGH);  // grab the delay of return echo
  // convert the time into a distance
  inches = microsecondsToInches(duration);
  //cm = microsecondsToCentimeters(duration);
  //Serial.print(inches);
  //Serial.print("in, ");
  //Serial.print(cm);
  //Serial.print("cm");
  //Serial.println();

  return (inches);
}

// ***********************************************************
// ***** pauseNgo() ******************************************
// ***********************************************************
void pauseNgo() {

  // If unpaused, but object less than two inches, go into paused mode
  dist = getdist();
  if (paused == false && dist < 2){
        Serial.println("UNPAUSED - I GO UNTIL I SEE SOMETHING CLOSE...");
  }
  while (paused == false && dist < 2 ) {
    dist = getdist();
    if (dist < 2 ) {
      paused = 1;
      Serial.println("ALL STOP");
      // Stop
      servoL.write(stopL);                  // stop L servo
      servoR.write(stopR);                  // stop R servo
      paused = 1;
      waspaused = 0;
      delay(250);
    }
  }
  

  // Paused Loop & Unpausing Detection  
  dist = getdist();
  if (paused == true){
    Serial.println("PAUSED - UNTIL I SEE SOMETHING CLOSE...");
  }
  while (paused == true) {
    dist = getdist();
    Serial.print(dist);
    Serial.print(" ");
    if (dist < 2 ) {
      Serial.println("");
      Serial.println("ALL AHEAD");
      // Start moving forward, full speed
      servoL.write(forwardL);                  // forward L servo
      servoR.write(forwardR);                  // forward R servo
      paused = 0;
      waspaused = 1;
    }
  }
  
  
}

// ***********************************************************
// ***** slowDown() ******************************************
// ***********************************************************
void slowDown() {
  Serial.println("ENTERED slowDown()");
  accel = 0;
  for ( int x = 0 ; x < 8 ; x++ ) {
    accel =  accel + 5;
    servoL.write(forwardL - accel);                // slow L servo
    servoR.write(forwardR + accel);                // slow R servo
    delay(50);
  }
  servoL.write(stopL);                  // stop L servo
  servoR.write(stopR);                  // stop R servo
  accel = conaccel;
  slowed = 1;
  waspaused = 0;
}

// Original code from the ping sensor library
long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
