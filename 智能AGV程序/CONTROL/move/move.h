#ifndef __MOVE_H
#define __MOVE_H
#include "system.h"
#include "servo.h"
#define PI					3.1415926535
#define Wheel_Diameter		0.195
#define Wheel_Track			0.400
#define Reduction_Ratio 	40
#define Encoder_Accuracy	10000
#define Velocity_Factor		240
#define Accelera_Factor		600
#define Decelera_Factor		600
#define	Speed_Conversion Reduction_Ratio*Velocity_Factor/(PI*Wheel_Diameter)


void Car_Control(void);
void Kinematic_Analysis(double Vel, double Pal);
#endif


