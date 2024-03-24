#include <Adafruit_NeoPixel.h> // neopixel library

#define PIN 13        // Digital pin Neopixels
#define NUM_PIXELS 4  // Number of Neopixels

Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

bool isRight = false;                       //Flag to indicate if the robot should turn right or not
bool raceStarted = false;                  // Flag to indicate if the robot should start or not
bool isTestingR = false;                  // Flag to indicate if the robot is testing the right turn
bool isLeft = false;                     // Flag to indicate if the robot should turn left or not
bool raceEnded = false;                 // Flag to indicate if the robot should stop or not
bool isAttempingToGetOntTrack = false; // Flag to indicate if the robot is attempting to get on track

const int motorPin1 = 3;    // Motor 1 control pin
const int motorPin2 = 5;   // Motor 1 control pin
const int motorPin3 = 6;  // Motor 2 control pin
const int motorPin4 = 9; // Motor 2 control pin

const int trigPin = 2;  // Connect Trig to digital pin 2
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
  
  for (int i = 0; i < sensorCount; i++) { //Insead of setting up manually with lots of lines, we use a for loop to set up
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
  if(!raceStarted && distance > 23 && distance < 27){//if it sees the cargo, pick it 
    isAttempingToGetOntTrack = true;//this one is true and only happens once
    raceStarted = true; // set to true as the race has started and again only happens once
    moveForward();
    delay(700);
    moveGripper(90); //close the gripper
  } else if (isAttempingToGetOntTrack){
    if(sensorValues[0] < BLACK || sensorValues[1] < BLACK || sensorValues[2] < BLACK ){
      isAttempingToGetOntTrack = false;// false when it starts moving right, and only happends once
    } else{
      moveLeftStart(); //when it sees the cargo and isAttemptingToGetOntTrack is true, it moves left
    }
  } else if(raceEnded){ 
    if (sensorValues[0] < BLACK && sensorValues[1] < BLACK && sensorValues[2] < BLACK && sensorValues[3] < BLACK && sensorValues[4] < BLACK && sensorValues[5] < BLACK && sensorValues[6] < BLACK && sensorValues[7] < BLACK ){
      stopRobot(); // When raceEnded is true AND all sensors is black, it stops the robot
    }else{
      moveGripper(130);
      moveBackwards();
    }
  } else if (isTestingR){
    if (sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK && sensorValues[3] > BLACK && sensorValues[4] > BLACK && sensorValues[5] > BLACK && sensorValues[6] > BLACK && sensorValues[7] > BLACK ){
      raceEnded = true; //this happends when all sensor is black and isTestingR is true, it moves forward a bit to track whether its a black square, if its true, raceEnded = true, otherwise isRight = true;
    }else{
      isRight = true;
      isTestingR = false;
    }
  } else if(isRight){
    if(sensorValues[0] > BLACK || sensorValues[1] > BLACK){
      isRight = false;
    }else{
      turnRight(); // 
    }
  } else if(isLeft){
    if(sensorValues[7] > BLACK || sensorValues[6] > BLACK){
      isLeft = false;
    }else{
      turnAround(); //We set turn around as turn left
    }
  }else if (raceStarted){
    if(sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK && sensorValues[3] > BLACK && sensorValues[4] > BLACK || sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK && sensorValues[3] > BLACK || sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK ){
      isTestingR = true; // This logic we would like to set if its on the right turn, 
      moveForward();    //go forward a bit to detect whether its a black square or not.
      delay(100);
  } else if (sensorValues[3] > BLACK || sensorValues[4] > BLACK){
      moveForward();
  } else if (sensorValues[5] > BLACK || sensorValues[6] > BLACK || sensorValues[7] > BLACK){ 
      moveLeft();
  } else if (sensorValues[0] > BLACK || sensorValues[1] > BLACK || sensorValues[2] > BLACK){
      moveRight();
    } else {
      isLeft = true; 
    }
  }
}

void startRace() {
  //Start the race only if it hasn't been started already
  if (!raceStarted) {
    raceStarted = true;
  }
}

void endRace() {
  raceEnded = true;
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
  analogWrite (motorPin1, 212);
  digitalWrite(motorPin2, LOW);
  analogWrite (motorPin3, 250);
  digitalWrite(motorPin4, LOW);
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
  pixels.setPixelColor(1, pixels.Color(0, 255, 0)); //
  pixels.setPixelColor(2, pixels.Color(0, 0, 0)); // 
  pixels.setPixelColor(3, pixels.Color(0, 0, 0)); // 
  pixels.setPixelColor(0, pixels.Color(0, 255, 0)); //
  pixels.show();
}

void turnRight() {
  analogWrite (motorPin1, 120);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  analogWrite (motorPin4, 160);
  pixels.setPixelColor(1, pixels.Color(155, 255, 0)); // 
  pixels.setPixelColor(2, pixels.Color(155, 255, 0)); //
  pixels.setPixelColor(3, pixels.Color(0, 0, 0)); //  
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); //
  pixels.show();
}

void moveLeftStart() {
  analogWrite (motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, 200);
  digitalWrite(motorPin4, LOW);
  pixels.setPixelColor(1, pixels.Color(0, 0, 0)); // 
  pixels.setPixelColor(2, pixels.Color(155, 255, 0)); // 
  pixels.setPixelColor(3, pixels.Color(0, 0, 0)); //
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); //
  pixels.show();
}

void moveRight() {
  analogWrite (motorPin1, 220);
  digitalWrite(motorPin2, LOW);
  analogWrite (motorPin3,  150);
  digitalWrite(motorPin4, LOW);
  pixels.setPixelColor(1, pixels.Color(0, 0, 0)); // 
  pixels.setPixelColor(2, pixels.Color(0, 0, 0)); // 
  pixels.setPixelColor(3, pixels.Color(155, 255, 0)); //
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); //
  pixels.show();
} 

void moveLeft() {
  digitalWrite(motorPin1, 150);
  digitalWrite(motorPin2, LOW);
  analogWrite (motorPin3, 230);
  digitalWrite(motorPin4, LOW);
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
  pixels.setPixelColor(1, pixels.Color(0, 0, 0)); //
  pixels.setPixelColor(2, pixels.Color(0, 0, 0)); // 
  pixels.setPixelColor(3, pixels.Color(155, 255, 0)); //
  pixels.setPixelColor(0, pixels.Color(155, 255, 0)); //
  pixels.show();
}
