//Yvan Gihoza
//010785599
//CSCE 4013 Assignment #1

#include <Servo.h>

#define THRESHOLD 100
#define ButtonPin  0
#define servoPin 7                        //the pin used by the servo motor
Servo servo;
                         
const int analogInPin = A0;               // Analog input pin that the potentiometer is attached to
const int analogOutPin = 5;               // Analog output pin that the LED is attached to
const int red = 1;
const int green = 2;
unsigned char ButtonPressed = 0;
int pos = 0;                             //position of the arm
int sensorValue = 0;
int outputValue = 0;
int count = 0;
unsigned char mode = 0;
int curr;
int lastTickMicros;
int tick = 0;

void setup() {
    
  //sensor
  pinMode(analogInPin, INPUT);
  //servo to pin defined
  servo.attach(servoPin);
  //LEDs
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(ButtonPin, INPUT);
  Serial.begin(9600);
  //starting
  for(int i = 0; i < 5; i++)
  {
   digitalWrite(green, HIGH);
   digitalWrite(red, HIGH);
   delay(500);
   digitalWrite(green, LOW);
   digitalWrite(red, LOW);
  }
}

void loop() {

  lastTickMicros = curr;  
  curr = micros();

  //Serial.println(curr-lastTickMicros);
  // read the analog in value:
  sensorValue = analogRead(analogInPin);

  //detect if the button has been pressed
  ButtonPressed = digitalRead(ButtonPin);
  if(ButtonPressed)
  {
    mode = !mode;
    delay(200);
  }
  Serial.println(ButtonPressed);
  //mode 1
  //standard light gauge mode
  if(mode==0)
  {
    Serial.println("MODE1");
    //if light intesifies go all the way to the rigt
    if(sensorValue > THRESHOLD)
    {
        servo.write(180);
        delay(15);
    }
    //if little/no light go all the way to the left  
    else{
        servo.write(0);
        delay(15);
    }
  }
  
  //mode 2
  //count down timer mode
  else if(mode==1)
  {
    
    Serial.println("MODE2");
    //start to the right with the green LED on
    pos = 180;
    servo.write(pos);
    digitalWrite(green, HIGH);
    if(sensorValue >= THRESHOLD)
    {
       count++;
       if((curr-lastTickMicros)>1000000)
       {
        tick++;
        count = 0;
        pos = 180 -(6*tick);
        servo.write(pos);
       }
       else if(pos == 0)
       {
         digitalWrite(green, LOW);
         digitalWrite(red, HIGH);
       }
    }

  }
  count = 0;
}
