 #include "Motors.h"
#include "linesensors.h"
#include "encoders.h"
#include "kinematics.h"
linefollowing line;
Motors_c motor;
Kinematics_c kinematics;
#define LINE_SENSOR_UPDATE 100
#define MOTOR_UPDATE 200
#define KINEMATICS_UPDATE 150

float rpm = 15;
float bend = 1.358 * rpm;
float corner = 1.6 * rpm;
float rpml = rpm * 1.1;//tuning to eliminate motor difference
float bendl = 1.358 * rpml;
float cornerl = 1.5 * rpml;
unsigned long ls_ts = millis();
unsigned long motor_ts = millis();
unsigned long kinematics_ts = millis();
int line_lost = 0;
float f;

unsigned long elapsed_t;
int i = 0;


//Note that sensors are ordered 0, 1, 2, 3, 4 for line following



void bangbang() {
  if (line.sensorstate[1] or line.sensorstate[2] or line.sensorstate[3]) {
    //if line is near center/line following
    if ((line.sensorstate[1] and line.sensorstate[2] and line.sensorstate[3]) or (!line.sensorstate[1] and line.sensorstate[2] and !line.sensorstate[3])) {
      //if line is center
      Motors_c motor(rpml, rpm);
      motor.set_pwm();
    }

    else if (line.sensorstate[1] and line.sensorstate[2] and !line.sensorstate[3]) {
      //if line slightly deviates left
      Motors_c motor(rpml, bend);
      motor.set_pwm();
    }

    else if (line.sensorstate[1] and !line.sensorstate[2] and !line.sensorstate[3]) {
      //if line can only be found on the left
      Motors_c motor(rpml, corner);
      motor.set_pwm();
    }

    else if (!line.sensorstate[1] and line.sensorstate[2] and line.sensorstate[3]) {
      //if line slightly deviates right
      Motors_c motor(bendl, rpm);
      motor.set_pwm();
    }

    else if (!line.sensorstate[1] and !line.sensorstate[2] and line.sensorstate[3]) {
      //if line can only be found on the right
      Motors_c motor(cornerl, rpm);
      motor.set_pwm();
    }
  }

  else if (line.sensorstate[0] or line.sensorstate[4]) {
    //line lost/ extreme control
    if ((line.sensorstate[0] and !line.sensorstate[4]) or (line.sensorstate[0] and line.sensorstate[4])) {
      //rotate anticlockwise if line cant be found or is found extreme left
      Motors_c motor(-rpml, rpm);
      motor.set_pwm();
    }

    else if (!line.sensorstate[0] and line.sensorstate[4]) {
      //rotate clockwise if line is found extreme right
      Motors_c motor(rpml, -rpm);
      motor.set_pwm();
    }
  }



}




void setup() {
  line.setupls();
  motor.initialise();
  setupEncoder0();
  setupEncoder1();
  // Start serial, send debug text.
  Serial.begin(9600);
  delay(1000);
  Serial.println("***RESET***");
}

