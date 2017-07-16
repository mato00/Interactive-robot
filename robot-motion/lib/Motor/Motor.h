#ifndef _MOTOR_H_
#define _MOTOR_H_

class Motor{
public:
  Motor();
  void motospd(int sp1, int sp2); //电机速度控制
  void stopper(); // 小车停止
  void forward(); // 小车前进
  void back(); //小车后退
  void turnRight30();
  void turnRight60();
  void turnRight90();
  void turnLeft30();
  void turnLeft60();
  void turnLeft90();
  void moveRound();
  void turn(int pos);
  void turnLeft();
  void turnRight();

  int INA = 4; //电机A正反控制端
  int PWMA = 5; //电机A调速端
  int INB = 7; //电机B正反控制端
  int PWMB = 6; //电机B调速端

private:
};

#endif
