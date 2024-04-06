//==========[Dollar Bill]==============================================================//
//==========[Hehehaha]=================================================================//

#include <Adafruit_NeoPixel.h> // neopixel library

#define PIN 13        // Digital pin Neopixels
#define NUM_PIXELS 4  // Number of Neopixels

Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned long timeStarted = 0;  // Variable to store the start time of the timer
unsigned long timePassed = 0; //this happends as it never blink before. That's why blinking time did not pass
unsigned long timeNow = 0; //Now it holds the actual current time before blinking
unsigned long timeCelebration = 0; //this should be 0 as it does not have any delay before celebrate1 is to be performed

bool timerStarted = false;    // Flag to indicate if the timer has started
bool raceStarted = false;     // Flag to indicate if the robot should start or not
bool raceEnded = false;       // Flag to indicate if the robot should end the race
bool isWhite = false;         // Flag to indicate if the robot after having moved forward from a T turn or an L turn not all sensorvalues or none of the sensorvalues were black 
bool isBlack = false;         // Flag to indicate if the robot senses that it at the end of track because all sensorvalues are consistantly black
bool isTestingT = false;      // Flag to indicate if the robot goes to the right turn (T turn)
bool isTestingR = false;      // Flag to indicate if the robot goes to the right turn (R turn or called L turn)


const int motorPin1 = 3;   // Motor 1 control pin
const int motorPin2 = 5;   // Motor 1 control pin
const int motorPin3 = 6;   // Motor 2 control pin
const int motorPin4 = 9;   // Motor 2 control pin

const int trigPin = 2; // Connect Trig to digital pin 2
const int echoPin = 7; // Connect Echo to digital pin 7

const int servoPin = 10; // Servo control pin

const int sensorCount = 8; // Number of sensors in your analog line sensor
const int sensorPins[sensorCount] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Analog sensor pins
int sensorValues[sensorCount]; // Array to store sensor values

int celebrationStep = 0; //We defined this on as it keeps track of following blinks;

#define BLACK 900 // defines the threshold of when we say the colour sensor senses the colour black

