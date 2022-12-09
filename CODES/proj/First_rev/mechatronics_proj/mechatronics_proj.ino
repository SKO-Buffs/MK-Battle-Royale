#include <Pixy2.h>
#include <SPI.h>
#include <Servo.h>

Pixy2 pixy;

// Camera Parameters
int signature = 0;
int x = 0;
int y = 0;
unsigned int width = 0;
unsigned int height = 0;
unsigned int area = 0;
unsigned int newArea = 0;
int Xmin = 70; //tunable
int Xmax = 200; //tunable
int maxArea = 0;
int minArea = 0;
static int i = 0;

// Ultrasonic sensor 1
#define trigPin1 7
#define echoPin1 8

// Ultrasonic sensor 2
#define trigPin2 10
#define echoPin2 11

boolean hasAlign = false;
int aligned = 0;

long unsigned duration, distance, sensor1Dist, sensor2Dist;

// Motor A connections
int in1 = 3;
int in2 = 9;

// Motor B connections
int in3 = 5;
int in4 = 6;

// Black Line Follower 
int IRLeft=2;      //Right sensor
int IRRight=4;    //left

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // pins for IR
  pinMode(IRLeft,INPUT);
  pinMode(IRRight,INPUT);
  
  // pins for ultrasonic sensors
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  // pins for motors
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Turn off motors initially
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  // pixy init
  pixy.init();
}

// Pixy Scanning Code ----------------------------------------------------

void scan(){
  uint16_t blocks;
  blocks = pixy.ccc.getBlocks();
  signature = pixy.ccc.blocks[i].m_signature;
  x = pixy.ccc.blocks[i].m_x;
  y = pixy.ccc.blocks[i].m_y;
  width = pixy.ccc.blocks[i].m_width;
  height = pixy.ccc.blocks[i].m_height;
}

void startScan(){
  
  while (millis() < 5000){
    scan();
    area = width*height;
    Serial.print("area");
    maxArea = area + 1000;
    minArea = area - 1000;
  }
  scan();

  areaTilt();
}

void moveRight(){
  //right
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(2000);
}
void moveLeft(){
  //left
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(2000);
}

// Using Area to Tilt Code

void areaTilt(){
  if (signature == 1){
    newArea = width*height;

    if (x < Xmin){
      Serial.print("Left");
      Serial.print("\n");
      moveLeft();
    }
    else if (x > Xmax){
      Serial.print("Right");
      Serial.print("\n");
      moveRight();
    }
    else{
      //Stop();
      Serial.print("Stop");
      Serial.print("\n");
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
      // shoot
    }
  }
  else{
    //stop
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    }
}

void SonarSensor(int trigPinSensor, int echoPinSensor){
    
  digitalWrite(trigPinSensor, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinSensor, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinSensor, LOW);
  
  duration = pulseIn(echoPinSensor, HIGH);
  //Serial.println(duration);
  //distance = (duration/2) / 29.1;
  distance= (duration/2)*0.0343;
    
}

int align(){
  if (hasAlign == false){

    SonarSensor(trigPin1, echoPin1);
    sensor1Dist = distance;
    Serial.println("distance measured by the first sensor: ");
    Serial.println(sensor1Dist);
    //Serial.println(" cm");
    
    SonarSensor(trigPin2, echoPin2);
    sensor2Dist = distance;
    Serial.println("distance measured by the second sensor: ");
    Serial.println(sensor2Dist);
    //Serial.println(" cm");
  
    //sensor1Dist != sensor2Dist
  
    long diff = sensor1Dist - sensor2Dist;
    //Serial.println(diff);
  
    // rotate robot in CW
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

  
    if(abs(diff) < 3){
  
      // Turn off motors initially
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
  
      hasAlign = true;
      Serial.println("Robot aligned!");
      aligned = 1;
      return aligned;
    }
  }
}

// Localization Code--------------------------------------

void localizeLeft(){
  delay(1000);

  // initally move robot to left
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  
  if(digitalRead(IRLeft)==HIGH) //IR will not glow on black line
  {
    //Stop both Motors
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    
    delay(1000);
    //tilt and shoot
    //startScan();
  }
}

void localizeRight(){

    delay(1000);
  
    // move robot to right
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    if(digitalRead(IRRight)==HIGH)  //IR not on black line
    {
       //Stop both Motors
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
      
      delay(1000);
      //tilt and shoot
      //startScan();
    }
}


void loop() {
  // put your main code here, to run repeatedly:
  
  if (aligned == 0){
    align();
  }
  //Serial.println(aligned);
/*
  if (aligned == 1){
    Serial.println("localize left");
    localizeLeft();

    Serial.println("localize right");
    localizeRight();
  
  }*/
}


    

  
