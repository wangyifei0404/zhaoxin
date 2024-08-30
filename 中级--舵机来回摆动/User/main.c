#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"
#include "Key.h"


float Angle=0;
int flag=1;

int main(void)
{
	OLED_Init();
	Servo_Init();
	
	OLED_ShowString(1,1,"Angle:");

	//while(1)
	{
		switch(flag)
		{
			case 1:Angle += 1;break;
			default :Angle -= 1;break;
		}
			
		if (Angle == 180)
		{
			flag = 2;
		}
		if (Angle == 0)
		{
			flag = 1;
		}
		Servo_SetAngle(Angle);
		Delay_ms(50);
		OLED_ShowNum(1,7,Angle,3);
	}
}
