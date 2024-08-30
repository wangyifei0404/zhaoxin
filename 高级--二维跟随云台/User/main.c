#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "sys.h"
#include "Servo.h"


float pitch,roll,yaw;
float angleX = 0;
float angleY = 0;
float angleZ = 0;

int main(void)
{
	OLED_Init();
	mpu_dmp_init();
	Servo_Init();
	

	OLED_ShowString(1,1,"Pitch:");
	OLED_ShowString(2,1,"Roll:");
	OLED_ShowString(3,1,"Yaw:");
	
	while(1)
	{
		mpu_dmp_get_data(&pitch,&roll,&yaw);
		OLED_ShowSignedNum(1,7,pitch,3);
		OLED_ShowSignedNum(2,7,roll,3);
		OLED_ShowSignedNum(3,7,yaw,3);
		angleZ = map(yaw,-180,180,0,180);
		angleY = map(pitch,-180,180,0,180);
		Servo_SetAngle1(angleZ);
		Servo_SetAngle2(angleY);
	}
}
