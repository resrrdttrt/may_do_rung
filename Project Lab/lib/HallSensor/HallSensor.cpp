#include <HallSensor.h>


HallSensor::HallSensor(uint8_t pin){
  _pin = pin;
}

float HallSensor::getMagneticFieldIntensityInPercent(){
    int adcVal = analogRead(_pin);
    float percent = adcVal/ ADC_resolution;
    return percent;

}

float HallSensor::getRPM(){

}
float HallSensor::getFrequency(){
  
}


void HallSensor::setADCResolution(int resolution){
    ADC_resolution = resolution;
}