#include <Servo.h>

Servo gripperServo;  // Create a servo object to control the gripper

const int motorPin1 = 3;   // Motor 1 control pin
const int motorPin2 = 5;   // Motor 1 control pin
const int motorPin3 = 6;   // Motor 2 control pin
const int motorPin4 = 9;   // Motor 2 control pin

const int trigPin = 2; // Connect Trig to digital pin 2
const int echoPin = 7; // Connect Echo to digital pin 7

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  gripperServo.attach(4);  // Attach the servo to the specified pin
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Read the distance to the object
  int distance = getDistance();

  // If the distance is greater than 10 cm, move forward
  if (distance > 10) {
    moveForward();
  } else {
    delay(200);
    stopRobot();
    delay(2000);    // Close the gripper gradually
    for (int angle = 0; angle <= 90; angle += 5) {
      moveGripper(angle);
      delay(60);  // Adjust the delay for the desired speed
    }

    // Wait for a moment (you can adjust the delay)
    delay(2000);

    // Stop the gripper gradually
 

    // Wait for a moment (you can adjust the delay)
    delay(2000);
  }
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) * 0.034 / 2;
}

void moveForward() {
  // Your existing motor control logic goes here
  // Example: Move forward at a moderate speed
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  delay(30);  // Adjust the delay for the desired speed
}

void stopRobot() {
  // Stop the robot
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

void moveGripper(int angle) {
  gripperServo.write(angle);  // Adjust the angle to move the gripper
}
