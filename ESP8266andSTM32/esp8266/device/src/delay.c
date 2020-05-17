#include "delay.h"

static u8  fac_us=0;//us延时倍速乘数
static u16 fac_ms=0;//ms延时倍速乘数

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
	SysTick->LOAD=nus*fac_us; //时间加载  	(SysTick->LOAD为24bit) 重装载数值寄存器 
	SysTick->VAL=0x00;        //清空计数器  当前数值寄存器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; //开始倒数   控制及状态寄存器 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达  
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}

//注意MS的范围
//SysTick->LOAD为24位寄存器，所以最大延时为
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位国MHz,nm单位为ms
//对48M条件下,nms<=2796
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
} 
