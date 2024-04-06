#include <Adafruit_NeoPixel.h> // neopixel library

#define PIN 13        // Digital pin Neopixels
#define NUM_PIXELS 4  // Number of Neopixels

Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

bool isRight = false;
bool raceStarted = false;     // Flag to indicate if the robot should start or not
bool isTestingR = false; //
bool isLeft = false;
bool raceEnded = false;
bool isAttempingToGetOntTrack = false;
bool isStart = false;

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

unsigned long beginTime = 0;

#define BLACK 900 // defines the threshold of when we say the colour sensor senses the colour black

void setup() 
{
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  for (int i = 0; i < sensorCount; i++) 
  {
    pinMode(sensorPins[i], INPUT);
  }

  pixels.begin();  // Initialize NeoPixels
  pixels.show();   // Initialize all pixels to 'off'
  pixels.setBrightness(50);  // Set NeoPixel brightness
}

void loop() 
{
  // Read sensor values
  for (int i = 0; i < sensorCount; ++i) 
  {
    sensorValues[i] = analogRead(sensorPins[i]);
  }

  unsigned long startTime = millis();

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

  if(!raceStarted && !isStart && distance > 23 && distance < 27)
   {
    isStart = true;
    beginTime = startTime;
    moveForward();
  }
  else if(!raceStarted && isStart && startTime - beginTime >= 900)
  {
    moveGripper(92);
    isAttempingToGetOntTrack = true;
    raceStarted = true;
  } 
  else if (isAttempingToGetOntTrack)
  {
    getOnTrack();
  } 
  else if(raceEnded)
  { 
    endOfRace();
  } 
  else if (isTestingR)
  {
    testingIfR();
  } 
  else if(isRight)
  {
    if(sensorValues[0] > BLACK || sensorValues[1] > BLACK)
    {
      isRight = false;
    } 
    else
    {
      turnRight();
    }
  } 
  else if(isLeft){
    if(sensorValues[7] > BLACK || sensorValues[6] > BLACK)
    {
      isLeft = false;
    }
    else
    {
      turnAround();
    }
  }
  else if (raceStarted)
  {
    solvemaze();
  }
}



void endOfRace()
{
  if (sensorValues[0] < BLACK && sensorValues[1] < BLACK && sensorValues[2] < BLACK && sensorValues[3] < BLACK && sensorValues[4] < BLACK && sensorValues[5] < BLACK && sensorValues[6] < BLACK && sensorValues[7] < BLACK)
  {
    stopRobot();
  }
  else
  {
    moveGripper(130);
    moveBackwards();
  }
}

void getOnTrack()
{
  if(sensorValues[0] < BLACK || sensorValues[1] < BLACK)
  {  
    isAttempingToGetOntTrack = false;
  } 
  else
  {
    moveLeftStart();
  }
}

void startRace() 
{
  //Start the race only if it hasn't been started already
  if (!raceStarted) 
  {
    raceStarted = true;
  }
}

void testingIfR()
{
  if (sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK && sensorValues[3] > BLACK && sensorValues[4] > BLACK && sensorValues[5] > BLACK && sensorValues[6] > BLACK && sensorValues[7] > BLACK)
  {
    raceEnded = true;
  }
  else
  {
    isRight = true;
    isTestingR = false;
  }
}

void solvemaze()
{
  if(sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[3] > BLACK || sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[4] > BLACK || sensorValues[0] > BLACK && sensorValues[1] > BLACK && sensorValues[2] > BLACK)
  {
      isTestingR = true;
      moveForward();
      delay(100);
  } 
  else if (sensorValues[3] > BLACK || sensorValues[4] > BLACK)
  {
      moveForward();
  } 
  else if (sensorValues[5] > BLACK || sensorValues[6] > BLACK || sensorValues[7] > BLACK)
  { 
      moveLeft();
  } 
  else if (sensorValues[0] > BLACK || sensorValues[1] > BLACK || sensorValues[2] > BLACK)
  {
      moveRight();
  } 
  else if (sensorValues[0] < BLACK && sensorValues[1] < BLACK && sensorValues[2] < BLACK && sensorValues[3] < BLACK && sensorValues[4] < BLACK && sensorValues[5] < BLACK && sensorValues[6] < BLACK && sensorValues[7] < BLACK)
  {
    isLeft = true;
  }
}


void endRace() 
{
  raceEnded = true;
}

//Activated the gripper
void moveGripper(int angle) 
{
  int pwmValue = map(angle, 0, 180, 0, 255);
  analogWrite(servoPin, pwmValue);
}

void stopRobot() 
{
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}
void lightStop()
{
  pixels.setPixelColor(1, pixels.Color(0, 255, 0));   // 
  pixels.setPixelColor(2, pixels.Color(0, 255, 0));  // 
  pixels.setPixelColor(3, pixels.Color(0, 255, 0)); //
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));//
  pixels.show();
}

void moveForward() 
{
  analogWrite (motorPin1, 209);
  digitalWrite(motorPin2, LOW);
  analogWrite (motorPin3, 250);
  digitalWrite(motorPin4, LOW);
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));      // 
  pixels.setPixelColor(2, pixels.Color(155, 255, 0)); // 
  pixels.setPixelColor(3, pixels.Color(155, 255, 0));//
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));   //
  pixels.show();
}

void moveBackwards() 
{ 
  digitalWrite(motorPin1, LOW);
  analogWrite(motorPin2, 170);
  digitalWrite(motorPin3, LOW);
  analogWrite(motorPin4, 180);
  pixels.setPixelColor(1, pixels.Color(0, 255, 0));   //
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));    // 
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));   // 
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));//
  pixels.show();
}

void turnRight() 
{
  analogWrite (motorPin1, 120);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  analogWrite (motorPin4, 160);
  pixels.setPixelColor(1, pixels.Color(0, 0, 0)); // 
  pixels.setPixelColor(2, pixels.Color(155, 255, 0));//
  pixels.setPixelColor(3, pixels.Color(155, 255, 0));   //  
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));  //
  pixels.show();
}

void moveLeftStart() 
{
  analogWrite (motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, 200);
  digitalWrite(motorPin4, LOW);
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));     // 
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));// 
  pixels.setPixelColor(3, pixels.Color(155, 255, 0));   //
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));  //
  pixels.show();
}

void moveRight() {
  analogWrite (motorPin1, 220);
  digitalWrite(motorPin2, LOW);
  analogWrite (motorPin3, 150);
  digitalWrite(motorPin4, LOW);
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));      // 
  pixels.setPixelColor(2, pixels.Color(155, 255, 0));     // 
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));//
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));   //
  pixels.show();
} 

void moveLeft() 
{
  analogWrite (motorPin1, 150);
  digitalWrite(motorPin2, LOW);
  analogWrite (motorPin3, 230);
  digitalWrite(motorPin4, LOW);
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));     // 
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));// 
  pixels.setPixelColor(3, pixels.Color(155, 255, 0));   //
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));  // 
  pixels.show();
}

// TurnAround also is TurnLeft
void turnAround() 
{
  digitalWrite(motorPin1, LOW);
  analogWrite (motorPin2, 170);
  analogWrite (motorPin3, 130);
  digitalWrite(motorPin4, LOW);
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));       //
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));      // 
  pixels.setPixelColor(3, pixels.Color(155, 255, 0)); //
  pixels.setPixelColor(0, pixels.Color(155, 255, 0));//
  pixels.show();
}                                                                  