void loop() {



  //  ( _ts = "time-stamp" )

  unsigned long current_ts;
  current_ts = millis();


  // Run our line sensor update
  // every 100ms (10hz).
  // Tracking time for the line sensor (ls)
  elapsed_t = current_ts - ls_ts;
  if (elapsed_t > LINE_SENSOR_UPDATE) {

    // Conduct a read of the line sensors
    line.readlinesensor();

    // Record when this execution happened.
    // for future iterations of loop()
    ls_ts = millis();
  }

  
  elapsed_t = current_ts - motor_ts;
  if (elapsed_t > MOTOR_UPDATE) {
    float z = kinematics.Z;
    float t = kinematics.theta;
    float y = kinematics.Y;
    float x = kinematics.X;


    //initiate Finite State Machine
    if (i < 3) {
      //map hasn't been completed

      if (z < 35) {
        //before the gap
        if (i == 0) {
          //Stage 0: find the Map
          if (!line.sensorstate[0] and !line.sensorstate[1] and !line.sensorstate[2] and !line.sensorstate[3] and !line.sensorstate[4]) {
            Motors_c motor(rpml, rpm);
            motor.set_pwm();


          } else {
            //line found
            i = 1;
          }
        }

        else if (i == 1) {
          //Stage 1: Find the beginning of the map
          if (!line.sensorstate[4]) {
            if (!line.sensorstate[0] and !line.sensorstate[1] and !line.sensorstate[2] and !line.sensorstate[3]) {
              //loss of all 4 line sensors indicates that the start of the line has been found
              i = 2;
            } else {
              bangbang();
            }
          } else {
            Motors_c motor(-rpml, rpm);
            motor.set_pwm();
          }
        }

        else if (i == 2) {
          //Stage 2: Follow the map
          if (line.sensorstate[0] or line.sensorstate[1] or line.sensorstate[2] or line.sensorstate[3] or line.sensorstate[4]) {
            bangbang();
          } else {
            Motors_c motor(-rpml, rpm);
            motor.set_pwm();
          }
        }
      } 
      
      else if (z >= 30 and z < 127) {
        //the gap
        if (line.sensorstate[0] or line.sensorstate[1] or line.sensorstate[2] or line.sensorstate[3] or line.sensorstate[4]) {
          bangbang();
        } else {
          Motors_c motor(rpml, rpm);
          motor.set_pwm();
        }
      }

      else if (z >= 127) {
        //after the gap
        if (line.sensorstate[0] or line.sensorstate[1] or line.sensorstate[2] or line.sensorstate[3] or line.sensorstate[4]) {
          bangbang();
        } else {
          //Possibly end of the map
          Motors_c motor(0, 0);
          motor.set_pwm();
          delay(50);
          i = 3;
        }
      }
    } else if (i == 3) {
      //confirm end of the map
      if ((line.sensorstate[0] or line.sensorstate[1] or line.sensorstate[2] or line.sensorstate[3] or line.sensorstate[4]) and line_lost < 5) {
        bangbang();
      } else if ((!line.sensorstate[0] and !line.sensorstate[1] and !line.sensorstate[2] and !line.sensorstate[3] and !line.sensorstate[4]) and line_lost < 5) {
        line_lost = line_lost + 1;

      } else {
        Motors_c motor(0, 0);
        motor.set_pwm();
        f = t;
        i = 4;
      }
    } else if (i == 4) {
      //calculate heading home
      // convert odometry angular displacement to between 0 and 360 degrees
      if (f <= 0) {
        f = f + 6.283;
      } else if (f > 6.283) {
        f = f - 6.283;
      } else {
        // calculate f to be a heading pointing at/near home
        if (f > 1.10 and f<3.142) {
          f = (f + 1.58);
        } else if (f < 1.00)  {
          f = (f + 3.142);
        } else if (f < 4.80 and f>3.142) {
          f = (f - 1.7);
        }else {
        f=(f-2.2);
        }

        i = 5;
      }
    }


    else if (i == 5) {
      //resume return to home sequence by seting heading towards home
      if (t < (f - 0.03)) {
        Motors_c motor(14, -14.5);
        motor.set_pwm();
      } else if (t > (f + 0.01)) {
        Motors_c motor(-14.5, 14);
        motor.set_pwm();
      }

      else {
        // drive in the direction of home until distance to home is within a set range.
        if (x > 3) {
          Motors_c motor(rpml*2, rpm*2);
          motor.set_pwm();
        } else if (x < 0) {
          Motors_c motor(-rpml*2, -rpm*2);
          motor.set_pwm();
        } else {
          Motors_c motor(0, 0);
          motor.set_pwm();
        }
      }
     
    }

  



  
    motor_ts = millis();
  
  }

  elapsed_t = current_ts - kinematics_ts;
  if (elapsed_t > KINEMATICS_UPDATE) {
    kinematics.update();
    kinematics_ts = millis();
    
     
  }
  
}