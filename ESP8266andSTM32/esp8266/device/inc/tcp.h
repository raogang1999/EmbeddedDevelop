#ifndef __TCP_H
#define __TCP_H 			   
#include "stm32f10x.h"


/*
*���²�����Ҫ�û������޸Ĳ��ܲ����ù�
*/

#define User_ESP8266_SSID     "FAST"          //wifi��
#define User_ESP8266_PWD      "12345678rg"      //wifi����

#define User_ESP8266_TCPServer_IP     "a1V0WObZHjP.iot-as-mqtt.cn-shanghai.aliyuncs.com"     //������IP
#define User_ESP8266_TCPServer_PORT   "1883"      //�������˿ں�


extern volatile uint8_t TcpClosedFlag;  //����״̬��־

void ESP8266_STA_TCPClient_Test(void);

#endif
