## 项目简介
使用ESP8266 STM32 连接阿里云

## 博客地址
[csdn](https://blog.csdn.net/u013671216/article/details/106171247)

## 使用说明
+ 使用的是STM32F103C8T6
+ 使用了USART2 给 ESP8266 发送 AT指令，USART2收到的信息会发给USART1方便查看
+ ESP8266 发送给STM32的信息可以通过 device/src/usart.c 的 USART2_IRQHandler(void) 的 if( USART_GetITStatus( USART2, USART_IT_IDLE ) != RESET )来处理收到的消息
+ 需要修改三元组和wifi信息，在mqtt.h中
+ 连接操作在mqtt.c中，主题定阅可在这里进行
