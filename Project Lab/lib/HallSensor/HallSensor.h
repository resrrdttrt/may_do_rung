#include <Arduino.h>


class HallSensor {
  public:
    HallSensor(uint8_t pin);
    float getMagneticFieldIntensityInPercent();
    float getRPM();
    float getFrequency();
    void setADCResolution(int resolution);

  private:
    int data;
    uint8_t _pin;
    int ADC_resolution=4095;
};

