#ifndef __MQTT_H
#define __MQTT_H 			   
#include "stm32f10x.h"


/*
*���²�����Ҫ�û������޸Ĳ��ܲ����ù�
*/


#define User_ESP8266_SSID     "FAST"    //wifi��
#define User_ESP8266_PWD      "1xxxxx"      //wifi����
    

// mqtt ���������

#define User_ESP8266_MQTTSession_Time  120
#define User_ESP8266_client_id    "xxxxxx|securemode=3\\,signmethod=hmacsha1|"   //MQTTclientID ���ڱ�־client���  �256�ֽ�
#define User_ESP8266_username     "xxxxxxx"						//���ڵ�¼ MQTT ������ �� username, � 64 �ֽ�	
#define User_ESP8266_password			"xxxxxxxx"          //���ڵ�¼ MQTT ������ �� password, � 64 �ֽ�
#define User_ESP8266_MQTTServer_IP     "xxxxxx.iot-as-mqtt.cn-shanghai.aliyuncs.com"     //MQTT���ط�����IP
#define User_ESP8266_MQTTServer_PORT   1883     //�������˿ں�
#define User_ESP8266_MQTTServer_Topic  "/xxxxxx/xxxxx/user/update"    //����MQTT����

void ESP8266_STA_MQTTClient_Test(void);

#endif
