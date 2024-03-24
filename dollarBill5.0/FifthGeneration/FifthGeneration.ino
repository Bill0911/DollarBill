#include <Adafruit_NeoPixel.h> // neopixel library

#define PIN 13        // Digital pin Neopixels
#define NUM_PIXELS 4  // Number of Neopixels

Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned long startTime = 0;  // Variable to store the start time of the timer
unsigned long timePassed = 0; //this happends as it never blink before. That's why blinking time did not pass
unsigned long timeNow = 0; //Now it holds the actual current time before blinking
unsigned long delayNeverDie = 0; //this should be 0 as it does not have any delay before celebrate1 is to be performed
int celebrationStep = 0; //We defined this on as it keeps track of following blinks;

bool timerStarted = false;    // Flag to indicate if the timer has started
bool raceStarted = false;     // Flag to indicate if the robot should start or not
bool raceEnded = false;  //Flag to indicate when the race is done
bool isWhite = false;    //Flag to track the color of the track is white
bool isBlack = false;    //Flag to track the color of the track is black
bool isTestingT = false; //Flag to track where is the T turn (case for both left and right turn)
bool isTestingR = false; //Flag to track where is the R turn (right turn)
bool disabled = false;  //Flag to know that the robot performs only once

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

#define BLACK 900 // defines the threshold of when we say the colour sensor senses the colour black

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  for (int i = 0; i < sensorCount; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  pixels.begin();  // Initialize NeoPixels
  pixels.show();   // Initialize all pixels to 'off'
  pixels.setBrightness(50);  // Set NeoPixel brightness
}
void loop() {
  // Read sensor values
  for (int i = 0; i < sensorCount; ++i) {
    sensorValues[i] = analogRead(sensorPins[i]);
  }

  // Triggering the sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reading the pulse duration from Echo pin
  long duration = pulseIn(echoPin, HIGH);

  float theSpeedOfSound = 0.034; //centimeters per microsecond
  // Calculating distance in centimeters
  int distance = duration * theSpeedOfSound / 2; //We divided it by 2 
  //because the signal has travelled to the target and back, 
  //so the distance is the total round-trip distance

  // Print sensor values and distance to the Serial Monitor
  Serial.print("Sensor Values: ");
  for (int i = 0; i < sensorCount; ++i) {
    Serial.print(sensorValues[i]);
    Serial.print(" ");
  }
  
  Serial.println();
  
  Serial.print("Distance:");
  Serial.print(distance);
  Serial.print("cm");
  
  
  if(raceEnded) {
    timeNow = millis();
    if(timeNow - timePassed >= delayNeverDie){// This condition is satisfied as zero is greater or equal to 0 (delayNeverDie is initially set to 0)
      timePassed = timeNow; //the action is executed, and timePassed is updated to timeNow
     switch (celebrationStep){
      case 0:
        lightStop();
        stopRobot();
        break;
      case 1:
        celebrate1();
        delayNeverDie = 200;
        break;
      case 2:
        celebrate2();
        delayNeverDie = 200;
        break;
      case 3:
        celebrate3();
        delayNeverDie = 200;
        break;
      case 4:
        celebrate4();
        delayNeverDie = 100;
        break;
      case 5: 
        celebrationStep = 0;
        break;
     //On the following loops, timePassed - timeNow will be the time elapsed since the last action
     //If this elapsed time is greater or equal to 0 to delayNeverDie, the condition is satisfied again
     }
    celebrationStep++;//replicate the series of celebrating
   }
  } else if(isBlack) { //if this condition is satisfied, the robot will drop cargo
    lightBackwards();
    moveBackwards();
    delay(325);
    moveGripper(130);
    lightBackwards();
    moveBackwards();
    delay(1000);
    endRace();
  } else if(isWhite) { //if this condition is satisfied, the robot will move on to turn left or right
    isWhite = false; //We set it to false so that this condition will reset 
    lightForward();
    moveForward();
    delay(30);
    lightTRight();
    turnRight();
    delay(460);
  }else if(isTestingT && sensorValues[0] < BLACK && sensorValues[1] < BLACK && sensorValues[2] < BLACK && sensorValues[3] < BLACK && sensorValues[4] < BLACK && sensorValues[5] < BLACK && sensorValues[6] < BLACK && sensorValues[7] < BLACK){
    isWhite = true; // This condition is satisfied if the robot on the white surface (finish line)
    isTestingT = false;
  } else if (isTestingT && sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK && sensorValues[3] > BLACK && sensorValues[4] > BLACK && sensorValues[5] > BLACK && sensorValues[6] > BLACK && sensorValues[7] > BLACK){ 
    isBlack = true; // This condition is satisfied if the robot is on the black surface (right turn)
    isTestingT = false;
  } else if (timerStarted && millis() - startTime <= 1350) {
    moveGripper(90); // This condition is satisfied if the robot see a cargo and pick up
    lightAround();
    turnAround(); // This one is turn left, not turn around
    delay(350);
    lightForward();
    moveForward();
    delay(300);
  } else if (raceStarted) {
    if (sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK && sensorValues[3] > BLACK && sensorValues[4] > BLACK && sensorValues[5] > BLACK && sensorValues[6] > BLACK && sensorValues[7] > BLACK) {
      lightForward(); // this condition is satisfied when the robot is grabbing cargo then it start to solve the maze
      moveForward();
      delay(130);
      isTestingT = true;
    } else if (isTestingR && sensorValues[0] < BLACK && sensorValues[1] < BLACK && sensorValues[2] < BLACK && sensorValues[6] < BLACK && sensorValues[7] < BLACK || isTestingR && sensorValues[0] < BLACK && sensorValues[1] < BLACK && sensorValues[5] < BLACK && sensorValues[6] < BLACK && sensorValues[7] < BLACK){
      isTestingR = false; //The condition is satisfied if the robot is on the R rightTurn, it should turn right
      isWhite = true;
    } else if (isTestingR && sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK && sensorValues[3] > BLACK && sensorValues[4] > BLACK && sensorValues[5] > BLACK && sensorValues[6] > BLACK && sensorValues[7] > BLACK){
      isTestingR = false; //The condition is satisfied if the robot is on the testing to see whether its the finishline or not, if it is, drop cargo
      isBlack = true;
    } else if (!isTestingR && sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK && sensorValues[3] > BLACK || sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK){
      moveForward(); 
      lightForward();
      delay(110);
      isTestingR = true;
    } else if (!isTestingR && sensorValues[3] > BLACK || !isTestingR && sensorValues[4] > BLACK) {
      lightForward();// The condition is met if the robot is on the track and does not have any turn right on its head
      moveForward();
    } else if (sensorValues[5] > BLACK || sensorValues[6] > BLACK && sensorValues[7] > BLACK) {
      lightLeft();//This condition is met if the robot is slightly on the right side
      moveLeft();
    } else if (sensorValues[0] > BLACK || sensorValues[1] > BLACK || sensorValues[2] > BLACK) {
      lightRight();//This condition is met if the robot is slightly on the left side
      moveRight();
    } else if(sensorValues[0] < BLACK || sensorValues[1] < BLACK && sensorValues[2] < BLACK && sensorValues[3] < BLACK && sensorValues[4] < BLACK && sensorValues[5] < BLACK && sensorValues[6] < BLACK && sensorValues[7] < BLACK){
     lightAround(); //This condition is met if the robot goes to the last line
      turnAround();
    } else {
      lightTRight();// The rest of others should always turn right
      turnRight();
    } 
  } else if (!disabled && distance > 23 && distance < 27) {
    startRace();// This condition is met if the robot did not start yet until it see cargo right in front of their eyes
    startTimer();
    lightForward();
    moveForward();
    disabled = true;
    delay(900);
    Serial.print("Start");
  }
}
void startRace() {
  //Start the race only if it hasn't been started already
  if (!raceStarted) {
    raceStarted = true;
    Serial.println("race has been started");
  }
}

