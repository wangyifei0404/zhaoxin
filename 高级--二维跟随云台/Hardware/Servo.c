#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Servo_Init(void)
{
	PWM_Init();
}

void Servo_SetAngle2(float Angle)
{
	PWM_SetCompare2(Angle/180*2000+500);
}

void Servo_SetAngle1(float Angle)
{
	PWM_SetCompare1(Angle/180*2000+500);
}
 
float map(float value,float fromLow,float fromHigh,float toLow,float toHigh)
{
	return ((value-fromLow)*(toHigh-toLow)/(fromHigh-fromLow)+toLow);
}
