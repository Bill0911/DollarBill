const int motorPin1 = 3;
const int motorPin2 = 5;
const int motorPin3 = 6;
const int motorPin4 = 9;

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  Serial.begin(9600);
}

void moveForward() {
  for (int speed = 0; speed <= 255; speed += 10) {
    analogWrite(motorPin1, speed);
    analogWrite(motorPin3, speed);

    Serial.print("Speed forward - Motor 1: ");
    Serial.println(speed);

    Serial.print("Speed forward - Motor 2: ");
    Serial.println(speed);

    delay(500);  // Adjust the delay as needed for observation
  }

  // Stop the motors after the test
  analogWrite(motorPin1, 0);
  analogWrite(motorPin3, 0);
}

void moveBackwards() {
  for (int speed = 0; speed <= 255; speed += 10) {
    analogWrite(motorPin2, speed);
    analogWrite(motorPin4, speed);

    Serial.print("Speed backward - Motor 1: ");
    Serial.println(speed);

    Serial.print("Speed backward - Motor 2: ");
    Serial.println(speed);

    delay(500);  // Adjust the delay as needed for observation
  }

  // Stop the motors after the test
  analogWrite(motorPin2, 0);
  analogWrite(motorPin4, 0);
}

void loop() {
  moveForward();
  delay(1000); // Adjust the delay as needed

  moveBackwards();
  delay(1000); // Adjust the delay as needed
}
