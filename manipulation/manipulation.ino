#include <ESP32Servo.h>

//motor
#define left_ctrl  33  //define direction control pins of the left motor as gpio33
#define left_pwm  26   //define PWM control pins of the left motor as gpio26.
#define right_ctrl  32 //define direction control pins of the right motor as gpio32.
#define right_pwm  25  //define PWM control pins of the right motor as gpio25

//ultrasonic sensor
#define TRIG_PIN 5 // Define the signal input of the ultrasonic sensor as gpio5.
#define ECHO_PIN 18 // Define the signal output of the ultrasonic sensor as gpio18.
long distance;//define three distance 

Servo myservo;  // create servo object to control a servo
int servoPin = 23; // Servo motor pin

void setup() {
  pinMode(left_ctrl,OUTPUT); //set control pins of the left motor to OUTPUT
  ledcAttach(left_pwm, 1200, 8); //Set the frequency of left_pwm pin to 1200, PWM resolution to 8 that duty cycle is 256.
  pinMode(right_ctrl,OUTPUT); //set direction control pins of the right motor to OUTPUT..
  ledcAttach(right_pwm, 1200, 8); //Set the frequency of right_pwm pin to 1200, PWM resolution to 8 that duty cycle is 256.

  pinMode(TRIG_PIN,OUTPUT); //set TRIG_PIN to OUTPUT.
  pinMode(ECHO_PIN,INPUT); //set ECHO_PIN to INPUT.

  myservo.setPeriodHertz(50);           // standard 50 hz servo
  myservo.attach(servoPin, 500, 2500);  // attaches the servo on servoPin to the servo object
  myservo.write(7);
  delay(300);
}
void loop() {

  myservo.write(7);
  delay(1000);

  distance = checkdistance();
  while(distance > 2){
    car_front();
    distance = checkdistance();
  }

  car_Stop();
  delay(1000);

  myservo.write(50);
  delay(1000);

  car_front();
  delay(2000);
  car_Stop();
  delay(500);

  car_left();
  delay(700);
  car_Stop();
  delay(500);
  car_left();
  delay(700);
  car_Stop();
  delay(500);

  myservo.write(7);
  delay(1000);

  car_back();
  delay(1000);
  car_Stop();

  //secondo
  delay(15000);

  myservo.write(7);
  delay(1000);

  
  distance = checkdistance(); //Get the value of ultrasonic distance
  while(distance > 8){
    car_front();
    distance = checkdistance();
  }
  car_Stop();
  delay(1000);

  myservo.write(140);
  delay(1000);

  distance = checkdistance();
  while(distance > 2){
    car_front();
    distance = checkdistance();
  }

  car_Stop();
  delay(1000);

  myservo.write(92);
  delay(1000);

  car_back();
  delay(1000);
  car_Stop();

  //terzo
  delay(15000);

  myservo.write(7);
  delay(1000);

  /*distance = checkdistance();
  while(distance > 2){
    car_front();
    distance = checkdistance();
  }

  car_Stop();
  delay(1000);

  myservo.write(100);
  delay(1000);*/

  /*distance = checkdistance();
  while(distance > 12){
    car_front();
    distance = checkdistance();
  }*/

  car_front();
  delay(2000);

  car_Stop();
  delay(1000);

  myservo.write(30);
  delay(1000);

  car_right();
  delay(250);
  car_Stop();
  delay(500);
  car_right();
  delay(250);
  car_Stop();
  delay(500);

  /*distance = checkdistance();
  while(distance > 12){
    car_front();
    distance = checkdistance();
  }*/

  car_front();
  delay(1500);

  car_Stop();
  delay(1000);

  car_left();
  delay(250);
  car_Stop();
  delay(500);
  car_left();
  delay(250);
  car_Stop();
  delay(500);

  /*distance = checkdistance();
  while(distance > 12){
    car_front();
    distance = checkdistance();
  }*/

  car_front();
  delay(2000);

  car_Stop();
  delay(1000);

  myservo.write(7);
  delay(1000);

  car_back();
  delay(1000);
  car_Stop();

  while(true);

}

float checkdistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  float distance = pulseIn(ECHO_PIN, HIGH) / 58.00;
  delay(10);
  return distance;
}

void car_front()//define the state of going forward
{
  digitalWrite(right_ctrl,LOW); //set control pins of the right motor to LOW.
  ledcWrite(right_pwm, 130); //the right motor outputs PWM 100
  digitalWrite(left_ctrl,LOW); //set direction control pins of the left motor to LOW.
  ledcWrite(left_pwm, 125); //the left motor outputs PWM 100
}
void car_back()//define the state of going back
{
  digitalWrite(left_ctrl,HIGH); //set direction control pins of the left motor to HIGH..
  ledcWrite(left_pwm, 125); //the left motor outputs PWM 100
  digitalWrite(right_ctrl,HIGH); //set control pins of the right motor to HIGH..
  ledcWrite(right_pwm, 130); //the right motor outputs PWM 100
}
void car_left()//define the state of turning left
{
  digitalWrite(left_ctrl,HIGH); //set direction control pins of the left motor to HIGH..
  ledcWrite(left_pwm, 90); //the left motor outputs PWM 100
  digitalWrite(right_ctrl,LOW); //set control pins of the right motor to LOW.
  ledcWrite(right_pwm, 90); //the right motor outputs PWM 100
}
void car_right()//define the state of turning right
{
  digitalWrite(left_ctrl,LOW); //set direction control pins of the left motor to LOW.
  ledcWrite(left_pwm, 90); //the left motor outputs PWM 100
  digitalWrite(right_ctrl,HIGH); //set control pins of the right motor to HIGH..
  ledcWrite(right_pwm, 90); //the right motor outputs PWM 100

}
void car_Stop()//define the state of stopping
{
  digitalWrite(left_ctrl,LOW);//set direction control pins of the left motor to LOW.
  ledcWrite(left_pwm, 0); //the left motor outputs PWM 0 
  digitalWrite(right_ctrl,LOW);//set control pins of the right motor to LOW.
  ledcWrite(right_pwm, 0); //the right motor outputs PWM 0
}

void car_left1()//define the state of turning left
{
  digitalWrite(left_ctrl,HIGH); //set direction control pins of the left motor to HIGH..
  ledcWrite(left_pwm, 90); //the left motor outputs PWM 100
  digitalWrite(right_ctrl,LOW); //set control pins of the right motor to LOW.
  ledcWrite(right_pwm, 90); //the right motor outputs PWM 100
}
void car_right1()//define the state of turning right
{
  digitalWrite(left_ctrl,LOW); //set direction control pins of the left motor to LOW.
  ledcWrite(left_pwm, 90); //the left motor outputs PWM 100
  digitalWrite(right_ctrl,HIGH); //set control pins of the right motor to HIGH..
  ledcWrite(right_pwm, 90); //the right motor outputs PWM 100

}