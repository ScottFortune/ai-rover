#include <Pixy2.h>
#include <SPI.h>
#include <SparkFun_TB6612.h>

//defines for motor control
#define AIN1 2
#define BIN1 7
#define AIN2 4
#define BIN2 8
#define PWMA 5
#define PWMB 6
#define STBY 9

//used to line up motor configuration
const int offsetA = -1;
const int offsetB = -1;

//initialize motors
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

//initialize pixy cam object
Pixy2 pixy;

void setup() {
  //initialize pixy cam and turn on light
   Serial.begin(9600);
   pixy.init();
   pixy.setLamp(1,0);
}

void loop() {
  while(0){
  Serial.print("Total blocks= ");
  Serial.println(pixy.ccc.getBlocks());
  delay(100);
  Serial.print("Detected signature 4?=");
  Serial.println(pixy.ccc.getBlocks(false, 4));
  delay(100);
  for(int i = 0; i < pixy.ccc.getBlocks(); i++){
  Serial.print("Block has signature = ");
  Serial.println(pixy.ccc.blocks[i].m_signature);
  }
  delay(1000);
  }
  //iterate through blocks array
  for(int i = 0; i < pixy.ccc.getBlocks(false); i++){
    //green card
    if(pixy.ccc.blocks[i].m_signature == 4){
      //card is left
      if(pixy.ccc.blocks[i].m_x < 128 && pixy.ccc.blocks[i].m_x > 0){
        brake(motor1, motor2);
        Serial.println("object left");
        motor1.drive(-180, 500);
        brake(motor1, motor2);
        delay(1000);
      }
      //card is right
      if(pixy.ccc.blocks[i].m_x < 316 && pixy.ccc.blocks[i].m_x > 198){
        brake(motor1, motor2);
        Serial.println("object right");
        motor2.drive(-180, 400);
        brake(motor1, motor2);
        delay(1000);
      }
    }
    //red card
    else if(pixy.ccc.blocks[i].m_signature == 1){
      //apply brake while red
      Serial.println("brake");
      while(pixy.ccc.blocks[i].m_signature == 1){
        brake(motor1, motor2);
        delay(1000);
        pixy.ccc.getBlocks(false);
      }
    }
    else if(pixy.ccc.blocks[i].m_signature == 7){
      //turn right
      if(pixy.ccc.blocks[i].m_x > 158){
        brake(motor1, motor2);
        delay(1000);
        motor1.drive(150, 3500);
        brake(motor1, motor2);
        delay(1000);
      }
      else{
        brake(motor1, motor2);
        delay(1000);
        motor2.drive(150, 3500);
        brake(motor1, motor2);
        delay(1000);
      }
    }
  }
  //if no objects in way, move forward
  forward(motor1, motor2, 100);
}
