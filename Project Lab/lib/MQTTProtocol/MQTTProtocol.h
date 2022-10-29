#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

struct Data{
    float x, y, z;
};

class MQTT{
  public:
    MQTT();
    bool send(Data,Data,float);
    bool send(int);

  private:
    const char* server_url = "192.168.1.1";
    WiFiClient espClient;
    PubSubClient client;
    int port = 1883;
};