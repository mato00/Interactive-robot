#include "IRDistance.h"

IRDistance::IRDistance(int IRPin)
{
  _IRPin = IRPin;
}

double IRDistance::Distance()
{
  double IR_value = analogRead(_IRPin);
  int distance_cm = pow(3027.4/IR_value, 1.2134); //convert readings to distance(cm)
  delay(25);
  return distance_cm;
}

double IRDistance::DistanceAvg(int wait, int count)
{
  double min, max, avg, d;
  min = 999.0;
  max = 0.0;
  avg = d = 0.0;

  if (wait < 25) {
      wait = 25;
  }

  if (count < 1) {
      count = 1;
  }

  for (int x = 0; x < count + 2; x++) {
      d = Distance();

      if (d < min) {
          min = d;
      }

      if (d > max) {
          max = d;
      }

      avg += d;
      delay(wait);
  }

  // substract highest and lowest value
  avg -= (max + min);
  // calculate average
  avg /= count;
  return avg;

}