void setup() {
  pinMode(motorPin1, OUTPUT); //left-wheel forward
  pinMode(motorPin2, OUTPUT); //left-wheel backward
  pinMode(motorPin3, OUTPUT); //right-wheel forward
  pinMode(motorPin4, OUTPUT); //right-wheel backward
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  for (int i = 0; i < sensorCount; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  Serial.begin(9600);

  pixels.begin();  // Initialize NeoPixels
  pixels.show();   // Initialize all pixels to 'off'
  pixels.setBrightness(50);  // Set NeoPixel brightness

  int distance_count = 0; 
  while (distance_count < 10)  // This while loops makes sure the robot does not get tricked into starting the line meze by errors and forces the robot to stop using the ultrasonic sensor after the start
  {
    // Triggering the sensor
  digitalWrite(trigPin, LOW);
  delay(100);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reading the pulse duration from Echo pin
  long duration = pulseIn(echoPin, HIGH);

  // Calculating distance in centimeters
  int distance = duration * 0.034 / 2; // We divided the duration multiplied by the speed of sound by 2 
  //because the signal travels to the target and back meaning it went over the distance twice. 
  if (distance > 23 && distance < 27) {// everytime we meassure a distance in between 23 and 27 the counter goes up.
    distance_count++;
    continue;
    }
  }// here we start the race and move forward up until where the black square is
  delay(1000);
  startRace();
  startTimer();
  moveForward();
  delay(900);
 }

//================[Loop]=======================//
void loop() {
  // Read sensor values of all 8 line sensors
  for (int i = 0; i < sensorCount; ++i) {
    sensorValues[i] = analogRead(sensorPins[i]);
  }
 
  if(raceEnded) {
    timeNow = millis();
    if(timeNow - timePassed >= timeCelebration){// This condition is satisfied as zero is greater or equal to 0 (delayNeverDie is initially set to 0)
      timePassed = timeNow; //the action is executed, and timePassed is updated to timeNow
     switch (celebrationStep){
      case 0:
        lightStop();
        timeCelebration = 2000;
        break;
      case 1:
        celebrate1();
        timeCelebration = 200;
        break;
      case 2:
        celebrate2();
        timeCelebration = 200;
        break;
      case 3:
        celebrate3();
        timeCelebration = 200;
        break;
      case 4:
        celebrate4();
        timeCelebration = 100;
        break;
      case 5:
        stopRobot();
        celebrationStep = 0;
        break;
     //On the following loops, timePassed - timeNow will be the time elapsed since the last action
     //If this elapsed time is greater or equal to 0 to timeCelebration, the condition is satisfied again
     }
    celebrationStep++;
   }
  } else if(isBlack) {//drops of the object and moves out of the way of the last robot
    moveBackwards();
    delay(250);
    moveGripper(130);
    moveBackwards();
    endRace();
  }
  else if(isWhite) {// turn right until atleast one of the right most two line sensors senses black
    if(sensorValues[0] > BLACK || sensorValues[1] > BLACK){  
    isWhite = false;
    }else if(sensorValues[0] < BLACK && sensorValues[1] < BLACK){
    turnRight();
    }
  }
  else if(isTestingT){
    if (sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK && sensorValues[3] > BLACK && sensorValues[4] > BLACK && sensorValues[5] > BLACK && sensorValues[6] > BLACK && sensorValues[7] > BLACK){
      // if it the sensors still sense that every sensor is black that means we reached the end of the track otherwise we turn right
      isBlack = true;
      isTestingT= false;
    } else{
      isWhite = true;
      isTestingT = false;
    }
  } 
  else if (timerStarted && millis() - timeStarted <= 1350) {
    // after having moved forward towards the square we close the gripper and turn left
    moveGripper(90);
    turnAround();
  } 
  else if (raceStarted) {// only do the maze solving logic when the race has been started
    if (sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK && sensorValues[3] > BLACK && sensorValues[4] > BLACK && sensorValues[5] > BLACK && sensorValues[6] > BLACK && sensorValues[7] > BLACK) {
      //if we see that all values are black move forward to see if it a T turn or the ending square to stop the race or turn right accordingly
      moveForward();
      delay(120);
      isTestingT = true;
    } 
    else if (isTestingR){
      if(sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK && sensorValues[3] > BLACK && sensorValues[4] > BLACK && sensorValues[5] > BLACK && sensorValues[6] > BLACK && sensorValues[7] > BLACK){
        // if it the sensors sense that every sensor is black that means we reached the end of the track otherwise we turn right
        isBlack = true;
        isTestingR = false;
      }else{
        isTestingR = false;
        isWhite = true;
        turnRight();
        delay(100);
      }
    } 
    else if (sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK && sensorValues[3] > BLACK || sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK){
      //if we see that all values are black move forward to see if it an R turn () or the ending square to stop the race or turn right accordingly
      moveForward();
      delay(120);
      isTestingR = true;
    } 
    else if (!isTestingR && sensorValues[3] > BLACK || !isTestingR && sensorValues[4] > BLACK) {
      // if 3 or 4 sense a black line move forward
      moveForward();
    } 
    else if (sensorValues[5] > BLACK || sensorValues[6] > BLACK || sensorValues[7] > BLACK){ 
      //if 5, 6 or 7 sense a black line while 3 or 4 don't sense a black line, move left
      moveLeft();
    } 
    else if (sensorValues[0] > BLACK || sensorValues[1] > BLACK || sensorValues[2] > BLACK){
      // if sensor 0, 1 or 2 sense a black line while sensor 5, 6, 7, 3, or 4 dont, move right
      moveRight();
    } 
    else {
      //otherwise turnaround to the left( so that it can make a 90 degree turn to the left but also a 180 to turn around
      //this should only happen when all the sensors sense no black line
      turnAround();
    } 
  } 
}





//==========[Basic Functions]=============================================================//

void startRace() {
  //Start the race only if it hasn't been started
  if (!raceStarted) {
    raceStarted = true;
  }
}

void endRace() {
  raceEnded = true;
}

void startTimer() {
  // Start the timer only if it hasn't been started already
  if (!timerStarted) {
    timeStarted = millis();
    timerStarted = true;
  }
}

//Activate the gripper
void moveGripper(int angle) {
  int pwmValue = map(angle, 0, 180, 0, 255);
  analogWrite(servoPin, pwmValue);
}


void stopRobot() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}
//keep in mind we use GRB and not RGB
void lightStop(){
  pixels.setPixelColor(1, pixels.Color(0, 255, 0)); // 
  pixels.setPixelColor(2, pixels.Color(0, 255, 0)); // 
  pixels.setPixelColor(3, pixels.Color(0, 255, 0)); //
  pixels.setPixelColor(0, pixels.Color(0, 255, 0)); //
  pixels.show();
}

