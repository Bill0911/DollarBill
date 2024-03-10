const int trigPin = 2; // Connect Trig to digital pin 2
const int echoPin = 7; // Connect Echo to digital pin 7

void setup()
{
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);// transmit sensor waves
  pinMode(echoPin, INPUT);// get object values
}

void loop()
{
  int distance = getDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm");
  if (distance > 10)
  {
    moveForward();
  } 
  else
  {
    delay(200);
    stopRobot();
    delay(2000); //Open the gripper gradually  
  }
} 
  
  // Triggering the sensor
  int getDistance()
  {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  //Calculating distance in centimeters
  return pulseIn(echoPin, HIGH) * 0.034 / 2;
  }

 void moveForward()
 {
    Serial.print("moveForward");
 }  

 void stopRobot()
 {
    Serial.print("Stop");
 }
 void grabItem()
 {
    
  }
