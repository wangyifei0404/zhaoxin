#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "MPU6050.h"
#include "Kalman.h"
#include <math.h>


int16_t AX,AY,AZ,GX,GY,GZ;
float AngleX, AngleY;
Kalman_t kalmanX;  // X轴的卡尔曼滤波器
Kalman_t kalmanY;  // Y轴的卡尔曼滤波器
float M_PI = 3.14159265358979323846;

int main(void)
{
	  float dt = 0.01f;  // 时间步长

    OLED_Init();
    MPU6050_Init();

    Kalman_Init(&kalmanX);  // 初始化卡尔曼滤波器
    Kalman_Init(&kalmanY);

    while(1)
    {
        MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);


        float accX = AX / 16384.0f;
        float accY = AY / 16384.0f;
        float accZ = AZ / 16384.0f;
        
        float gyroX = GX / 131.0f;
        float gyroY = GY / 131.0f;

        float roll = atan2(accY, accZ) * 180 / M_PI;
        float pitch = atan2(-accX, sqrt(accY * accY + accZ * accZ)) * 180 / M_PI;


        AngleX = Kalman_GetAngle(&kalmanX, roll, gyroX, dt);
        AngleY = Kalman_GetAngle(&kalmanY, pitch, gyroY, dt);


        OLED_ShowSignedNum(2, 1, AngleX, 5);
        OLED_ShowSignedNum(3, 1, AngleY, 5);


        OLED_ShowSignedNum(2, 8, GX, 5);
        OLED_ShowSignedNum(3, 8, GY, 5);
        OLED_ShowSignedNum(4, 8, GZ, 5);
        

        Delay_ms(10);  // 10ms延时，对应时间步长dt
	}
}
