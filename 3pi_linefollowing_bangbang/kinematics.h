// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _KINEMATICS_H
#define _KINEMATICS_H

// Class to track robot position.
class Kinematics_c {
  public:
  unsigned long previouslcount = 0;
unsigned long previousrcount = 0;
unsigned long currentlcount;
unsigned long currentrcount;
long lcount;
long rcount;
float Xr;
float thetar;
float Y = 0;
float X = 0;
float Z = 0;
float theta = 0;
   
    Kinematics_c() {

    } 

 
    void update() {
 currentlcount = count_l;
    currentrcount = count_r;
    lcount = currentlcount - previouslcount;
    rcount = currentrcount - previousrcount;


    Xr = 0.01404 * (lcount + rcount);
    thetar = 0.0033036794 * (lcount - rcount);
    X = X + Xr * cos(theta);
    Y = Y + Xr * sin(theta);
    theta = theta + thetar;
    Z = sqrt((X*X) + (Y*Y));
    previouslcount = currentlcount;
    previousrcount = currentrcount;

    

    }

};



#endif
