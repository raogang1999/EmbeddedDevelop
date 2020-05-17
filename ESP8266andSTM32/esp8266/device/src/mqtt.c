#include "mqtt.h"
#include "esp8266.h"
#include "delay.h"

void ESP8266_STA_MQTTClient_Test(void)
{
	
	
	  //char str[1024]; // ̫�󣬷��䲻��
	  char str[100]={0};
	
		printf("��������ESP8266����\r\n");
    ESP8266_AT_Test();//�ָ�����Ĭ��ģʽ
	 
		 
  	printf("�ָ�����Ĭ��ģʽ�ɹ�\r\n");
    ESP8266_Net_Mode_Choose(STA);
	 
		printf("��ʼ����wifi\r\n");
    while(!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD));
	  
		printf("��ʼ����  \r\n");
		//�û���������
		ESP8266_MQTTUSERCFG(User_ESP8266_client_id,User_ESP8266_username,User_ESP8266_password);
	  printf("����������� \r\n");
		delay_ms(1000);
		
		//����Ựʱ������
		ESP8266_MQTTCONNCFG(0,User_ESP8266_MQTTSession_Time,0,"","",0,0);
		printf("����Ựʱ��������� \r\n");
		delay_ms(1000);
		
		//���ӷ�����
		ESP8266_MQTTCONN( User_ESP8266_MQTTServer_IP, User_ESP8266_MQTTServer_PORT);
	  printf("���ӷ������ɹ� \r\n");
		delay_ms(1000);
		
		//��������
		ESP8266_MQTTSUB( User_ESP8266_MQTTServer_Topic);
		delay_ms(1000);
		
		printf("\r\nMQTT�������");
		while(1)
		{   
			  delay_ms(10000);
			  sprintf(str,"aithinker");//��ʽ�������ַ�����MQTT������
			  MQTT_SendString (User_ESP8266_MQTTServer_Topic,str);//�������ݵ�MQTT������
			  printf("\r\n����һ�����ݰ�");
		}
}
