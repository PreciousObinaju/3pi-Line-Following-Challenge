#ifndef _MOTORS_H
#define _MOTORS_H

#define L_PWM_PIN 10
#define L_DIR_PIN 16
#define R_PWM_PIN 9
#define R_DIR_PIN 15

#define FWD LOW
#define REV HIGH


// Class to operate the motor(s).
class Motors_c {
public:
  float left_pwm;
  float right_pwm;
  // Constructor, must exist.
  Motors_c() {
    left_pwm = 0;
    right_pwm = 0;
  }

  Motors_c(float lleftpwm, float rrightpwm) {
    left_pwm = lleftpwm;
    right_pwm = rrightpwm;
  }

  // Use this function to
  // initialise the pins and
  // state of your motor(s).
  void initialise() {
    pinMode(L_PWM_PIN, OUTPUT);
    pinMode(L_DIR_PIN, OUTPUT);
    pinMode(R_PWM_PIN, OUTPUT);
    pinMode(R_DIR_PIN, OUTPUT);
    digitalWrite(R_DIR_PIN, FWD);
    digitalWrite(L_DIR_PIN, FWD);

    analogWrite(R_PWM_PIN, 0);
    analogWrite(L_PWM_PIN, 0);
  }

  // Write a function to operate
  // your motor(s)
  // ...
  void set_pwm() {

    if (left_pwm <= 0 and right_pwm <= 0) {
      digitalWrite(L_DIR_PIN, REV);
      digitalWrite(R_DIR_PIN, REV);
    } else if (left_pwm <= 0 and right_pwm > 0) {
      digitalWrite(L_DIR_PIN, REV);
      digitalWrite(R_DIR_PIN, FWD);
    } else if (left_pwm > 0 and right_pwm <= 0) {
      digitalWrite(L_DIR_PIN, FWD);
      digitalWrite(R_DIR_PIN, REV);
    } else {
      digitalWrite(L_DIR_PIN, FWD);
      digitalWrite(R_DIR_PIN, FWD);
    }
    left_pwm = abs(left_pwm);
    right_pwm = abs(right_pwm);
  /*  if (left_pwm < 13 or left_pwm > 200 or right_pwm < 13 or right_pwm > 200) {
      analogWrite(L_PWM_PIN, 0);
      analogWrite(R_PWM_PIN, 0);
    } else {*/
      analogWrite(L_PWM_PIN, left_pwm);
      analogWrite(R_PWM_PIN, right_pwm);
  //  }
  }
};


#endif