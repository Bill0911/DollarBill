//#include <Servo.h>

//Servo gripperServo;  // Create a servo object to control the gripper

const int motorPin1 = 3;   // Motor 1 control pin
const int motorPin2 = 5;   // Motor 1 control pin
const int motorPin3 = 6;   // Motor 2 control pin
const int motorPin4 = 9;   // Motor 2 control pin

const int trigPin = 2; // Connect Trig to digital pin 2
const int echoPin = 7; // Connect Echo to digital pin 7

const int sensorCount = 8; // Number of sensors in your analog line sensor
const int sensorPins[] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Analog sensor pins

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  //gripperServo.attach(4);  // Attach the servo to the specified pin
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600); // Initialize serial communication for debugging
}

void loop() {
  int sensorValues[sensorCount];
  int distance = getDistance();
  //If the distance os greater than 10 cm, move forward
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
  
  // Read sensor values
  for (int i = 0; i < sensorCount; ++i) {
    sensorValues[i] = analogRead(sensorPins[i]);
  }

  // Print sensor values to Serial Monitor
  Serial.print("Sensor Values: ");
  for (int i = 0; i < sensorCount; ++i) {
    Serial.print(sensorValues[i]);
    Serial.print(" ");
  }
  Serial.println();
  int error = calculateError(sensorValues);
if (sensorPins[3] > 700 && sensorPins[4] > 700){
      moveForward();
    } else {
      if (sensorPins[0] > 700 && sensorPins[1] > 700){
        moveLeft();
      } else if (sensorPins[7] > 700 && sensorPins[8] > 700){
        moveRight();
      }
    }
  // Your line following logic goes here
  // Example: Move forward if the middle sensor reads a high value
 
  delay(200); // Adjust delay based on your requirements
}
int calculateError(int sensorValues[]) {
 // Implement logic to calculate the error based on the sensor values
       if(sensorValues[0] < 700 && sensorValues[1] < 700 && sensorValues[2] < 700 && sensorValues[3] < 700 && sensorValues[4] < 700 && sensorValues[5] < 700 && sensorValues[6] < 700 && sensorValues[7] > 700) return 7;
  else if(sensorValues[0] < 700 && sensorValues[1] < 700 && sensorValues[2] < 700 && sensorValues[3] < 700 && sensorValues[4] < 700 && sensorValues[5] < 700 && sensorValues[6] > 700 && sensorValues[7] > 700) return 6;
  else if(sensorValues[0] < 700 && sensorValues[1] < 700 && sensorValues[2] < 700 && sensorValues[3] < 700 && sensorValues[4] < 700 && sensorValues[5] < 700 && sensorValues[6] > 700 && sensorValues[7] < 700) return 5;
  else if(sensorValues[0] < 700 && sensorValues[1] < 700 && sensorValues[2] < 700 && sensorValues[3] < 700 && sensorValues[4] < 700 && sensorValues[5] > 700 && sensorValues[6] > 700 && sensorValues[7] < 700) return 4;
  else if(sensorValues[0] < 700 && sensorValues[1] < 700 && sensorValues[2] < 700 && sensorValues[3] < 700 && sensorValues[4] < 700 && sensorValues[5] > 700 && sensorValues[6] < 700 && sensorValues[7] < 700) return 3;
  else if(sensorValues[0] < 700 && sensorValues[1] < 700 && sensorValues[2] < 700 && sensorValues[3] < 700 && sensorValues[4] > 700 && sensorValues[5] > 700 && sensorValues[6] < 700 && sensorValues[7] < 700) return 2;
  else if(sensorValues[0] < 700 && sensorValues[1] < 700 && sensorValues[2] < 700 && sensorValues[3] < 700 && sensorValues[4] > 700 && sensorValues[5] < 700 && sensorValues[6] < 700 && sensorValues[7] < 700) return 1;
  else if(sensorValues[0] < 700 && sensorValues[1] < 700 && sensorValues[2] < 700 && sensorValues[3] > 700 && sensorValues[4] > 700 && sensorValues[5] < 700 && sensorValues[6] < 700 && sensorValues[7] < 700) return 0;
  else if(sensorValues[0] < 700 && sensorValues[1] < 700 && sensorValues[2] < 700 && sensorValues[3] > 700 && sensorValues[4] < 700 && sensorValues[5] < 700 && sensorValues[6] < 700 && sensorValues[7] < 700) return -1;
  else if(sensorValues[0] < 700 && sensorValues[1] < 700 && sensorValues[2] > 700 && sensorValues[3] > 700 && sensorValues[4] < 700 && sensorValues[5] < 700 && sensorValues[6] < 700 && sensorValues[7] < 700) return -2;
  else if(sensorValues[0] < 700 && sensorValues[1] < 700 && sensorValues[2] > 700 && sensorValues[3] < 700 && sensorValues[4] < 700 && sensorValues[5] < 700 && sensorValues[6] < 700 && sensorValues[7] < 700) return -3;
  else if(sensorValues[0] < 700 && sensorValues[1] > 700 && sensorValues[2] > 700 && sensorValues[3] < 700 && sensorValues[4] < 700 && sensorValues[5] < 700 && sensorValues[6] < 700 && sensorValues[7] < 700) return -4;
  else if(sensorValues[0] < 700 && sensorValues[1] > 700 && sensorValues[2] < 700 && sensorValues[3] < 700 && sensorValues[4] < 700 && sensorValues[5] < 700 && sensorValues[6] < 700 && sensorValues[7] < 700) return -5;
  else if(sensorValues[0] > 700 && sensorValues[1] > 700 && sensorValues[2] < 700 && sensorValues[3] < 700 && sensorValues[4] < 700 && sensorValues[5] < 700 && sensorValues[6] < 700 && sensorValues[7] < 700) return -6;
  else return -7;
   
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
  // Example: Move forward
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  delay(20); //Adjust the delay for the desired speed
}

void stopRobot() {
  // Stop the robot
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

void moveGripper(int angle){
  gripperServo.write(angle); // Adjust the angle to move the gripper
}

const int initialSpeed = 255; //Set the initial motor speed

void reduceSpeed(){
  static int currentSpeed = initialSpeed; // Initialize the current speed

  //Gradually reduce speed (you can adjust the decrement value)
  currentSpeed -= 10;

  //Ensure speed does not go below a minimumValue
  if(currentSpeed < 50){
    currentSpeed = 50;
  }

  //Apply the reduced speed to the motors
  analogWrite(motorPin1, currentSpeed);
  analogWrite(motorPin2, 0); //Assuming motorPin2 is the correspond pin for the first motor
  analogWrite(motorPin3, currentSpeed);
  analogWrite(motorPin4, 0); //Assuming motorPin4 is the correspond pin for the second motor
}
void moveRight() {
  // Implement your right turn logic here
  analogWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(motorPin3, LOW);  // Stop the right motor
  digitalWrite(motorPin4, HIGH);
}

void moveLeft() {
  // Implement your left turn logic here
  analogWrite(motorPin1, LOW);  // Stop the left motor
  digitalWrite(motorPin2, HIGH);
  analogWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}
