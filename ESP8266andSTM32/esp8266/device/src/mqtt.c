#include "mqtt.h"
#include "esp8266.h"
#include "delay.h"

void ESP8266_STA_MQTTClient_Test(void)
{
	
	
	  //char str[1024]; // 太大，分配不了
	  char str[100]={0};
	
		printf("正在配置ESP8266参数\r\n");
    ESP8266_AT_Test();//恢复出厂默认模式
	 
		 
  	printf("恢复出厂默认模式成功\r\n");
    ESP8266_Net_Mode_Choose(STA);
	 
		printf("开始连接wifi\r\n");
    while(!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD));
	  
		printf("开始配置  \r\n");
		//用户连接配置
		ESP8266_MQTTUSERCFG(User_ESP8266_client_id,User_ESP8266_username,User_ESP8266_password);
	  printf("连接配置完成 \r\n");
		delay_ms(1000);
		
		//网络会话时间配置
		ESP8266_MQTTCONNCFG(0,User_ESP8266_MQTTSession_Time,0,"","",0,0);
		printf("网络会话时间配置完成 \r\n");
		delay_ms(1000);
		
		//连接服务器
		ESP8266_MQTTCONN( User_ESP8266_MQTTServer_IP, User_ESP8266_MQTTServer_PORT);
	  printf("连接服务器成功 \r\n");
		delay_ms(1000);
		
		//订阅主题
		ESP8266_MQTTSUB( User_ESP8266_MQTTServer_Topic);
		delay_ms(1000);
		
		printf("\r\nMQTT配置完成");
		while(1)
		{   
			  delay_ms(10000);
			  sprintf(str,"aithinker");//格式化发送字符串到MQTT服务器
			  MQTT_SendString (User_ESP8266_MQTTServer_Topic,str);//发送数据到MQTT服务器
			  printf("\r\n发送一个数据包");
		}
}
