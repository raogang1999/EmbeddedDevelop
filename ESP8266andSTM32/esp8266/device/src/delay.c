#include "delay.h"

static u8  fac_us=0;//us��ʱ���ٳ���
static u16 fac_ms=0;//ms��ʱ���ٳ���

void delay_init(void)	 
{

	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//72000 0000/8=9000 000  HCLK/8
	SysTick->CTRL = 0xfffffffb;
	fac_us=SystemCoreClock/8000000;	//fac_us=72000 000/8000 000=9
	fac_ms=(u16)fac_us*1000;//fac_ms=9*1000=9000

}								    

		   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //ʱ�����  	(SysTick->LOADΪ24bit) ��װ����ֵ�Ĵ��� 
	SysTick->VAL=0x00;        //��ռ�����  ��ǰ��ֵ�Ĵ���
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; //��ʼ����   ���Ƽ�״̬�Ĵ��� 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��  
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	 
}

//ע��MS�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ��������������ʱΪ
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λ��MHz,nm��λΪms
//��48M������,nms<=2796
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ���� 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	  	    
} 
