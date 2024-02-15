const int motorPin1 = 3;   // Motor 1 control pin
const int motorPin2 = 5;   // Motor 1 control pin
const int motorPin3 = 6;   // Motor 2 control pin
const int motorPin4 = 9;   // Motor 2 control pin

const int sensorCount = 8; // Number of sensors in your analog line sensor
const int sensorPins[] = {A0, A1, A2, A3, A4, A5, A6, A7}; // Analog sensor pins

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

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

  // Your line following logic goes here
  // Example: Move forward if the middle sensor reads a high value
  if (sensorValues[sensorCount / 2] > 500) {
    moveForward();
  } else {
    stopRobot();
    // Add more logic for turning or adjusting based on sensor values
  }

  delay(200); // Adjust delay based on your requirements
}

void moveForward() {
  // Your existing motor control logic goes here
  // Example: Move forward
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void stopRobot() {
  // Stop the robot
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}
