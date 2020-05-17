#ifndef __USART_H
#define __USART_H 			   

#include "stm32f10x.h"
#include <stdio.h>
#include "esp8266.h"
#include "tcp.h"
#include "mqtt.h"

#define  ReceivedBufferMaxLength 1024


void uart2_Init(u32 bound);
void uart3_Init(u32 bound);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);

#endif
