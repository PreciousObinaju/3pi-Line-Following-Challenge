#ifndef _LINESENSOR_H
#define _LINESENSOR_H
#define lightsensor1 A11
#define lightsensor5 A4


#define lightsensor2 A0
#define lightsensor3 A2
#define lightsensor4 A3
#define emit 11
#define nblightsensors 5
#define led 13
#define active 1150


class linefollowing {
public:
  unsigned long sensorread[nblightsensors];
  bool sensorstate[nblightsensors];
  int pin;
  int lightsensor[nblightsensors] = { lightsensor1, lightsensor2, lightsensor3, lightsensor4, lightsensor5 };
  unsigned long start_time;
  unsigned long timeout = 9000;
  unsigned long current_time;
  unsigned long elapsed_time = current_time - start_time;
  int remaining;
  linefollowing() {
  }

  void setupls() {
    pinMode(emit, OUTPUT);
    pinMode(lightsensor1, INPUT);
    pinMode(lightsensor2, INPUT);
    pinMode(lightsensor3, INPUT);
    pinMode(lightsensor4, INPUT);
    pinMode(lightsensor5, INPUT);
    pinMode(led, OUTPUT);
      digitalWrite(emit, HIGH);
  }

  void readlinesensor() {
    start_time = micros();
    setsensorread();
    chargecapacitor();
    setasinput();
    remaining = nblightsensors;
    while (remaining > 0) {
      readsensors();
      checktimeout();
    }
alertblackline();
    
  }

  void setsensorread() {
    for (pin = 0; pin < nblightsensors; pin++) {
      sensorread[pin] = 0;
    }
  }

   void setsensorstate() {
    for (pin = 0; pin < nblightsensors; pin++) {
      sensorstate[pin] = false;
    }
  }

  void chargecapacitor() {
    for (pin = 0; pin < nblightsensors; pin++) {
      pinMode(lightsensor[pin], OUTPUT);
      digitalWrite(lightsensor[pin], HIGH);
    }
    delayMicroseconds(10);
  }

  void setasinput() {
    for (pin = 0; pin < nblightsensors; pin++) {
      pinMode(lightsensor[pin], INPUT);
    }
  }



  void readsensors() {
    for (pin = 0; pin < nblightsensors; pin++) {

      if (digitalRead(lightsensor[pin]) == LOW) {

        if (sensorread[pin] == 0) {
          current_time = micros();
          elapsed_time = current_time - start_time;
          sensorread[pin] = elapsed_time;

          if(sensorread[pin]>= active){
            sensorstate[pin]= true;
          }

          else {
           sensorstate[pin]= false;
          }
          remaining = remaining - 1;
        }
      }
    }
  }

  void checktimeout() {
    if (elapsed_time >= timeout) {
      remaining = 0;
    }
  }

  void printresult() {
    for (pin = 0; pin < nblightsensors; pin++) {
      Serial.print(" line sensor");
      Serial.print(pin + 1);
      Serial.print(" : ");
      Serial.print(sensorread[pin]);
      Serial.print("\n");
    }
  }

  void alertblackline(){
if (sensorread[0] >= active or sensorread[1] >= active or sensorread[2] >= active or sensorread[3] >= active or sensorread[4] >= active) {
      digitalWrite(led, HIGH);
    }

    else {
      digitalWrite(led, LOW);
    }
  }
};

#endif