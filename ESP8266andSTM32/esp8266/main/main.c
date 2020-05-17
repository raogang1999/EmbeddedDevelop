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
 * 本工程只是STM32对接ESP8266的驱动demo，仅供参考，不保证商用稳定性,
 * 本工程参考该作者进行移植，连接为https://blog.csdn.net/it_boy__/article/details/71975797
 * 最终解释权归深圳市安信可科技有限公司所有。
 *
 * author     Specter
 */



int main(void){
	delay_init();
	uart3_Init(115200);
	ESP8266_Init(115200);
	while(1)
	{
		//ESP8266_STA_TCPClient_Test();//测试TCP通讯
		ESP8266_STA_MQTTClient_Test();//测试MQTT通讯
	}
}
