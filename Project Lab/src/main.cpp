#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>

#include "MotionSensor_MPU6.h"  
#include "TemperatureSensor.h"
#include "HTTPProtocol.h"



#define THRESHOLD_MEASUREMENTS 30
#define THRESHOLD_DATA1 50
#define THRESHOLD_DATA2 50
#define TIME_TO_SLEEP 10
#define uS_TO_S_FACTOR 1000000

#define TEMPERATURE_SENSOR_PIN 25


RTC_DATA_ATTR int count_measurements=0;
RTC_DATA_ATTR int data1_median,data2_median;
RTC_DATA_ATTR String ssid;
RTC_DATA_ATTR String pass;
const char* ap_ssid = "ESP32-AP-Mode";
const char* ap_password = "";
Data Acc,Gyro;
float temp;

WebServer my_server(80);
MotionSensor mpu;
TemperatureSensor ts(TEMPERATURE_SENSOR_PIN);
HTTP client;


// Function declarations
void set_ap_mode();
void send_data();
void get_data();
void get_wifi_credentials();
void set_wifi_credentials();
bool connect_wifi();
void set_home_page();
void hard_reset();


// Errors handlers declarations
void server_error();
void wifi_error();
void sensor_error();


void setup(){
    EEPROM.begin(100);
    bool wifi_not_setted = EEPROM.read(99);
    if (wifi_not_setted==0){
        set_ap_mode();
        ESP.restart();
    }
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch(wakeup_reason)
    {
      case ESP_SLEEP_WAKEUP_EXT0 : 
        hard_reset();
        break;
      case ESP_SLEEP_WAKEUP_TIMER : 
        if (count_measurements>THRESHOLD_MEASUREMENTS){
          send_data();
          count_measurements = 0;
        }
        else{
          get_data();
        }
        esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
        delay(1000);
        break;
    }

    esp_deep_sleep_start();

}

void loop(){

}


// Function definitions
void set_home_page(){
  my_server.on("/", []() {
  IPAddress ip = WiFi.softAPIP();
  String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
  String content = "<!DOCTYPE HTML>\r\n<html>Welcome to Wifi Credentials Update page";
  content += ipStr;
  content += "<form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
  content += "</html>";
  my_server.send(200, "text/html", content);
});
}

bool connect_wifi(){
  WiFi.begin(ssid.c_str(), pass.c_str());
  int c = 0;
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(500);
    c++;
  }
  wifi_error();
  return false;
}



void set_ap_mode(){
    WiFi.softAP(ap_ssid,ap_password,13);
    set_wifi_credentials();
    set_home_page();
    my_server.begin();
    while ((WiFi.status() != WL_CONNECTED)){
      my_server.handleClient();
    }
    WiFi.softAPdisconnect();
}

void send_data(){
  if(connect_wifi){
    if(client.send(Acc, Gyro, temp)){
      WiFi.disconnect();
    }
    else{
      server_error();
    }
  }
}

void get_data(){
  if(mpu.update()){
    Acc = mpu.getAcc();
    Gyro = mpu.getGyro();
  }
  else{
    sensor_error();
  }
  temp = ts.getTemperature();
}

void get_wifi_credentials(){
  for (int i = 0; i < 32; ++i)
  {
    ssid += char(EEPROM.read(i));
  }
  for (int i = 32; i < 96; ++i)
  {
    pass += char(EEPROM.read(i));
  }
}

void set_wifi_credentials(){
    my_server.on("/setting", []() {
    String content;
    int statusCode;
    String qsid = my_server.arg("ssid");
    String qpass = my_server.arg("pass");
    if (qsid.length() > 0 && qpass.length() > 0) {
      for (int i = 0; i < 96; ++i){
          EEPROM.write(i, 0);
      }
      for (int i = 0; i < qsid.length(); ++i){
          EEPROM.write(i, qsid[i]);
      }
      for (int i = 0; i < qpass.length(); ++i)
      {
          EEPROM.write(32 + i, qpass[i]);
      }
      EEPROM.commit();
      get_wifi_credentials();
      if (connect_wifi){
        content = "{\"Success\":\"saved to eeprom....connected to wifi\"}";
        statusCode = 200;
      }
      else{
        content = "{\"Success\":\"unable to connect to wifi\"}";
        statusCode = 404;
      }
    }
    else {
      content = "{\"Error\":\"404 not found\"}";
      statusCode = 404;
    }
    my_server.sendHeader("Access-Control-Allow-Origin", "*");
    my_server.send(statusCode, "application/json", content);
    });
}

void hard_reset(){
  for (int i = 0; i < 100; ++i) {
    EEPROM.write(i, 0);
  }
}


// Error handlers definitions
void server_error(){

}

void wifi_error(){

}

void sensor_error(){

}




