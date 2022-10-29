#include <Arduino.h>
#include <MPU9250.h>


struct Data{
    float x, y, z;
};

class MotionSensor{
    public:
        MotionSensor(const uint8_t addr);
        Data getAcc();
        Data getGyro();
        Data getLinearAcc();
        Data getRPY();
        Data getEuler();
        void calibrateAccGyro();
        float getTemperature();
    
    private:
        Data Acc,Gyro,LinearAcc,RPY,Euler;
        float temperature;
        MPU9250 mpu;
};