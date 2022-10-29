#include <HTTPProtocol.h>


HTTP::HTTP(){
}

bool HTTP::send(Data Acc, Data Gyro, float temp){
  HTTPClient http;
  http.begin(server_url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String data="AccX="+String(Acc.x)+"&AccY="+String(Acc.y)+"&AccZ="+String(Acc.z)
  +"&GyroX"+String(Gyro.x)+"&GyroY"+String(Gyro.y)+"&GyroZ"+String(Gyro.z)
  +"&temp"+String(temp);

  int responseCode = http.POST(data);
  if (responseCode > 0) {
      return true;
    }
  else {
    return false;
    }
  http.end();
}

bool HTTP::send(int errorCode){
  HTTPClient http;
  http.begin(server_url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String data="ErrorCode="+String(errorCode);
  int responseCode = http.POST(data);
  if (responseCode > 0) {
      return true;
    }
  else {
    return false;
    }
  http.end();
}
