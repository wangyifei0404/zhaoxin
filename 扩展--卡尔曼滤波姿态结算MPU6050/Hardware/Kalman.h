#ifndef __KALMAN_H
#define __KALMAN_H

typedef struct {
    float Q_angle;  
    float Q_gyro;   
    float R_angle;  
    float dt;       

    float angle;    
    float Q_bias;   

    float P[2][2];  
} Kalman_t;
void Kalman_Init(Kalman_t *kalman);
float Kalman_GetAngle(Kalman_t *kalman, float newAngle, float newGyro, float dt);

#endif
