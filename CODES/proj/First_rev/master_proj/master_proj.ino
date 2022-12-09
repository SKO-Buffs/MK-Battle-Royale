// This code is written by Rohit Kharat

#include <Pixy2.h>
#include <Wire.h>

#define SLAVE_ADDR 9

Pixy2 pixy;

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

void setup() {
  Wire.begin();
  Serial.begin(9600);
  pixy.init();
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

  scan();

  if (signature == 1){
    newArea = width*height;
    //Serial.println(newArea);

    if (x < Xmin){
      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write("L");
      Wire.endTransmission();
    }
    else if (x > Xmax){
      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write("R");
      Wire.endTransmission();
    }
    else if (newArea < minArea){
      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write("B");
      Wire.endTransmission();
    }
    else{
      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write("S");
      Wire.endTransmission();
    }
  }
  else{
      Wire.beginTransmission(SLAVE_ADDR);
      Wire.write("E");
      Wire.endTransmission();
    }
  }
