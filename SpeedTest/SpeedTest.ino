const int motorPin1 = 3;
const int motorPin2 = 5;
const int motorPin3 = 6;
const int motorPin4 = 9;

void setup(){
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  Serial.begin(9600);
}

void moveForward(){
  analogWrite (motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite (motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);

  Serial.print("Speed forward: ");
  Serial.println(analogRead(motorPin1));
  Serial.println(analogRead(motorPin2));
}

void moveBackwards(){
  digitalWrite(motorPin1, LOW);
  analogWrite (motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  analogWrite (motorPin4, HIGH);

  Serial.print("Speed backward: ");
  Serial.println(analogRead(motorPin2));
  Serial.println(analogRead(motorPin4));
}

void loop(){
  moveForward();
}
