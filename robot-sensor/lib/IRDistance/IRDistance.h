#ifndef _IRDISTANCE_H_
#define _IRDISTANCE_H_

#include <Arduino.h>

#define IR_DEFAULT_DELAY 25
#define IR_DEFAULT_COUNTS 10

class IRDistance
{
public:
  IRDistance(int IRPin);
  double Distance();
  double DistanceAvg(int wait = IR_DEFAULT_DELAY, int count = IR_DEFAULT_COUNTS);

private:
  int _IRPin;

};
#endif
