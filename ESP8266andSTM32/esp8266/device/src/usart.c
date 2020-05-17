#include "usart.h"
 
//����ɾ�� -----
bool alarmStatus = false;//falseΪ�� true Ϊ��

bool isClose  = false;
bool isOpen    = false;
//------����ɾ�� 

char ReceivedBuffer[ReceivedBufferMaxLength] = {0};
int  ReceivedBufferCurrentLen = 0;

int GetKey(void);
int SendChar(int ch);

#pragma import(__use_no_semihosting)             
//��׼����Ҫ֧�ֵĺ���
struct __FILE 
{
	int handle;
};

FILE __stdout,__stdin;       
//����_sys_exit()�Ա��⹤���ڰ�����״̬
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc����
//�����Ҫ����MCU������ϣ��printf���ĸ����������ȷ�� __WAIT_TODO__
int fputc(int ch, FILE *f)
{
	//ע�⣺USART_FLAG_TXE�Ǽ�鷢�ͻ������Ƿ�Ϊ�գ����Ҫ�ڷ���ǰ��飬������������߷���Ч�ʣ����������ߵ�ʱ����ܵ������һ���ַ���ʧ
	//USART_FLAG_TC�Ǽ�鷢����ɱ�־������ڷ��ͺ��飬����������˯�߶�ʧ�ַ����⣬����Ч�ʵͣ����͹����з��ͻ������Ѿ�Ϊ���ˣ����Խ�����һ�������ˣ�������ΪҪ�ȴ�������ɣ�����Ч�ʵͣ�
	//��Ҫ����һ���ã�һ����Ч�����
	
	//ѭ���ȴ�ֱ�����ͻ�����Ϊ��(TX Empty)��ʱ���Է������ݵ�������
//  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
//  {}
//	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
//  {}
//	 
//	USART_SendData(USART1, (uint8_t) ch);

//  /* ѭ���ȴ�ֱ�����ͽ���*/
//  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}

//	return ch;
	
	 return (SendChar(ch));
}

