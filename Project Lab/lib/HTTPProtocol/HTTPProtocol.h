#include <Arduino.h>
#include <HTTPClient.h>
#pragma once

#ifndef my_data
#define my_data
  struct Data{
    float x, y, z;
  };
#endif

class HTTP{
  public:
    HTTP();
    bool send(Data,Data,float);
    bool send(int);

  private:
    String server_url="https://hustsmate.000webhostapp.com/";
};