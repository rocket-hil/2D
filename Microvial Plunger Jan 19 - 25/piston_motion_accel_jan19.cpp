// UDF to describe motion in the y-direction of a piston pushing fluid up the cannula
// Accelerates for the first 3 cm of the cannula, travels at 1.2 m/s for the remainder to 
//  a height of 10.5 cm. 

#include "udf.h"

static real barrel_length = 0.01706;
static real acc_dist = 0.003; //distance over which piston accelerates in m
static real vel_y = 0.42; // Target y velocity
static real vel_y_current = 0.0; // Current y velocity
static real vel_y_0 = 0.0; // Initial y velocity
static real acc_y = (vel_y*vel_y)/(2*acc_dist); // a = (vf)^2-(vi^2)/2d, vi always equal to zero
static real acc_time = vel_y / acc_y; // t = vf-vi/a Time to accelerate
static real steady_time = (barrel_length - acc_dist - 0.0001)/ vel_y; // ((barrel-length)-(acceleration-length)- 0.0005m)/ speed   Time to maintain target velocity. 

DEFINE_CG_MOTION(box, dt, vel, omega, time, dtime) {
    NV_S(vel, =, 0.0); // ANSYS velocity vector
     
    if (time < acc_time) {
      vel_y_current = vel_y_0 + (acc_y * time);
    
    } else if (time >= acc_time && time < (steady_time + acc_time)) {
        vel_y_current = vel_y;
    
    } else {
        vel_y_current  = 0.0;
    }

    vel[1] = vel_y_current;
    
    Message ("time = %f, vel[1] = %f, acceleration time = %f, total time of motion = %f\n", time, vel[1], acc_time, acc_time + steady_time);
    
}
