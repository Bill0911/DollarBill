//==========[Dollar Bill]==============================================================//
//==========[Hehehaha]=================================================================//

#include <Adafruit_NeoPixel.h> // neopixel library

#define PIN 13        // Digital pin Neopixels
#define NUM_PIXELS 4  // Number of Neopixels

Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned long startTime = 0;  // Variable to store the start time of the timer
unsigned long timePassed = 0; //this happends as it never blink before. That's why blinking time did not pass
unsigned long timeNow = 0; //Now it holds the actual current time before blinking
unsigned long delayNeverDie = 0; //this should be 0 as it does not have any delay before celebrate1 is to be performed

bool timerStarted = false;    // Flag to indicate if the timer has started
bool raceStarted = false;     // Flag to indicate if the robot should start or not
bool raceEnded = false;
bool isWhite = false;
bool isBlack = false;
bool isTestingT = false;
bool isTestingR = false;
bool isTurnLeft = false;
bool disabled = false;
bool isNotError = false;

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
int whiteValues[sensorCount];
int blackValues[sensorCount];
int thresholdValues[sensorCount];

int celebrationStep = 0; //We defined this on as it keeps track of following blinks;

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

  Serial.begin(9600);

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
  
  if(raceEnded) {
    timeNow = millis();
    if(timeNow - timePassed >= delayNeverDie){// This condition is satisfied as zero is greater or equal to 0 (delayNeverDie is initially set to 0)
      timePassed = timeNow; //the action is executed, and timePassed is updated to timeNow
     switch (celebrationStep){
      case 0:
        lightStop();
        delayNeverDie = 2000;
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
        stopRobot();
        celebrationStep = 0;
        break;
     //On the following loops, timePassed - timeNow will be the time elapsed since the last action
     //If this elapsed time is greater or equal to 0 to delayNeverDie, the condition is satisfied again
     }
    celebrationStep++;
   }
  } else if(isBlack) {
    moveBackwards();
    delay(450);
    moveGripper(130);
    moveBackwards();
    endRace();
  } else if(isWhite) {
    if(sensorValues[0] > BLACK || sensorValues[1] > BLACK){  
    isWhite = false;
    }else if(sensorValues[0] < BLACK && sensorValues[1] < BLACK){
    turnRight();
    }
  }else if(isTestingT){
    if (sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK && sensorValues[3] > BLACK && sensorValues[4] > BLACK && sensorValues[5] > BLACK && sensorValues[6] > BLACK && sensorValues[7] > BLACK){
      isBlack = true;
      isTestingT= false;
    } else{
      isWhite = true;
      isTestingT = false;
    }
  } else if (timerStarted && millis() - startTime <= 1350) {
    moveGripper(90);
    turnAround();
  } else if (raceStarted) {
    if (sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK && sensorValues[3] > BLACK && sensorValues[4] > BLACK && sensorValues[5] > BLACK && sensorValues[6] > BLACK && sensorValues[7] > BLACK) {
      moveForward();
      delay(120);// changed new delay
      isTestingT = true;
    } else if (isTestingR){
      if(sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK && sensorValues[3] > BLACK && sensorValues[4] > BLACK && sensorValues[5] > BLACK && sensorValues[6] > BLACK && sensorValues[7] > BLACK){
        isBlack = true;
        isTestingR = false;
      }else{
      isTestingR = false;
      isWhite = true;
      turnRight();
      delay(100);
      }
    } else if (sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK && sensorValues[3] > BLACK ||
    sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK){
      moveForward();
      delay(120);
      isTestingR = true;
    } else if (!isTestingR && sensorValues[3] > BLACK || !isTestingR && sensorValues[4] > BLACK) {
      moveForward();
    } else if (sensorValues[5] > BLACK || sensorValues[6] > BLACK || sensorValues[7] > BLACK){ 
      moveLeft();
    } else if (sensorValues[0] > BLACK || sensorValues[1] > BLACK || sensorValues[2] > BLACK){
      moveRight();
    } else {
      turnAround();
    } 
  } else if (!disabled && isNotError && distance > 23 && distance < 26) {
    delay(1000);
    startRace();
    startTimer();
    moveForward();
    disabled = true;
    isTurnLeft = true;
    delay(900);
    Serial.print("Start");
  } else if (!disabled && !isNotError && distance > 23 && distance < 26){
    isNotError = true;
    delay(1000);
  }
}


//==========[LaiLaiLaiLaLai]==============================================================//
//==========[Functions]===================================================================//


void startRace() {
  if (!raceStarted) {
    raceStarted = true;
  }
}

void endRace() {
  raceEnded = true;
}

void startTimer() {
  if (!timerStarted) {
    startTime = millis();
    timerStarted = true;
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
  analogWrite (motorPin3, 150);//
  digitalWrite(motorPin4, LOW);//
  pixels.clear();
  pixels.setPixelColor(2, pixels.Color(155, 255, 0)); // 
  pixels.show();
}

void moveLeft() {
  analogWrite (motorPin1, 150);//
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

void calibrateSensors() {
  Serial.println("Calibrating sensors...");
  Serial.println("Move the robot over black and white lines...");

  // Initialize min and max arrays
  int minValues[numSensors];
  int maxValues[numSensors];
  for (int i = 0; i < numSensors; i++) {
    minValues[i] = 1023;
    maxValues[i] = 0;
  }

  // Read sensor values while moving over lines
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) {  // Calibration for 5 seconds
    readSensorValues();
    for (int i = 0; i < numSensors; i++) {
      // Update min and max values
      if (sensorValues[i] < minValues[i]) {
        minValues[i] = sensorValues[i];
      }
      if (sensorValues[i] > maxValues[i]) {
        maxValues[i] = sensorValues[i];
      }
    }
  }

  // Calculate black and white values based on min and max
  for (int i = 0; i < numSensors; i++) {
    blackValues[i] = minValues[i];
    whiteValues[i] = maxValues[i];
    // Calculate threshold value
    thresholdValues[i] = (blackValues[i] + whiteValues[i]) / 2;
  }

  Serial.println("Calibration complete.");
}

void readSensorValues() {
  // Read analog sensor values
  for (int i = 0; i < numSensors; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
  }
}

void printCalibratedValues() {
  // Print calibrated values for debugging
  Serial.println("Calibrated Values:");
  Serial.print("Black Values: ");
  for (int i = 0; i < numSensors; i++) {
    Serial.print(blackValues[i]);
    Serial.print(" ");
  }
  Serial.println();
  Serial.print("White Values: ");
  for (int i = 0; i < numSensors; i++) {
    Serial.print(whiteValues[i]);
    Serial.print(" ");
  }
  Serial.println();
  Serial.print("Threshold Values: ");
  for (int i = 0; i < numSensors; i++) {
    Serial.print(thresholdValues[i]);
    Serial.print(" ");
  }
  Serial.println();
}

