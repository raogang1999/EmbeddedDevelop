#include "usart.h"
 
//可以删除 -----
bool alarmStatus = false;//false为关 true 为开

bool isClose  = false;
bool isOpen    = false;
//------可以删除 

char ReceivedBuffer[ReceivedBufferMaxLength] = {0};
int  ReceivedBufferCurrentLen = 0;

int GetKey(void);
int SendChar(int ch);

#pragma import(__use_no_semihosting)             
//标准库需要支持的函数
struct __FILE 
{
	int handle;
};

FILE __stdout,__stdin;       
//定义_sys_exit()以避免工作在半主机状态
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数
//这个需要根据MCU和我们希望printf从哪个串口输出来确认 __WAIT_TODO__
int fputc(int ch, FILE *f)
{
	//注意：USART_FLAG_TXE是检查发送缓冲区是否为空，这个要在发送前检查，检查这个提议提高发送效率，但是在休眠的时候可能导致最后一个字符丢失
	//USART_FLAG_TC是检查发送完成标志，这个在发送后检查，这个不会出现睡眠丢失字符问题，但是效率低（发送过程中发送缓冲区已经为空了，可以接收下一个数据了，但是因为要等待发送完成，所以效率低）
	//不要两个一起用，一起用效率最低
	
	//循环等待直到发送缓冲区为空(TX Empty)此时可以发送数据到缓冲区
//  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
//  {}
//	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
//  {}
//	 
//	USART_SendData(USART1, (uint8_t) ch);

//  /* 循环等待直到发送结束*/
//  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}

//	return ch;
	
	 return (SendChar(ch));
}

/*重定向Scanf函数*/
int fgetc(FILE *f)
{
    return (SendChar(GetKey()));
    /*调用scanf()在串口中输入数据时，必须以空格结束，否则无法完成发送*/
}

int SendChar(int ch)
{
    while(!(USART1->SR & USART_FLAG_TXE));
    USART1->DR = (ch & 0x1FF);

    return ch;
}

int GetKey(void)
{
    while(!(USART1->SR & USART_FLAG_RXNE));
    return ((int)(USART1->DR & 0X1FF));
}

