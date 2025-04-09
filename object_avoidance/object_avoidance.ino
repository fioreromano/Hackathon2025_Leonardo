#include <ESP32Servo.h>
#include "HT16K33_Lib_For_ESP32.h"

#define SDA 21
#define SCL 22

//motor
#define left_ctrl  33  //define direction control pins of the left motor as gpio33
#define left_pwm  26   //define PWM control pins of the left motor as gpio26.
#define right_ctrl  32 //define direction control pins of the right motor as gpio32.
#define right_pwm  25  //define PWM control pins of the right motor as gpio25

//ultrasonic sensor
#define TRIG_PIN 5 // Define the signal input of the ultrasonic sensor as gpio5.
#define ECHO_PIN 18 // Define the signal output of the ultrasonic sensor as gpio18.
long distance,a1,a2;//define three distance 

//IR line tracking 
#define tracking_left  17  //set the pin of the left line tracking sensor to gpio17
#define tracking_right  16  //set the pin of the right line tracking sensor to gpio16
int L_val = 0,R_val = 0;//Define two variables

#define PHOTOSENSITIVE_PIN 35
#define buzzerPin 2 //buzzer pin
const int resolution = 8;

bool stato = false; // avoid/line detector
bool first = false; // light

int photosensitiveADC;
int count = -1;

//servo
const int servoPin = 4; //set the pin of the servo to gpio4.
Servo myservo;  // create servo object to control a servo

ESP32_HT16K33 matrix = ESP32_HT16K33();

uint8_t data0[] =
{
0x00, 0x3c, 0x3c, 0x24, 0x24, 0x3c, 0x3c, 0x00
};
uint8_t data1[] =
{
0x00, 0x20, 0x43, 0x7f, 0x7f, 0x03, 0x00, 0x00
};
uint8_t data2[] =
{
0x00, 0x00, 0x66, 0x4e, 0x4a, 0x7a, 0x00, 0x00
};
uint8_t data3[] =
{
0x00, 0x00, 0x52, 0x52, 0x52, 0x7e, 0x00, 0x00
};

void avoid();
float checkdistance();
void car_front();
void car_back();
void car_left();
void car_right();
void car_Stop();
void tracking();


void setup() {
  pinMode(left_ctrl,OUTPUT); //set control pins of the left motor to OUTPUT
  ledcAttach(left_pwm, 1200, 8); //Set the frequency of left_pwm pin to 1200, PWM resolution to 8 that duty cycle is 256.
  pinMode(right_ctrl,OUTPUT); //set direction control pins of the right motor to OUTPUT..
  ledcAttach(right_pwm, 1200, 8); //Set the frequency of right_pwm pin to 1200, PWM resolution to 8 that duty cycle is 256.
  
  pinMode(TRIG_PIN,OUTPUT); //set TRIG_PIN to OUTPUT.
  pinMode(ECHO_PIN,INPUT); //set ECHO_PIN to INPUT.

  pinMode(tracking_left, INPUT); //Set right pins of the left sensor to input
  pinMode(tracking_right, INPUT); //Set right pins of the right sensor to input

  pinMode(PHOTOSENSITIVE_PIN, INPUT);//Configure the pins for input mode

  //pinMode(buzzerPin, OUTPUT);
  //ledcAttach(buzzerPin, 2000, 8); // Set up the PWM pin

  matrix.init(0x70, SDA, SCL);//Initialize matrix
  matrix.setBrightness(15);
  
  myservo.setPeriodHertz(50);           // standard 50 hz servo
  myservo.attach(servoPin, 500, 2500);  // attaches the servo on servoPin to the servo object.
  myservo.write(90);
  delay(1000);

  Serial.begin(9600);
}
 