void endRace() {
  raceEnded = true;
}

void startTimer() {
  // Start the timer only if it hasn't been started already
  if (!timerStarted) {
    startTime = millis();
    timerStarted = true;
    Serial.println("Timer started");
  }
}

//Activated the gripper
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
void lightStop(){
  pixels.setPixelColor(1, pixels.Color(0, 255, 0)); // 
  pixels.setPixelColor(2, pixels.Color(0, 255, 0)); // 
  pixels.setPixelColor(3, pixels.Color(0, 255, 0)); //
  pixels.setPixelColor(0, pixels.Color(0, 255, 0)); //
  pixels.show();
}

void moveForward() {
  analogWrite (motorPin1, 200);
  digitalWrite(motorPin2, LOW);
  analogWrite (motorPin3, 242);
  digitalWrite(motorPin4, LOW);
}
void lightForward(){
  pixels.setPixelColor(1, pixels.Color(0, 0, 0)); // 
  pixels.setPixelColor(2, pixels.Color(155, 255, 0)); // 
  pixels.setPixelColor(3, pixels.Color(155, 255, 0)); //
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); //
  pixels.show();
}

void moveBackwards() { 
  digitalWrite(motorPin1, LOW);
  analogWrite(motorPin2, 170);
  digitalWrite(motorPin3, LOW);
  analogWrite(motorPin4, 180);
}
void lightBackwards(){
  pixels.setPixelColor(1, pixels.Color(0, 255, 0)); //
  pixels.setPixelColor(2, pixels.Color(0, 0, 0)); // 
  pixels.setPixelColor(3, pixels.Color(0, 0, 0)); // 
  pixels.setPixelColor(0, pixels.Color(0, 255, 0)); //
  pixels.show();
}

