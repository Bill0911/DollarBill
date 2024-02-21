const int motorPin1 = 9;   // Motor 1 control pin
const int motorPin2 = 6;   // Motor 1 control pin
const int motorPin3 = 5;   // Motor 2 control pin
const int motorPin4 = 3;   // Motor 2 control pin

const int trigPin = 2; // Connect Trig to digital pin 2
const int echoPin = 7; // Connect Echo to digital pin 7

const int sensorCount = 8; // Number of sensors in your analog line sensor
const int sensorPins[sensorCount] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Analog sensor pins

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

void loop() {
  int sensorValues[sensorCount];
  
  
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
if(sensorValues[3] > 700 && sensorValues[4] > 700 || sensorValues[2] > 700 && sensorValues[3] > 700 || sensorValues[4] > 700 && sensorValues[5] > 700){
      moveForward();
       Serial.print("Sensor moveForward: ");
  }else if(sensorValues[6] > 700 && sensorValues[7] > 700 || sensorValues[5] > 700 && sensorValues[6] > 700){
      moveLeft();
       Serial.print("Sensor moveLEft: ");
  }else if(sensorValues[0] > 700 && sensorValues[1] > 700 || sensorValues[1] > 700 && sensorValues[2] > 700){
      moveRight();
       Serial.print("Sensor moveRight: ");
  }else{
      stopRobot();
       Serial.print("Sensor stop: ");
    }
    
  // Your line following logic goes here
  // Example: Move forward if the middle sensor reads a high value
 
  
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

void stopRobot() {
  // Stop the robot
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}


int initialSpeed = 50; //Set the initial motor speed


void moveForward() {
  // Your existing motor control logic goes here
  // Example: Move forward
  digitalWrite(motorPin1, LOW);
  analogWrite(motorPin2, 200);
  digitalWrite(motorPin3, LOW);
  analogWrite(motorPin4, 200);
  delay(100);
  
}

void moveRight() {
  // Implement your right turn logic here
  analogWrite(motorPin1, 200);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);  // Stop the right motor
  analogWrite(motorPin4, 200);
  delay(100);
}

void moveLeft() {
  // Implement your left turn logic here
  digitalWrite(motorPin1, LOW);  // Stop the left motor
  analogWrite(motorPin2, 150);
  analogWrite(motorPin3, 130);
  digitalWrite(motorPin4, LOW);
  delay(100);
}