void moveForward() {
  analogWrite (motorPin1, 205);
  digitalWrite(motorPin2, LOW);
  analogWrite (motorPin3, 240);
  digitalWrite(motorPin4, LOW);
  pixels.clear();
  pixels.setPixelColor(2, pixels.Color(155, 255, 0)); // 
  pixels.setPixelColor(3, pixels.Color(155, 255, 0)); //
  pixels.show();
}

void moveBackwards() { 
  digitalWrite(motorPin1, LOW);
  analogWrite (motorPin2, 170);
  digitalWrite(motorPin3, LOW);
  analogWrite (motorPin4, 180);
  pixels.clear();
  pixels.setPixelColor(1, pixels.Color(0, 255, 0)); //
  pixels.setPixelColor(0, pixels.Color(0, 255, 0)); //
  pixels.show();
}

void turnRight() {
  analogWrite (motorPin1, 120);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  analogWrite (motorPin4, 160);
  pixels.clear();
  pixels.setPixelColor(1, pixels.Color(155, 255, 0)); // 
  pixels.setPixelColor(2, pixels.Color(155, 255, 0)); //
  pixels.show();
}

void moveRight() {
  analogWrite (motorPin1, 225);//
  digitalWrite(motorPin2, LOW);//
  analogWrite (motorPin3, 110);//
  digitalWrite(motorPin4, LOW);//
  pixels.clear();
  pixels.setPixelColor(2, pixels.Color(155, 255, 0)); // 
  pixels.show();
}

void moveLeft() {
  analogWrite (motorPin1, 110);//
  digitalWrite(motorPin2, LOW);//
  analogWrite (motorPin3, 230);//
  digitalWrite(motorPin4, LOW);//
  pixels.clear();
  pixels.setPixelColor(3, pixels.Color(155, 255, 0)); //
  pixels.show();
}

// TurnAround also is TurnLeft
void turnAround() {
  digitalWrite(motorPin1, LOW);
  analogWrite (motorPin2, 170);
  analogWrite (motorPin3, 130);
  digitalWrite(motorPin4, LOW);
  pixels.clear();
  pixels.setPixelColor(3, pixels.Color(155, 255, 0)); //
  pixels.setPixelColor(0, pixels.Color(155, 255, 0)); //
  pixels.show();
}

void celebrate1(){
  pixels.setPixelColor(1, pixels.Color(0,255,0));
  pixels.setPixelColor(2, pixels.Color(0,70,0 ));
  pixels.setPixelColor(3, pixels.Color(0,70,0 ));
  pixels.setPixelColor(0, pixels.Color(0,70,0));
  pixels.show();  
}

void celebrate2(){
  pixels.setPixelColor(1, pixels.Color(0,70,0 ));
  pixels.setPixelColor(2, pixels.Color(0,255,0));
  pixels.setPixelColor(3, pixels.Color(0,70,0 ));
  pixels.setPixelColor(0, pixels.Color(0,70,0 ));
  pixels.show();
}


void celebrate3(){
  pixels.setPixelColor(1, pixels.Color(0,70,0 ));
  pixels.setPixelColor(2, pixels.Color(0,70,0 ));
  pixels.setPixelColor(3, pixels.Color(0,255,0));
  pixels.setPixelColor(0, pixels.Color(0,70,0 ));
  pixels.show();  
}


void celebrate4(){
  pixels.setPixelColor(1, pixels.Color(0,70,0 ));
  pixels.setPixelColor(2, pixels.Color(0,70,0 ));
  pixels.setPixelColor(3, pixels.Color(0,70,0 ));
  pixels.setPixelColor(0, pixels.Color(0,255,0));
  pixels.show();  
}                                                                                                                                                                                                     
