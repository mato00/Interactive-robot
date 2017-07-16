# Interactive-robot
engineering project about interactive robot in unfamiliar environment containing vision/speech/motion algorithms.

## 发起这项工程的原因
之前一直是一个人负责预研项目——陌生环境下自主运动与交互的移动机器人，之后由于产品需求临时改成做一款STEM可编程机器人，毕竟之前做的产品都是为少儿双语教学人工智能机器人服务，于是花了两天时间把原来的代码改完交付，本来想搭建一个智能小车作为实验平台跑各种机器视觉/语音/运动控制和SLAM的算法，现在相当于交付了实验平台，我通过改动之前的代码，将小车平台分为休眠/自主运动/遥控模式，并且将传感器采集数据和小车运动都模块化，方便之后开发时可以改成类似Scratch可拖拽的可视化编程，最终的产品应该类似于小米的米兔，只不过最终硬件上要加强，把驱动模块和传感器采集判断模块分布到不同的mcu集群上。
但之前做的研究不能浪费，于是在github上开了这个项目，之后会不断更新直至该项目最终成型。

## 工程模块
为了方便该工程的逐步推进，讲该工程分为四大模块：
* 机器人的运动模块
* 机器人的定位模块
* 机器人的交互模块
* 最终的整合与通信

### 机器人的运动模块
基于之前工作的经验，要想在小车同时采用ros同步即时采集传感器数据，最好用讲小车驱动和传感器分布到两个线程。之前尝试基于Arduino单核写个伪多线程，结果发现时钟不够用，Arduino还是太弱了，不过正好手头有两块Arduino UNO，不妨尝试加一块。
