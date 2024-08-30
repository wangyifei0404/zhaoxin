#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "sys.h"


float pitch,roll,yaw;

int main(void)
{
	OLED_Init();
	mpu_dmp_init();
	

	OLED_ShowString(1,1,"Pitch:");
	OLED_ShowString(2,1,"Roll:");
	OLED_ShowString(3,1,"Yaw:");
	
	while(1)
	{
		mpu_dmp_get_data(&pitch,&roll,&yaw);
		OLED_ShowSignedNum(1,7,pitch,5);
		OLED_ShowSignedNum(2,7,roll,5);
		OLED_ShowSignedNum(3,7,yaw,5);
	}
}
