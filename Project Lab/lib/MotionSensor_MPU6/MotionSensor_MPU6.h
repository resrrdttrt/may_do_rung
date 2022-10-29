#include <Arduino.h>
#include <MPU9250_asukiaaa.h>
#pragma once


#ifndef my_data
#define my_data
  struct Data{
    float x, y, z;
  };
#endif

class MotionSensor{
    public:
        MotionSensor();
        Data getAcc();
        float getAccSqrt();
        Data getGyro();
        bool update();
    
    private:
        Data Acc,Gyro;
        float Acc_sqrt;
        MPU9250_asukiaaa mpu;
};