/*�ض���Scanf����*/
int fgetc(FILE *f)
{
    return (SendChar(GetKey()));
    /*����scanf()�ڴ�������������ʱ�������Կո�����������޷���ɷ���*/
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

void uart2_Init(u32 bound)//����2  ����ΪPA2  PA3
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //ʹ��ָ���˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��GPIO
	
	//Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;      
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         
  NVIC_Init(&NVIC_InitStructure); 

	
	//USART2����
	USART_InitStructure.USART_BaudRate = bound;	//���ô��ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//�ֳ�Ϊ8
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;	//����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //����USART����
	
	USART_ITConfig(USART2, USART_IT_RXNE|USART_IT_IDLE, ENABLE);//�����˽����жϺ����߿����ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART2, ENABLE);          //ʹ��USART
	
}
void USART2_IRQHandler(void)
{   
    u8 ucCh;

	  //�����ռĴ�����Ϊ��
	  //Receive Data register not empty interrupt
    if(USART_GetITStatus( USART2, USART_IT_RXNE ) != RESET )
    {
			  USART_ClearITPendingBit(USART2, USART_IT_RXNE); //ֻUSART_ReceiveDataҲ����
			  //��ȡusart2���յ������� ��usart2�����յ�����������esp8266
        ucCh  = USART_ReceiveData( USART2 );
			  // ��usart2���������� ���͸�USart1 usart1 ���Խ����ݴ��ڷ��͵����ڵ��Թ���
			  USART_SendData(USART1,ucCh);
			  
			  /**
			    ������Ҫ����������� ��λ�ȡ��esp8266 ���ĵ�������Ļظ�
			    ��֪�� 
			        1. ֻҪ����Ϣ���͵�esp8266 ,���ͻὫ���ݷ��͸�����2
			        2. ֻҪ����2�յ���Ϣ�����ͻὫ���ݷ��͸�����1
			        3. ֮��Ὣesp8266���ص����ݴ洢��ESP8266_Fram_Record_Struct .Data_RX_BUF��
			    Ŀ�꣺ 
			        ����esp8266���ص����� ��topic xxxxx/test���ⷢ������Ϣ ��ȡ���Կ��صĲ���
			        topic��ʽ ��
			            +MQTTSUBRECV:0,"/a1V0WObZHjP/STM32andESP8266/user/test",16,{"value":"open"}
                  +MQTTSUBRECV:0,"/a1V0WObZHjP/STM32andESP8266/user/test",17,{"value":"close"}
              ��֪�յ�esp8266�����ݺ��ԭ�ⲻ���Ĵ浽 ESP8266_Fram_Record_Struct .Data_RX_BUF			
			        ����ֻҪʹ��strstr()�����жϸ��ַ��������Ƿ��������������Ϳ����ж��Ƿ���xxx/set���ⷢ������Ϣ
			        �����xx/test��취��ȡ��
			    */
			 
        if(ESP8266_Fram_Record_Struct.InfBit.FramLength < ( RX_BUF_MAX_LEN - 1 ) ) 
        {
					  //usart2 ���յ������ݻᱻ�洢����
            ESP8266_Fram_Record_Struct .Data_RX_BUF[ ESP8266_Fram_Record_Struct.InfBit .FramLength ++ ]  = ucCh;   
				   
				}                      
    } 
		 
 
		//Idle line detection interrupt
		if( USART_GetITStatus( USART2, USART_IT_IDLE ) != RESET )                                         //������߿���
    {
        ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag = 1;

        ucCh = USART_ReceiveData( USART2 );                                                              //�������������жϱ�־λ���ȶ�USART_SR,Ȼ���USART_DR��
				USART_SendData(USART1,ucCh);
        if(ESP8266_Fram_Record_Struct .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) ) 
        {
					  //usart2 ���յ������ݻᱻ�洢����
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
					 ESP8266_Fram_Record_Struct.InfBit .FramLength = 0; //���½����µ����ݰ�
					 alarmStatus = 	true;			
           strcpy(ESP8266_Fram_Record_Struct.Data_RX_BUF," ");
           printf("\n ����״̬: %d \n",alarmStatus);					
				}
				if(isClose)
				{
					
					 ESP8266_Fram_Record_Struct.InfBit .FramLength = 0; //���½����µ����ݰ�
					 alarmStatus = 	false;		
           strcpy(ESP8266_Fram_Record_Struct.Data_RX_BUF," ");
           printf("\n ����״̬: %d \n",alarmStatus);							
				}	
		}

}


void uart3_Init(u32 bound)//����3  ����ΪPB10  PB11 PA9 PA10
{

    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PB10 PA9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //?�䨮?��?������?3?
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PB11 PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO

    //USART1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;      //��Ӧ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //USART_IRQnͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure); //��ʼ��NVIC

    //USART1 ����
    USART_InitStructure.USART_BaudRate = bound;//������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//���ݳ���
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//ֹͣλ1
    USART_InitStructure.USART_Parity = USART_Parity_No;//У��λ��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ����������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //ʹ�ܴ��ڵĽ��պͷ��͹���
    USART_Init(USART1, &USART_InitStructure); //��ʼ������

    USART_ITConfig(USART1, USART_IT_RXNE|USART_IT_IDLE, ENABLE);//�����˽����жϺ����߿����ж�

    USART_Cmd(USART1, ENABLE);      //��������ʹ��    
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
//            //�����һλ������λ
//             ESP8266_Fram_Record_Struct .Data_RX_BUF[ ESP8266_Fram_Record_Struct .InfBit .FramLength ++ ]  = ucCh;   
//						
//				}                      
//    }

//    if( USART_GetITStatus( USART1, USART_IT_IDLE ) == SET )                              //������߿���
//    {
//        ESP8266_Fram_Record_Struct .InfBit .FramFinishFlag = 1;
//        ucCh = USART_ReceiveData( USART1 );                                                              //�������������жϱ�־λ���ȶ�USART_SR,Ȼ���USART_DR��
//        TcpClosedFlag = strstr ( ESP8266_Fram_Record_Struct .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;

//    }   

}
