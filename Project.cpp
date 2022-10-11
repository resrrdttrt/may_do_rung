#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <EEPROM.h>


#define THRESHOLD_MEASUREMENTS 30
#define THRESHOLD_DATA1 50
#define THRESHOLD_DATA2 50
#define TIME_TO_SLEEP 10
#define uS_TO_S_FACTOR 1000000



RTC_DATA_ATTR int count_measurements=0;
RTC_DATA_ATTR int data1_median,data2_median;
RTC_DATA_ATTR String ssid;
RTC_DATA_ATTR String pass;
const char* ap_ssid = "ESP32-AP-Mode";
const char* ap_password = "";
String server_url="http://";
WebServer my_server(80);


// Function declarations
void set_ap_mode();
void send_data();
void get_data();
void get_wifi_credentials();
void set_wifi_credentials();
bool connect_wifi();
void httpGETRequest(String);
void set_home_page();
void hard_reset();


// Errors handlers declarations
void server_error();
void wifi_error();


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

void httpGETRequest(String serverName){
  HTTPClient http;
  http.begin(serverName);
  int responseCode = http.GET();
  if (responseCode > 0) {
      count_measurements = 0;
    }
  else {
    server_error();
    }
  http.end();
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
    String data_url="data1="+String(data1_median)+"&data2="+String(data2_median);
    httpGETRequest(server_url+data_url);
    WiFi.disconnect();
  }
}

void get_data(){
    count_measurements+=1;
    int data1=random(100);
    int data2=random(100);
    if (data1>THRESHOLD_DATA1 || data2>THRESHOLD_DATA2){
      if (connect_wifi){
        String alert_url = "/alert";
        httpGETRequest(server_url+alert_url);
        WiFi.disconnect();
      }
    }
    else{
      (data1_median*(count_measurements-1)+data1)/count_measurements;
      (data2_median*(count_measurements-1)+data2)/count_measurements;
    }
    
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