void turnRight() {
  analogWrite (motorPin1, 130);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, 100);
}
void lightTRight(){
  pixels.setPixelColor(1, pixels.Color(155, 255, 0)); // 
  pixels.setPixelColor(2, pixels.Color(155, 255, 0)); //
  pixels.setPixelColor(3, pixels.Color(0, 0, 0)); //  
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); //
  pixels.show();
}

void moveRight() {
  analogWrite (motorPin1, 113);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}
void lightRight(){
  pixels.setPixelColor(1, pixels.Color(0, 0, 0)); // 
  pixels.setPixelColor(2, pixels.Color(155, 255, 0)); // 
  pixels.setPixelColor(3, pixels.Color(0, 0, 0)); //
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); //
  pixels.show();
}

void moveLeft() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  analogWrite (motorPin3, 150);
  digitalWrite(motorPin4, LOW);
}
void lightLeft(){
  pixels.setPixelColor(1, pixels.Color(0, 0, 0)); // 
  pixels.setPixelColor(2, pixels.Color(0, 0, 0)); // 
  pixels.setPixelColor(3, pixels.Color(155, 255, 0)); //
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // 
  pixels.show();
}

// TurnAround also is TurnLeft
void turnAround() {
  digitalWrite(motorPin1, LOW);
  analogWrite (motorPin2, 170);
  analogWrite (motorPin3, 130);
  digitalWrite(motorPin4, LOW);
}
void lightAround(){
  pixels.setPixelColor(1, pixels.Color(0, 0, 0)); //
  pixels.setPixelColor(2, pixels.Color(0, 0, 0)); // 
  pixels.setPixelColor(3, pixels.Color(155, 255, 0)); //
  pixels.setPixelColor(0, pixels.Color(155, 255, 0)); //
  pixels.show();
}

void celebrate1(){
  pixels.setPixelColor(1, pixels.Color(0,255,0));
  pixels.setPixelColor(2, pixels.Color(0,70,0));
  pixels.setPixelColor(3, pixels.Color(0,70,0));
  pixels.setPixelColor(0, pixels.Color(0,70,0));
  pixels.show();  
}

void celebrate2(){
  pixels.setPixelColor(1, pixels.Color(0,70,0));
  pixels.setPixelColor(2, pixels.Color(0,255,0));
  pixels.setPixelColor(3, pixels.Color(0,70,0));
  pixels.setPixelColor(0, pixels.Color(0,70,0));
  pixels.show();
}


void celebrate3(){
  pixels.setPixelColor(1, pixels.Color(0,70,0));
  pixels.setPixelColor(2, pixels.Color(0,70,0));
  pixels.setPixelColor(3, pixels.Color(0,255,0));
  pixels.setPixelColor(0, pixels.Color(0,70,0));
  pixels.show();  
}


void celebrate4(){
  pixels.setPixelColor(1, pixels.Color(0,70,0));
  pixels.setPixelColor(2, pixels.Color(0,70,0));
  pixels.setPixelColor(3, pixels.Color(0,70,0));
  pixels.setPixelColor(0, pixels.Color(0,255,0));
  pixels.show();  
}