void loop()
 {
  if(!stato){
   /* L_val = digitalRead(tracking_left);//read the value of the left line tracking sensor
    R_val = digitalRead(tracking_right);//read the value of the right line tracking sensor

    Serial.println(L_val);
    Serial.println(R_val);
    */
    if((L_val == 0)&&(R_val == 0))//if both of sensors don't detect black lines
    {
      Serial.println("avoid");
      avoid();
      if(!first){
        photosensitiveADC = analogRead(PHOTOSENSITIVE_PIN);
        if(photosensitiveADC > 3000){
          count++;
          delay(300);
        }
      }
      else{
          if(count == 0 || count == -1){
            matrix.showLedMatrix(data0,0,0);
          }
          else if(count == 1){
            matrix.showLedMatrix(data1,0,0);
          }
          else if(count == 2){
            matrix.showLedMatrix(data2,0,0);
          }
          else{
            matrix.showLedMatrix(data3,0,0);
          }
          matrix.show();  
      }
    }
    else{
      Serial.println("tracking");
      tracking();
    }
  }
  else{
    stato = true;
  }

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

void avoid()
{
  distance = checkdistance(); //Get the value of ultrasonic distance
  //Serial.println(distance);
  if((distance < 14)&&(distance != 0))//if 0<distance<10
  {
    first = true;
    car_Stop();//stop

    delay(100);
    myservo.write(180);  //the servo turns to 180°. 
    //Serial.println("180");
    delay(500);
    a1=checkdistance();//Measuring distance
    delay(100);
    myservo.write(0); //the servo turns to 0°. 
    //Serial.println("0");
    delay(500);
    a2=checkdistance();//Measuring distance
    delay(100);

    myservo.write(90); //the initial angle of the servo is set to 90° .
    //Serial.println("90");
    delay(500);

    ledcAttach(buzzerPin, 2000, 8); // Set up the PWM pin

    if(a1 > a2)//
    {
      ledcWriteTone(buzzerPin, 50); // Start the tone
      delay(300); // Wait for the specified duration
      ledcWriteTone(buzzerPin, 0); // Stop the buzzer
      car_left();//turn left
      delay(150);
    }
    else//if left distance < right distance
    {
      ledcWriteTone(buzzerPin, 50); // Start the tone
      delay(300); // Wait for the specified duration
      ledcWriteTone(buzzerPin, 0); // Stop the buzzer
      delay(100);
      ledcWriteTone(buzzerPin, 50); // Start the tone
      delay(300); // Wait for the specified duration
      ledcWriteTone(buzzerPin, 0); // Stop the buzzer

      car_right();//turn right
      delay(150);
    }

    car_Stop();
    ledcDetach(buzzerPin);

  }
  else//
  {
    car_front();//go forward  
  }
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
  ledcWrite(left_pwm, 120); //the left motor outputs PWM 100
  digitalWrite(right_ctrl,HIGH); //set control pins of the right motor to HIGH..
  ledcWrite(right_pwm, 120); //the right motor outputs PWM 100
}
void car_left()//define the state of turning left
{
  digitalWrite(left_ctrl,HIGH); //set direction control pins of the left motor to HIGH..
  ledcWrite(left_pwm, 120); //the left motor outputs PWM 100
  digitalWrite(right_ctrl,LOW); //set control pins of the right motor to LOW.
  ledcWrite(right_pwm, 120); //the right motor outputs PWM 100
}
void car_right()//define the state of turning right
{
  digitalWrite(left_ctrl,LOW); //set direction control pins of the left motor to LOW.
  ledcWrite(left_pwm, 120); //the left motor outputs PWM 100
  digitalWrite(right_ctrl,HIGH); //set control pins of the right motor to HIGH..
  ledcWrite(right_pwm, 120); //the right motor outputs PWM 100

}
void car_Stop()//define the state of stopping
{
  digitalWrite(left_ctrl,LOW);//set direction control pins of the left motor to LOW.
  ledcWrite(left_pwm, 0); //the left motor outputs PWM 0 
  digitalWrite(right_ctrl,LOW);//set control pins of the right motor to LOW.
  ledcWrite(right_pwm, 0); //the right motor outputs PWM 0
} 

void tracking()
{
  L_val = digitalRead(tracking_left);//read the value of the left line tracking sensor
  R_val = digitalRead(tracking_right);//read the value of the right line tracking sensor
  if((L_val == 1)&&(R_val == 1))//if both of sensors detect black lines
  {
    car_front();//go forward
  }
  else if((L_val == 1)&&(R_val == 0))//if only the left sensor detects black lines
  {
    car_left();//turn left
  }
  else if((L_val == 0)&&(R_val == 1))//if only the right sensor detects black lines
  {
    car_right();//turn right
  }
  else//if none of sensors detects black lines
  {
    car_Stop();//stop
    stato = true;
  }
}
