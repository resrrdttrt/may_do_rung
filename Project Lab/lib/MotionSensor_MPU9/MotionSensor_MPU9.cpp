#include<MotionSensor_MPU9.h>


MotionSensor::MotionSensor(const uint8_t addr=0x68){
    mpu.setup(addr);
}

Data MotionSensor::getAcc(){
    mpu.update_accel_gyro();
    Acc.x=mpu.getAccX();
    Acc.y=mpu.getAccY();
    Acc.z=mpu.getAccZ();
    return Acc;
}
Data MotionSensor::getGyro(){
    mpu.update_accel_gyro();
    Gyro.x=mpu.getGyroX();
    Gyro.y=mpu.getGyroY();
    Gyro.z=mpu.getGyroZ();
    return Gyro;
}
Data MotionSensor::getLinearAcc(){
    mpu.update_accel_gyro();
    LinearAcc.x=mpu.getLinearAccX();
    LinearAcc.y=mpu.getLinearAccY();
    LinearAcc.z=mpu.getLinearAccZ();
    return LinearAcc;
}
Data MotionSensor::getRPY(){
    if(mpu.update()){
        RPY.x=mpu.getRoll();
        RPY.y=mpu.getYaw();
        RPY.z=mpu.getPitch();
        return RPY;
    }
}
Data MotionSensor::getEuler(){
    if(mpu.update()){
        Euler.x=mpu.getEulerX();
        Euler.y=mpu.getEulerY();
        Euler.z=mpu.getEulerZ();
        return Euler;
    }
}
void MotionSensor::calibrateAccGyro(){
    mpu.calibrateAccelGyro(); 
}

float MotionSensor::getTemperature(){
    if(mpu.update()){
        temperature=mpu.getTemperature();
        return temperature;
    }
}