#include <MQTTProtocol.h>

MQTT::MQTT(){
    client.setClient(espClient);
    client.setServer(server_url, port);

}

bool MQTT::send(Data Acc,Data Gyro,float temp){
    String value = String(Acc.x)+","+String(Acc.y)+","+String(Acc.z)+","+String(Gyro.x)+","+String(Gyro.y)+","+String(Gyro.z)+","+String(temp);
    client.publish("esp/channel1",value.c_str());
}
bool MQTT::send(int errorCode){
    String value=String(errorCode);
    client.publish("esp/channel2",value.c_str());
}