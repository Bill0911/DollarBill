const int servoPin =4;
const int pickupDuration = 4000; //40 seconds in miliseconds
void setup()
{
  pinMode(servoPin, OUTPUT); //setup gripper
}

void loop()
{
  for(int angle = 0; angle <= 90; angle += 5)
  {
    moveGripper(angle); // configure the speed of opening gripper
    delay(100); // Can adjust the delay for desired speed
  }
  delay(2000); //wait for 2 seconds after picking up the cargo
  delay(pickupDuration);
  for(int angle = 90; angle >= 0; angle -= 5 )
  {
    moveGripper(angle); // configure the speed of closing gripper
    delay(100);
  }

  delay(2000); //wait for 2 seconds after dropping the cargo
}

void moveGripper(int angle)
{
  analogWrite(servoPin, map(angle, 0, 180, 0, 255)); //configure the angle of the gripper  
}//the map function is used to convert the angle values(0 to 180) to the correstponding PWM values (0 to 255)
