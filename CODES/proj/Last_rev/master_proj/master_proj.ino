// This is a master code on Arduino Mega to communicate about the target (balloon) to a shooting & driving mechanism controlled by Arduino Uno
// This code is written by Rohit Kharat

#include <Pixy2.h>
#include <Wire.h>
//#include <SPI.h>

#define SLAVE_ADDR 9

Pixy2 pixy;

int signature = 0;
int x = 0;
int y = 0;
unsigned int width = 0;
unsigned int height = 0;
unsigned int area = 0;
unsigned int newArea = 0;
int Xmin = 70; //tunable 70
int Xmax = 200; //tunable
int maxArea = 0;
int minArea = 0;
static int i = 0;

void setup() {
  pixy.init();
  Wire.begin();
  Serial.begin(9600);
}

void scan(){
  uint16_t blocks;
  blocks = pixy.ccc.getBlocks();
  signature = pixy.ccc.blocks[i].m_signature;
  x = pixy.ccc.blocks[i].m_x;
  y = pixy.ccc.blocks[i].m_y;
  width = pixy.ccc.blocks[i].m_width;
  height = pixy.ccc.blocks[i].m_height;
}

void loop() {
  delay(50);

  while (millis() < 5000){
    scan();
    area = width*height;
    //Serial.println(area);
    maxArea = area + 1000;
    minArea = area - 1000;
  }

  //Serial.println(signature);
  
  scan();
  

  if (signature == 1){
    newArea = width*height;
    //Serial.println(newArea);

    if (x < Xmin){
      //Serial.println('L');
      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write("L");
      Wire.endTransmission();
    }
    else if (x > Xmax){
      //Serial.println('R');
      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write("R");
      Wire.endTransmission();
    }
    else if (newArea < minArea){
      //Serial.println('B');
      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write("B");
      Wire.endTransmission();
      delay(4600);
    }
    else{
      //Serial.println('S');
      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write("S");
      Wire.endTransmission();
      delay(4600);
    }
  }
  else{
      //Serial.println('E');
      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write("E");
      Wire.endTransmission();
    }
  }
