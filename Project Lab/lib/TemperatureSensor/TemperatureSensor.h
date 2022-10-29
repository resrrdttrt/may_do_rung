#include <Arduino.h>
#pragma once


class TemperatureSensor {
  public:
    TemperatureSensor(uint8_t);
    float getTemperature(bool inFahrenheit=false);
    void setInputVoltageInVolt(int);
    void setADCResolution(int);

  private:
    int data;
    uint8_t _pin;
    int input_volt=5000;
    int ADC_resolution=4095;
};