void uart2_Init(u32 bound)//串口2  引脚为PA2  PA3
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //使能指定端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化GPIO
	
	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;      
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         
  NVIC_Init(&NVIC_InitStructure); 

	
	//USART2配置
	USART_InitStructure.USART_BaudRate = bound;	//设置串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//字长为8
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;	//无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //配置USART参数
	
	USART_ITConfig(USART2, USART_IT_RXNE|USART_IT_IDLE, ENABLE);//配置了接收中断和总线空闲中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART2, ENABLE);          //使能USART
	
}
void USART2_IRQHandler(void)
{   
    u8 ucCh;

	  //当接收寄存器不为空
	  //Receive Data register not empty interrupt
    if(USART_GetITStatus( USART2, USART_IT_RXNE ) != RESET )
    {
			  USART_ClearITPendingBit(USART2, USART_IT_RXNE); //只USART_ReceiveData也可以
			  //读取usart2接收到的数据 ，usart2所接收到的数据来自esp8266
        ucCh  = USART_ReceiveData( USART2 );
			  // 将usart2读到的数据 发送给USart1 usart1 可以将数据串口发送到串口调试工具
			  USART_SendData(USART1,ucCh);
			  
			  /**
			    这里需要解决的问题是 如何获取到esp8266 订阅到的主题的回复
			    已知： 
			        1. 只要有信息推送到esp8266 ,它就会将数据发送给串口2
			        2. 只要串口2收到消息，他就会将数据发送给串口1
			        3. 之后会将esp8266返回的数据存储到ESP8266_Fram_Record_Struct .Data_RX_BUF中
			    目标： 
			        分析esp8266返回的数据 即topic xxxxx/test主题发来的消息 获取它对开关的操作
			        topic格式 ：
			            +MQTTSUBRECV:0,"/a1V0WObZHjP/STM32andESP8266/user/test",16,{"value":"open"}
                  +MQTTSUBRECV:0,"/a1V0WObZHjP/STM32andESP8266/user/test",17,{"value":"close"}
              已知收到esp8266的数据后会原封不动的存到 ESP8266_Fram_Record_Struct .Data_RX_BUF			
			        所以只要使用strstr()函数判断该字符数组中是否包含这样的主题就可以判断是否是xxx/set主题发来的消息
			        如果是xx/test想办法获取到
			    */
			 
        if(ESP8266_Fram_Record_Struct.InfBit.FramLength < ( RX_BUF_MAX_LEN - 1 ) ) 
        {
					  //usart2 接收到的数据会被存储起来
            ESP8266_Fram_Record_Struct .Data_RX_BUF[ ESP8266_Fram_Record_Struct.InfBit .FramLength ++ ]  = ucCh;   
				   
				}                      
    } 
		 
 
		//Idle line detection interrupt
		if( USART_GetITStatus( USART2, USART_IT_IDLE ) != RESET )                                         //如果总线空闲
    {
        ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag = 1;

        ucCh = USART_ReceiveData( USART2 );                                                              //由软件序列清除中断标志位（先读USART_SR,然后读USART_DR）
				USART_SendData(USART1,ucCh);
        if(ESP8266_Fram_Record_Struct .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) ) 
        {
					  //usart2 接收到的数据会被存储起来
            ESP8266_Fram_Record_Struct.Data_RX_BUF[ ESP8266_Fram_Record_Struct .InfBit .FramLength ++ ]  = ucCh;   
				   
				} 
				TcpClosedFlag = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;
				
				
			  ESP8266_Fram_Record_Struct.Data_RX_BUF[ ESP8266_Fram_Record_Struct.InfBit.FramLength] = '\0';
				
				printf("received a package \n");
			  printf("%s \n",ESP8266_Fram_Record_Struct.Data_RX_BUF);
			  
				isOpen  = ( bool ) strstr (ESP8266_Fram_Record_Struct.Data_RX_BUF,"alarmSwithIsOpen");
			  isClose = ( bool ) strstr (ESP8266_Fram_Record_Struct.Data_RX_BUF,"alarmSwithIsClose");  
				
			  if(isOpen)
				{
					 ESP8266_Fram_Record_Struct.InfBit .FramLength = 0; //重新接收新的数据包
					 alarmStatus = 	true;			
           strcpy(ESP8266_Fram_Record_Struct.Data_RX_BUF," ");
           printf("\n 开关状态: %d \n",alarmStatus);					
				}
				if(isClose)
				{
					
					 ESP8266_Fram_Record_Struct.InfBit .FramLength = 0; //重新接收新的数据包
					 alarmStatus = 	false;		
           strcpy(ESP8266_Fram_Record_Struct.Data_RX_BUF," ");
           printf("\n 开关状态: %d \n",alarmStatus);							
				}	
		}

}


void uart3_Init(u32 bound)//串口3  引脚为PB10  PB11 PA9 PA10
{

    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PB10 PA9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //?′ó?í?íìê?3?
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PB11 PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO

    //USART1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;      //响应优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //USART_IRQn通道使能
    NVIC_Init(&NVIC_InitStructure); //初始化NVIC

    //USART1 配置
    USART_InitStructure.USART_BaudRate = bound;//波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//数据长度
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位1
    USART_InitStructure.USART_Parity = USART_Parity_No;//校验位无
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制无
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //使能串口的接收和发送功能
    USART_Init(USART1, &USART_InitStructure); //初始化串口

    USART_ITConfig(USART1, USART_IT_RXNE|USART_IT_IDLE, ENABLE);//配置了接收中断和总线空闲中断

    USART_Cmd(USART1, ENABLE);      //串口外设使能    
}

void USART1_IRQHandler( void )
{   
 
//    u8 ucCh;

//    if(USART_GetITStatus( USART1, USART_IT_RXNE ) != RESET )
//    {
//        ucCh  = USART_ReceiveData( USART1 );
//			

//        if(ESP8266_Fram_Record_Struct .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) ) 
//        {
//            //留最后一位做结束位
//             ESP8266_Fram_Record_Struct .Data_RX_BUF[ ESP8266_Fram_Record_Struct .InfBit .FramLength ++ ]  = ucCh;   
//						
//				}                      
//    }

//    if( USART_GetITStatus( USART1, USART_IT_IDLE ) == SET )                              //如果总线空闲
//    {
//        ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag = 1;
//        ucCh = USART_ReceiveData( USART1 );                                                              //由软件序列清除中断标志位（先读USART_SR,然后读USART_DR）
//        TcpClosedFlag = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;

//    }   

}
