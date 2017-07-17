#include <Arduino.h>
#include <Servo.h>
#include <SR04.h>
#include <IRDistance.h>
#include <MPU9250.h>
#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/Range.h>
#include <sensor_msgs/Imu.h>

// 舵机
Servo Myservo1;
// 舵机角度
int angle1 = 0;

// 红外测距
IRDistance IRSensor(A0); // 前方红外测距
const int ir_sensor = A0; //分配激光传感器
const int xoy_val = 7;
float xoyDistance[xoy_val]; //存储xoy平面每30度的距离信息

// ros节点声明
ros::NodeHandle nh;

// 定义传感器msg
sensor_msgs::LaserScan scan;
sensor_msgs::Range ir_range_msg;
sensor_msgs::Range ultra_left_msg;
sensor_msgs::Range ultra_right_msg;
sensor_msgs::Range ultra_along_msg;

// 定义传感器publisher
ros::Publisher scan_range("scan", &scan);
ros::Publisher ir_range( "ir_data", &ir_range_msg);

unsigned long range_timer;

void irScanRange(){
  float ir_range[xoy_val];
  for(angle1 = 0; angle1 < 181; angle1 += 30)
  {
    Myservo1.write(angle1);
    delay(500);
    ir_range[angle1/30] = IRSensor.Distance()/100;
    delay(500);
  }
  Myservo1.write(90);
  delay(500);
  // 填充红外测距数据
  scan.header.stamp = nh.now();

  for(unsigned int i = 0; i < xoy_val; i++){
    scan.ranges[i] = ir_range[i];
  }
  scan_range.publish(&scan);
}

//红外正前方测距
void irPowRange(){
  ir_range_msg.range = IRSensor.Distance()/100;
  ir_range_msg.header.stamp = nh.now();
  ir_range.publish(&ir_range_msg);

}

void setup(){
  Serial.begin(115200);
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.spinOnce();
  // nh.advertise(scan_range);
  // nh.spinOnce();
  // delay(1000);
  nh.advertise(ir_range);
  nh.spinOnce();
  delay(500);
  scan.header.frame_id = "/ir_scan";
  scan.angle_min = 0;
  scan.angle_max = 3.14;
  scan.angle_increment = 3.14 / xoy_val;
  scan.time_increment = 7 / xoy_val;
  scan.range_min = 0.2;
  scan.range_max = 1.5;

  ir_range_msg.radiation_type = sensor_msgs::Range::INFRARED;
  ir_range_msg.header.frame_id =  "/ir_range";
  ir_range_msg.field_of_view = 0.01;
  ir_range_msg.min_range = 0.2;
  ir_range_msg.max_range = 1.5;

  // 初始化舵机
  Myservo1.attach(10);
  Myservo1.write(90);
}

void loop(){
  if((millis() - range_timer) > 50){
    irPowRange();
    range_timer = millis();
  }
  nh.spinOnce();
  delay(500);
}
