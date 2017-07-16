#include <Arduino.h>
#include "Motor.h"

Motor::Motor()
{
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);  //配置电机驱动IO口为输出s
}

void Motor::motospd(int sp1, int sp2)
{
  if(sp1 > 0)
    digitalWrite(INB, HIGH);
  else
    digitalWrite(INB, LOW);
  if(sp2 > 0)
    digitalWrite(INA, HIGH);
  else
    digitalWrite(INA, LOW);
  analogWrite(PWMA, abs(sp2));
  analogWrite(PWMB, abs(sp1));
}

void Motor::stopper()
{
  Motor::motospd(0,0);
}

void Motor::forward()
{
  Motor::motospd(100, 100);
}

void Motor::back()
{
  Motor::motospd(-100, -100);
}

void Motor::turnLeft30()
{
  for(int i = 0; i < 6; i++)
	 {
		Motor::motospd(40+i*20,-(40+i*20));
		delay(20*i+20);
	 }
}

void Motor::turnLeft60()
{
  for(int i = 0; i < 2; i++)
  {
    Motor::turnLeft30();
    Motor::stopper();
    delay(500);
  }
}

void Motor::turnLeft90()
{
  for(int i = 0; i < 3; i++)
  {
    Motor::turnLeft30();
    Motor::stopper();
    delay(500);
  }
}

void Motor::turnRight30()
{
  for(int i = 0; i < 6; i++)
	{
		Motor::motospd(-(40+i*20),40+i*20);
		delay(20*i+20);
	}
}

void Motor::turnRight60()
{
  for(int i = 0; i < 2; i++)
  {
    Motor::turnRight30();
    Motor::stopper();
    delay(500);
  }
}

void Motor::turnRight90()
{
  for(int i = 0; i < 3; i++)
  {
    Motor::turnRight30();
    Motor::stopper();
    delay(500);
  }
}

void Motor::moveRound(){
  for(int i = 0; i < 12; i++){
    Motor::turnLeft30();
    Motor::stopper();
    delay(500);
  }
}

//向左微调
void Motor::turnLeft(){
  Motor::motospd(150, 100);
}

//向右微调
void Motor::turnRight(){
  Motor::motospd(100, 150);
}

void Motor::turn(int pos)
{
    switch (pos) {
      case -90: Motor::turnRight90(); Serial.println("turnRight90");
                break;
      case -60: Motor::turnRight60(); Serial.println("turnRight60");
                break;
      case -30: Motor::turnRight30(); Serial.println("turnRight30");
                break;
      case  0 : Motor::stopper(); Serial.println("stop");
                break;
      case  30: Motor::turnLeft30(); Serial.println("turnLeft30");
                break;
      case  60: Motor::turnLeft60(); Serial.println("turnLeft60");
                break;
      case  90: Motor::turnLeft90(); Serial.println("turnLeft90");
                break;
    }
}
