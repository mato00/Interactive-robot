#include <Motor.h>
#include <IRremote.h>

// 电机初始化
Motor PiCar;

// 红外遥控初始化
int remote_pin = 2;
IRrecv irrecv(remote_pin);
decode_results results;
// 红外遥控指令
typedef enum{
  _MOVEBACK,
  _MOVEFORWARD,
  _STOP,
  _TURNLEFT30,
  _TURNRIGHT30
}command;
command carCommand;
// 遥控指令翻译
command translateIR(){
switch (results.value) {
  case 0xFFA25D:
    // Serial.println(" Move Back.            ");
    carCommand = _MOVEBACK;
    break;
  case 0xFF629D:
    // Serial.println(" STOP.             ");
    carCommand = _STOP;
    break;
  case 0xFFE21D:
    // Serial.println(" Move Forward.            ");
    carCommand = _MOVEFORWARD;
    break;
  case 0xFF22DD:
    // Serial.println(" turnLeft 30.          ");
    carCommand = _TURNLEFT30;
    break;
  case 0xFF02FD:
    // Serial.println(" turnRight 30.        ");
    carCommand = _TURNRIGHT30;
    break;
  case 0xFFC23D:
    // Serial.println(" PLAY/PAUSE     ");
    carCommand = _STOP;
    break;
  case 0xFFE01F:
    // Serial.println(" VOL-           ");
    carCommand = _STOP;
    break;
  case 0xFFA857:
    // Serial.println(" VOL+           ");
    carCommand = _STOP;
    break;
  case 0xFF906F:
    // Serial.println(" EQ             ");
    carCommand = _STOP;
    break;
  case 0xFF6897:
    // Serial.println(" 0              ");
    carCommand = _STOP;
    break;
  case 0xFF9867:
    // Serial.println(" 100+           ");
    carCommand = _STOP;
    break;
  case 0xFFB04F:
    // Serial.println(" 200+           ");
    carCommand = _STOP;
    break;
  case 0xFF30CF:
   // Serial.println(" 1              ");
   carCommand = _STOP;
    break;

  case 0xFF18E7:
    // Serial.println(" 2              ");
    carCommand = _STOP;
    break;

  case 0xFF7A85:
    // Serial.println(" 3              ");
    carCommand = _STOP;
    break;

  case 0xFF10EF:
    // Serial.println(" 4              ");
    carCommand = _STOP;
    break;

  case 0xFF38C7:
    // Serial.println(" 5              ");
    carCommand = _STOP;
    break;

  case 0xFF5AA5:
    // Serial.println(" 6              ");
    carCommand = _STOP;
    break;

  case 0xFF42BD:
    // Serial.println(" 7              ");
    carCommand = _STOP;
    break;

  case 0xFF4AB5:
    // Serial.println(" 8              ");
    carCommand = _STOP;
    break;

  case 0xFF52AD:
    // Serial.println(" 9              ");
    carCommand = _STOP;
    break;

  default:
    Serial.print(" unknown button   ");
    carCommand = _STOP;
  }
  delay(250);

  return carCommand;
}
// 红外遥控接受信号
command remote(){
  if(irrecv.decode(&results)){
      Serial.println("car remote judging...");
      carCommand = translateIR();
      irrecv.resume();
  }else
    Serial.println("no signal.");
  delay(500);
  return carCommand;
}

void carMove(command _carCommand){
  switch (_carCommand) {
    case _STOP:
      PiCar.stopper();
      Serial.println("Stop.");
      break;
    case _MOVEFORWARD:
      PiCar.forward();
      Serial.println("Forward.");
      break;
    case _MOVEBACK:
      PiCar.back();
      Serial.println("Back.");
      break;
    case _TURNLEFT30:
      PiCar.turnLeft30();
      Serial.println("turnLeft30.");
      break;
    case _TURNRIGHT30:
      PiCar.turnRight30();
      Serial.println("turnRight30.");
      break;
    default:
      _carCommand = _STOP;
      carMove(_carCommand);
  }
}

void setup(){
  Serial.begin(9600);
  PiCar.stopper();
  irrecv.enableIRIn();
}

void loop(){
  carCommand = remote();
  carMove(carCommand);
}
