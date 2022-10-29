#include<MotionSensor_MPU6.h>


MotionSensor::MotionSensor(){
  mpu.beginAccel();
  mpu.beginGyro();
}

Data MotionSensor::getAcc(){
    Acc.x=mpu.accelX();
    Acc.y=mpu.accelY();
    Acc.z=mpu.accelZ();
    Acc_sqrt=mpu.accelSqrt();
    return Acc;
}
Data MotionSensor::getGyro(){
    Gyro.x=mpu.gyroX();
    Gyro.y=mpu.gyroY();
    Gyro.z=mpu.gyroZ();
    return Gyro;
}

float MotionSensor::getAccSqrt(){
    return Acc_sqrt;
}

bool MotionSensor::update(){
    uint8_t result1 = mpu.gyroUpdate();
    uint8_t result2 = mpu.accelUpdate();
    if (result1 !=0 || result2 !=0){
        return false;
    }
    else{
        return true;
    }
}