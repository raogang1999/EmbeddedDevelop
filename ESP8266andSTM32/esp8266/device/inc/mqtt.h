#ifndef __MQTT_H
#define __MQTT_H 			   
#include "stm32f10x.h"


/*
*以下参数需要用户自行修改才能测试用过
*/


#define User_ESP8266_SSID     "FAST"    //wifi名
#define User_ESP8266_PWD      "1xxxxx"      //wifi密码
    

// mqtt 服务器相关

#define User_ESP8266_MQTTSession_Time  120
#define User_ESP8266_client_id    "xxxxxx|securemode=3\\,signmethod=hmacsha1|"   //MQTTclientID 用于标志client身份  最长256字节
#define User_ESP8266_username     "xxxxxxx"						//用于登录 MQTT 服务器 的 username, 最长 64 字节	
#define User_ESP8266_password			"xxxxxxxx"          //用于登录 MQTT 服务器 的 password, 最长 64 字节
#define User_ESP8266_MQTTServer_IP     "xxxxxx.iot-as-mqtt.cn-shanghai.aliyuncs.com"     //MQTT本地服务器IP
#define User_ESP8266_MQTTServer_PORT   1883     //服务器端口号
#define User_ESP8266_MQTTServer_Topic  "/xxxxxx/xxxxx/user/update"    //订阅MQTT主题

void ESP8266_STA_MQTTClient_Test(void);

#endif
