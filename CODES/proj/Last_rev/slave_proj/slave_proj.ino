// This code is use for alignment, localizing left or right, and shooting the target (balloon) controlled by Arduino Uno
// This code is written by Rohit Kharat

#include <Wire.h>

#define SLAVE_ADDR 9

// Ultrasonic sensor 1
#define trigPin1 8
#define echoPin1 7

// Ultrasonic sensor 2
#define trigPin2 4
#define echoPin2 2

boolean hasAlign = false;
int aligned = 0;
int goToLeft = 0;
int goToRight = 1;
int stayLeft = 0;
int stayRight = 0;
int reachedLeft = 0;
int reachedRight = 0;
int firstFast = 0;
int alignThreshold = 2;
char a;

long unsigned duration, distance, sensor1Dist, sensor2Dist;

int diff = 0;

// Motor A connections
int in1 = 3;
int in2 = 9;
// Motor B connections
int in3 = 5;
int in4 = 6;
//Hopper connections
int hop1 = 10;
int hop2 = 11;

// Black Line Follower 
int IRLeft = 12;      
int IRRight = 13;   

void setup() {
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveEvent);

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
  pinMode(hop1, OUTPUT);
  pinMode(hop2, OUTPUT);

  // Turn off motors initially
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

char receiveEvent(){
  a = Wire.read();
  //Serial.println(a);
  /*
  if ((stayLeft == 1) && (reachedLeft == 1)){
    //a = Wire.read();
    //Serial.println(a);
    delay(1000);
    if (a == 'L') {
      Serial.println("enemy left");
      //moveLeft();
      TurnLeft();
      }
    else if (a == 'R'){
      Serial.println("enemy right");
      //moveRight();
      TurnRight();
     }
    else if (a == 'B' || a == 'S'){
      Serial.println("shoot enemy");
      //Stop();
      shootEnemy();
    }
    else {
      Serial.println("enemy ran off");
      while(digitalRead(IRLeft) == LOW){
        TurnLeft();
      }
      //Stop();
      stayLeft = 0; 
      goToRight = 1;
    }
  }*/
  
  if ((stayRight == 1) && (reachedRight == 1)){
    //a = Wire.read();
    //Serial.println(a);
    delay(100);
    if (a == 'L') {
      Serial.println("enemy left");
      //moveLeft();
      TurnLeft();
     }
    else if (a == 'R'){
      Serial.println("enemy right");
      //moveRight();
      TurnRight();
     }
    else if (a == 'B' || a == 'S'){
      Serial.println("shoot enemy");
      //Stop();
      stayRight = 0;
      shootEnemy();
    }
    else {
      Serial.println("enemy ran off");
      /*
      while(digitalRead(IRRight) == LOW){
        TurnRight();
      }*/
      stayRight = 1; 
      goToLeft = 1;
    }
  }
  
}

void TurnRight(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW); 
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void TurnLeft(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
/*
void moveRight(){
  //right
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(100);
}
void moveLeft(){
  //left
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(100);
}*/
void Stop(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void shootEnemy(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  //spinHopper();
  //delay(200);
 digitalWrite(hop1, LOW);
  digitalWrite(hop2, HIGH);
  delay(300); //4600
  digitalWrite(hop1, LOW);
  digitalWrite(hop2, LOW);
 
  stayRight = 1;
}
/*
void spinHopper(){
  digitalWrite(hop1, LOW);
  digitalWrite(hop2, HIGH);
  delay(300); //4600
  digitalWrite(hop1, LOW);
  digitalWrite(hop2, LOW);
}*/

void SonarSensor(int trigPinSensor, int echoPinSensor){
  digitalWrite(trigPinSensor, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinSensor, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinSensor, LOW);
  duration = pulseIn(echoPinSensor, HIGH);
  //distance = (duration/2) / 29.1;
  distance = (duration/2)*0.0343;
}

void fastLeft(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(1000);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  firstFast = 1;
}

void fastRight(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(1000);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  firstFast = 1;
}

/*
void LocalizeLeft(){
  if (goToLeft == 1){
      Serial.println("localize left");
      // initally move robot to left

      if (firstFast == 0){
        fastLeft();
      }
      
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      delay(200);      
      
      //IR will stop on white line (LOW == WHITE)
      if(digitalRead(IRLeft)==LOW){ 
        Serial.println("Reached left");
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
        delay(1000);
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
        delay(1000);
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);

        reachedLeft = 1; 
        goToLeft = 0;
        stayLeft = 1;
        firstFast = 0;
      }
    }
}*/

void LocalizeRight(){
  if (goToRight == 1){
      Serial.println("localize right");
      // initally move robot to left

      if (firstFast == 0){
        fastRight();
      }
      
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      delay(300);
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
      
      //IR will stop on white line (LOW == WHITE)
      if(digitalRead(IRRight)==LOW){ 
        Serial.println("Reached right");
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
        delay(1000);
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
        delay(500);
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);

        reachedRight = 1; 
        goToRight = 0;
        stayRight = 1;
        firstFast = 0;
      }
    }
}


void align(){
  if (hasAlign == false){
    SonarSensor(trigPin1, echoPin1);
    sensor1Dist = distance;
    Serial.println("distance measured by the first sensor: ");
    Serial.println(sensor1Dist);
    SonarSensor(trigPin2, echoPin2);
    sensor2Dist = distance;
    Serial.println("distance measured by the second sensor: ");
    Serial.println(sensor2Dist);
    diff = sensor1Dist - sensor2Dist;  


   if(diff > alignThreshold){
        // rotate robot in CW
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW); 
    delay(300);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    delay(100); 
    
    }

    else if(diff < -alignThreshold){
            // rotate robot in CCW
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH); 
    delay(300);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    delay(100); 
    }

    else{
      // Turn off motors initially
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
      hasAlign = true;
      Serial.println("Robot aligned!");
      aligned = 1;
      //return hasAlign;
    }
  }
}

void loop() {
  
  if (hasAlign == false){
    delay(2000);
    align();
  }
  
  if (aligned == 1){
    /*
    if (goToLeft == 1){
      delay(1000);
      LocalizeLeft();
    }*/
    
    if(goToRight == 1){
      delay(50);
      LocalizeRight();
    }
  }
}


    

  
