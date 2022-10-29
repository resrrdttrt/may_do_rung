#include <TemperatureSensor.h>


TemperatureSensor::TemperatureSensor(uint8_t pin){
  _pin = pin;
}

float TemperatureSensor::getTemperature(bool inFahrenheit){
    int adcVal = analogRead(_pin);
    float milliVolt = adcVal * (input_volt / ADC_resolution);
    float tempC = milliVolt / 10;
    if (!inFahrenheit){
        return tempC;
    }
    else{
        float tempF = tempC * 9 / 5 + 32;
        return tempF;
    }
}

void TemperatureSensor::setInputVoltageInVolt(int voltage){
    input_volt = voltage*1000;
}

void TemperatureSensor::setADCResolution(int resolution){
    ADC_resolution = resolution;
}