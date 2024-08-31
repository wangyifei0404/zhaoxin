#include "stm32f10x.h"                  // Device header

#include <math.h>


// 卡尔曼滤波器结构体
typedef struct {
    float Q_angle;  // 角度噪声协方差
    float Q_gyro;   // 陀螺仪噪声协方差
    float R_angle;  // 角度测量噪声协方差
    float dt;       // 采样周期

    float angle;    // 当前角度
    float Q_bias;   // 漂移量

    float P[2][2];  // 协方差矩阵
} Kalman_t;

// 初始化卡尔曼滤波器
void Kalman_Init(Kalman_t *kalman) {
    kalman->Q_angle = 0.001;
    kalman->Q_gyro = 0.003;
    kalman->R_angle = 0.03;
    kalman->dt = 0.01;

    kalman->angle = 0;
    kalman->Q_bias = 0;
    
    kalman->P[0][0] = 1;
    kalman->P[0][1] = 0;
    kalman->P[1][0] = 0;
    kalman->P[1][1] = 1;
}

// 更新卡尔曼滤波器
float Kalman_GetAngle(Kalman_t *kalman, float newAngle, float newGyro, float dt) {
    // 预测当前角度
    kalman->angle = kalman->angle - kalman->Q_bias * dt + newGyro * dt;
    
    // 预测协方差矩阵
    kalman->P[0][0] = kalman->P[0][0] + kalman->Q_angle - (kalman->P[0][1] - kalman->P[1][0]) * dt;
    kalman->P[0][1] = kalman->P[0][1] - kalman->P[1][1] * dt;
    kalman->P[1][0] = kalman->P[1][0] - kalman->P[1][1] * dt;
    kalman->P[1][1] = kalman->P[1][1] + kalman->Q_gyro;

    // 计算卡尔曼增益
    float S = kalman->P[0][0] + kalman->R_angle;
    float K_0 = kalman->P[0][0] / S;
    float K_1 = kalman->P[1][0] / S;

    // 更新角度和漂移量的估计
    float y = newAngle - kalman->angle;
    kalman->angle = kalman->angle + K_0 * y;
    kalman->Q_bias = kalman->Q_bias + K_1 * y;

    // 更新协方差矩阵
    float P00_temp = kalman->P[0][0];
    float P01_temp = kalman->P[0][1];
    
    kalman->P[0][0] = P00_temp - K_0 * P00_temp;
    kalman->P[0][1] = P01_temp - K_0 * P01_temp;
    kalman->P[1][0] = kalman->P[1][0] - K_1 * P00_temp;
    kalman->P[1][1] = kalman->P[1][1] - K_1 * P01_temp;

    return kalman->angle;
}
