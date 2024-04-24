# 3pi+ Line Following Challenge
The Pololu 3Pi+ 32u4 Robot - STD Edition is a versatile robot platform designed for educational and hobbyist purposes. It features an ATmega32U4 microcontroller, allowing for programmability using the Arduino IDE. With its array of sensors and motor controllers, the 3Pi+ enables users to explore various robotics concepts such as line following, maze solving, and remote control applications.
![image](https://github.com/PreciousObinaju/3pi-Line-Following-Challenge/assets/147056844/5ae7696c-5fcd-48ab-bf66-639a7c9e70c4)

# Task Definition
This code was written to fulfil a set of requirements set out to complete the line following challenge using the map in the image below:
![Screenshot 2024-04-23 231105](https://github.com/PreciousObinaju/3pi-Line-Following-Challenge/assets/147056844/de174e68-21a6-401b-bc74-e1a71365b19f)

The rules were as follows:
1. the code must be such that the 3pi+ should be able to complete the map from both start boxes without any changes to the code
2. the 3pi+ must cross the gap and follow the line.
3. The 3pi+ doesn't need to start from the begining of the line map however it must exit the starting box and join the line.
4. The 3pi+ must be able to identify the end of the line and return to the start box

# Files Description
The Overall Solution to this Challenge is Coded in 5 files servint the following purposes
1. **3pi_linefollowing_bangbang.ino**: Contains the executable code sequence for the 3pi+ to complete the line following challenge.
2. **Motors.h**: Contains code used to inteprete the direction and speed of the left and right wheel rotation.
3. **encoders.h**: Contains code used to read the rotary encoders on the 3pi+.
4. **kinematics.h**: Contains code that converts the encoder readings to estimate position and rotation of the robot on a 2d plane
5. **linesensors.h**: Contains all code necessary to read line sensors.
