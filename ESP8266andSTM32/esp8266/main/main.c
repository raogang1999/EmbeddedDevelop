#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x_it.h"
#include "tcp.h"
#include "esp8266.h"
#include "mqtt.h"


/*
 * Copyright (c) 2020-2021 AIThinker.yangbin All rights reserved.
 *
 * ������ֻ��STM32�Խ�ESP8266������demo�������ο�������֤�����ȶ���,
 * �����̲ο������߽�����ֲ������Ϊhttps://blog.csdn.net/it_boy__/article/details/71975797
 * ���ս���Ȩ�������а��ſɿƼ����޹�˾���С�
 *
 * author     Specter
 */



int main(void){
	delay_init();
	uart3_Init(115200);
	ESP8266_Init(115200);
	while(1)
	{
		//ESP8266_STA_TCPClient_Test();//����TCPͨѶ
		ESP8266_STA_MQTTClient_Test();//����MQTTͨѶ
	}
}
