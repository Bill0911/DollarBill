#include <Adafruit_NeoPixel.h>

#define PIN 13        // Digital pin Neopixels
#define NUM_PIXELS 4  // Number of Neopixels

Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned long startTime = 0;  // Variable to store the start time of the timer
bool timerStarted = false;    // Flag to indicate if the timer has started
bool raceStarted = false;     // Flag to indicate if the robot should start or not
bool raceEnded = false;
bool isWhite = false;
bool isBlack = false;
bool isTesting = false;
bool destroyed = false;

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
  analogWrite (motorPin1, 170);
  digitalWrite(motorPin2, LOW);
  analogWrite (motorPin3, 200);
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
  analogWrite (motorPin2, 150);
  analogWrite (motorPin3, 113);
  digitalWrite(motorPin4, LOW);
}
void lightAround(){
  pixels.setPixelColor(1, pixels.Color(0, 0, 0)); //
  pixels.setPixelColor(2, pixels.Color(0, 0, 0)); // 
  pixels.setPixelColor(3, pixels.Color(155, 255, 0)); //
  pixels.setPixelColor(0, pixels.Color(155, 255, 0)); //
  pixels.show();
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
    lightStop();
    stopRobot();
  } else if(isBlack) {
    lightBackwards();
    moveBackwards();
    delay(325);
    moveGripper(130);
    lightBackwards();
    moveBackwards();
    delay(1000);
    endRace();
  } else if(isWhite) {
    isWhite = false;
    lightTRight();
    turnRight();
    delay(500);
  }else if(isTesting && sensorValues[0] < 900 && sensorValues[1] < 900 && sensorValues[2] < 900 && sensorValues[3] < 900 && sensorValues[4] < 900 && sensorValues[5] < 900 && sensorValues[6] < 900 && sensorValues[7] < 900){
    isWhite = true;
    isTesting = false;
  } else if (isTesting && sensorValues[0] > 900 && sensorValues[1] > 900 && sensorValues[2] > 900 && sensorValues[3] > 900 && sensorValues[4] > 900 && sensorValues[5] > 900 && sensorValues[6] > 900 && sensorValues[7] > 900){ 
    isBlack = true;
    isTesting = false;
  } else if (timerStarted && millis() - startTime <= 1350) {
    moveGripper(90);
    lightAround();
    turnAround();
    delay(350);
    lightForward();
    moveForward();
    delay(300);
  } else if (raceStarted &&  (distance > 25 || distance < 21)) {
    if (sensorValues[0] > 900 && sensorValues[1] > 900 && sensorValues[2] > 900 && sensorValues[3] > 900 && sensorValues[4] > 900 && sensorValues[5] > 900 && sensorValues[6] > 900 && sensorValues[7] > 900) {
      lightForward();
      moveForward();
      delay(350);
      isTesting= true;
    } else if (sensorValues[0] > 900 && sensorValues[1] > 900 && sensorValues[2] > 900 && sensorValues[3] > 900){
      moveForward();
      lightForward();
      delay(350);
      lightTRight();
      turnRight();
      delay(450);
    } else if (sensorValues[3] > 900 || sensorValues[4] > 900) {
      lightForward();
      moveForward();
    } else if (sensorValues[5] > 900 || sensorValues[6] > 900 || sensorValues[7] > 900) {
      lightLeft();
      moveLeft();
    } else if (sensorValues[0] > 900 || sensorValues[1] > 900 || sensorValues[2] > 900) {
      lightRight();
      moveRight();
    } else if(sensorValues[0] < 900 && sensorValues[1] < 900 && sensorValues[2] < 900 && sensorValues[3] < 900 && sensorValues[4] < 900 && sensorValues[5] < 900 && sensorValues[6] < 900 && sensorValues[7] < 900){
     lightAround();
      turnAround();
    } else {
      lightTRight();
      turnRight();
    }
  } else if (!destroyed && distance > 21 && distance < 26) {
    delay(1000);
    startRace();
    startTimer();
    lightForward();
    moveForward();
    destroyed = true;
    delay(1300);
    Serial.print("Start");
  }
}
