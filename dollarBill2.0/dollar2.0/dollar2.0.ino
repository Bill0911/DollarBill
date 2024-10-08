unsigned long startTime = 0;  // Variable to store the start time of the timer
bool timerStarted = false;    // Flag to indicate if the timer has started
bool raceStarted = false;     // Flag to indicate if the robot should start or not
bool raceEnded = false;
bool isWhite = false;
bool isBlack = false;
bool isTesting = false;

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

  Serial.begin(9600); // Initialize serial communication for debugging
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

void moveForward() {
  analogWrite (motorPin1, 150);
  digitalWrite(motorPin2, LOW);
  analogWrite (motorPin3, 190);
  digitalWrite(motorPin4, LOW);
}

void moveBackwards() { // Corrected the function name
  analogWrite(motorPin1, LOW);
  analogWrite(motorPin2, 180);
  analogWrite(motorPin3, LOW);
  analogWrite(motorPin4, 180);
}

void turnRight() {
  analogWrite (motorPin1, 150);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(800);
}

void moveRight() {
  analogWrite (motorPin1, 113);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

void moveLeft() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  analogWrite (motorPin3, 150);
  digitalWrite(motorPin4, LOW);
}

// TurnAround also is TurnLeft
void turnAround() {
  digitalWrite(motorPin1, LOW);
  analogWrite (motorPin2, 180);
  analogWrite (motorPin3, 130);
  digitalWrite(motorPin4, LOW);
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
    stopRobot();
  } else if(isBlack) {
    moveGripper(130);
    moveBackwards();
    delay(1000);
    endRace();
  } else if(isWhite) {
    moveBackwards();
    delay(400);
    isWhite = false;
    turnRight();
    delay(350);

  }else if(isTesting && sensorValues[0] < 700 && sensorValues[1] < 700 && sensorValues[2] < 700 && sensorValues[3] < 700 && sensorValues[4] < 700 && sensorValues[5] < 700 && sensorValues[6] < 700 && sensorValues[7] < 700){
    isWhite = true;
    isTesting = false;
  } else if (isTesting && sensorValues[0] > 700 && sensorValues[1] > 700 && sensorValues[2] > 700 && sensorValues[3] > 700 && sensorValues[4] > 700 && sensorValues[5] > 700 && sensorValues[6] > 700 && sensorValues[7] > 700){ 
    isBlack = true;
    isTesting = false;
  } else if (timerStarted && millis() - startTime <= 1350) {
    moveGripper(90);
    turnAround();
    delay(250);
  } else if (raceStarted &&  (distance > 25 || distance < 21)) {
    if (sensorValues[0] > 700 && sensorValues[1] > 700 && sensorValues[2] > 700 && sensorValues[3] > 700 && sensorValues[4] > 700 && sensorValues[5] > 700 && sensorValues[6] > 700 && sensorValues[7] > 700) {
      moveForward();
      delay(100);
      isTesting= true;
      Serial.println("isTesting");
    } else if (sensorValues[0] > 700 && sensorValues[1] > 700 && sensorValues[2] > 700 && sensorValues[3] > 700){
      turnRight();
      Serial.print("turnRight");
    } else if (sensorValues[3] > 700 || sensorValues[4] > 700) {
      moveForward();
      Serial.println("moveForward");
    } else if (sensorValues[5] > 700 || sensorValues[6] > 700 || sensorValues[7] > 700) {
      moveLeft();
      Serial.println("moveLeft");
    } else if (sensorValues[0] > 700 || sensorValues[1] > 700 || sensorValues[2] > 700) {
      moveRight();
      Serial.println("moveRight");
    } else {
      turnAround();
      Serial.println("turnAround");
    }
  } else if (distance > 21 && distance < 26) {
    delay(5000);
    startRace();
    startTimer();
    moveForward();
    delay(1300);
    Serial.print("Start");
  }
}
