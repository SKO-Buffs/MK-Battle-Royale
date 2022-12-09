// This code is written by Rohit Kharat and Luke Prins

#include <Pixy2.h>
#include <SPI.h>
#include <Servo.h>

//Pixy2 pixy;

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
#define trigPinL 8
#define echoPinL 7

// Ultrasonic sensor 2
#define trigPinR 13
#define echoPinR 12

boolean hasAlign = false;
boolean atFront = false;
int aligned = 0;
int alignThreshold = 2;
int localized = 1;

long durationR, durationL, diff;
int distanceR, distanceL;

// Motor A connections
int in1 = 3;
int in2 = 9;

// Motor B connections
int in3 = 5;
int in4 = 6;

// Black Line Follower 
int IRLeft=2;      
int IRRight=4;    

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // pins for IR
  pinMode(IRLeft,INPUT);
  pinMode(IRRight,INPUT);
  
  // pins for ultrasonic sensors
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinL, INPUT);
  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR, INPUT);

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

  //pixy init
  //pixy.init();
}

// Pixy Scanning Code ----------------------------------------------------

/*
void scan(){
  uint16_t blocks;
  blocks = pixy.ccc.getBlocks();
  signature = pixy.ccc.blocks[i].m_signature;
  x = pixy.ccc.blocks[i].m_x;
  y = pixy.ccc.blocks[i].m_y;
  width = pixy.ccc.blocks[i].m_width;
  height = pixy.ccc.blocks[i].m_height;
}
*/
void startScan(){
  
  while (millis() < 5000){
//    scan();
    area = width*height;
    Serial.print("area");
    maxArea = area + 1000;
    minArea = area - 1000;
  }
 // scan();

  areaTilt();
}

void moveRight(int d){
  //right
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(d);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
void moveLeft(int d){
  //left
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(d);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void spinLeft(int d){
  //left
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(d);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
void spinRight(int d){
  //right
   digitalWrite(in1, LOW);
   digitalWrite(in2, HIGH);
   digitalWrite(in3, LOW);
   digitalWrite(in4, HIGH);
   delay(d);
   digitalWrite(in1, LOW);
   digitalWrite(in2, LOW);
   digitalWrite(in3, LOW);
   digitalWrite(in4, LOW);
}
void stopMotors(){
   digitalWrite(in1, LOW);
   digitalWrite(in2, LOW);
   digitalWrite(in3, LOW);
   digitalWrite(in4, LOW);
}


// Using Area to Tilt Code

void areaTilt(){
  if (signature == 1){
    newArea = width*height;

    if (x < Xmin){
      Serial.print("Left");
      Serial.print("\n");
      moveLeft(100);
    }
    else if (x > Xmax){
      Serial.print("Right");
      Serial.print("\n");
      moveRight(100);
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

void SonarSensor(){
  digitalWrite(trigPinR, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinR, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinR, LOW);
  
  durationR = pulseIn(echoPinR, HIGH);
  //distance = (duration/2) / 29.1;
  distanceR= (durationR)*0.0343 / 2;
  
  
  digitalWrite(trigPinL, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinL, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinL, LOW);
  
  durationL = pulseIn(echoPinL, HIGH);
  //distance = (duration/2) / 29.1;
  distanceL= (durationL)*0.0343 / 2;
    
    
}


void roughAlign(){
  SonarSensor();
    while(distanceL > 30){
    Serial.print("rough aligning: ");
    Serial.println(distanceR);
    //rotate cw
    spinRight(100);
    delay(200);
    SonarSensor();
  }
}


void align(){
  if (hasAlign == false){
    SonarSensor();

    if(distanceR > 30)
      roughAlign();
      
    Serial.println("distance measured by the L sensor: ");
    Serial.print(distanceL);
    Serial.println(" cm");
    
    Serial.println("distance measured by the R sensor: ");
    Serial.print(distanceR);
    Serial.println(" cm");
  
    //sensor1Dist != sensor2Dist
    diff = distanceL - distanceR;
    Serial.println(diff);
    delay(200);

    if(diff > alignThreshold){
    // rotate robot in CW
    spinRight(50);
    }

    else if(diff < -alignThreshold){
    // rotate robot in CCW
    spinLeft(50);
    }
    
    else {
      // Turn off motors initially
      stopMotors();
  
      hasAlign = true;
      Serial.println("Robot aligned!");
      aligned = 1;
    }
  }
}

// Localization Code--------------------------------------



void localizeLeft(){
  Serial.print("localize left: ");
  Serial.println(digitalRead(IRLeft));
  // initally move robot to left
  moveLeft(100);
  
  if(digitalRead(IRLeft)==LOW) //IR will not glow on white line
  {
    //Stop both Motors
    stopMotors();
    Serial.println("reached left side");
   localized = -1; //means it has localized left 
    //tilt and shoot
    //startScan();
  }

  //delay(100);
  
}

void localizeRight(){
    Serial.print("localize right: ");
    Serial.println(digitalRead(IRRight));
   
  
    // move robot to right
   moveRight(100);

    if(digitalRead(IRRight)==LOW)  //IR not on white line
    {
       //Stop both Motors
       stopMotors();
      
      //tilt and shoot
      //startScan();
        Serial.println("reached right side");
      localized = 1; //1 means it has localized right 
    } 

 
    //delay(100);
    
}


void navigateFront(){
   // initial allign:
  while(aligned == 0){
    align();
  }

  //find left side
  Serial.println("loclalizing left ::");
  while(localized == 1){
    localizeLeft();
  }
  //turn back wheel to face slant
  Serial.println("Turning to face slant");
  SonarSensor();
  while(distanceR < 150)
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    delay(100);

    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    delay(100);
    SonarSensor();
  }

  //find front white line
  while(localized == -1){
    localizeRight();
  }


  //align to front
  spinRight(300);
  hasAlign = false;
  aligned = 0; 
  while(aligned == 0){
    align();
  }

  //strafe left and right (left first)
  atFront = true;
  localized = 1;
  
  


  
}

void loop(){
  if(!atFront)
    navigateFront();

  if(localized == 1)
    localizeLeft();
  if(localized == -1)
    localizeRight();
    
 
  
 
  

  
 
}

    

  
