#include "Reg_RW.h"
#include "LDChip.h"
#include "delay.h"

void LD3320_EXTIX_Init(void)
{
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
		GPIO_EXTILineConfig(LD3320_IRQEXIT_PORTSOURCE,LD3320_IRQPINSOURCE);

		EXTI_InitStructure.EXTI_Line=LD3320_IRQEXITLINE;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

		NVIC_InitStructure.NVIC_IRQChannel = LD3320_IRQN;			
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
		NVIC_Init(&NVIC_InitStructure);
}

//软件模拟SPI方式读写
void LD3320_GPIO_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd(LD3320_SDCK_GPIO_CLK|LD3320_SDO_GPIO_CLK|LD3320_SDI_GPIO_CLK|\
		LD3320_SCS_GPIO_CLK|LD3320_RSTB_GPIO_CLK|LD3320_IRQ_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);	 
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//要先开时钟，再重映射；关闭jtag，保留swd。
		
		GPIO_InitStructure.GPIO_Pin = LD3320_SDCK_PIN;				
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(LD3320_SDCK_GPIO_PORT, &GPIO_InitStructure);	 
		
		GPIO_InitStructure.GPIO_Pin = LD3320_SDI_PIN;				
		GPIO_Init(LD3320_SDI_GPIO_PORT, &GPIO_InitStructure);	  
		
		GPIO_InitStructure.GPIO_Pin = LD3320_SCS_PIN;				
		GPIO_Init(LD3320_SCS_GPIO_PORT, &GPIO_InitStructure);	  
		
		GPIO_InitStructure.GPIO_Pin = LD3320_RSTB_PIN;				
		GPIO_Init(LD3320_RSTB_GPIO_PORT, &GPIO_InitStructure);	  
		
		GPIO_InitStructure.GPIO_Pin = LD3320_SDO_PIN;				
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		  
		GPIO_Init(LD3320_SDO_GPIO_PORT, &GPIO_InitStructure);	  

		GPIO_InitStructure.GPIO_Pin = LD3320_IRQ_PIN;				
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		  
		GPIO_Init(LD3320_IRQ_GPIO_PORT, &GPIO_InitStructure);	  
}

void LD_WriteReg(unsigned char address,unsigned char dataout)
{
		unsigned char i = 0;
		unsigned char command=0x04;
		SCS = 0;
		delay_us(1);

		//write command
		for (i=0;i < 8; i++)
		{
				if (command & 0x80) 
					SDI = 1;
				else
					SDI = 0;
				
				delay_us(1);
				SDCK = 0;
				command = (command << 1);  
				delay_us(1);
				SDCK = 1;  
		}
		//write address
		for (i=0;i < 8; i++)
		{
				if (address & 0x80) 
					SDI = 1;
				else
					SDI = 0;
				delay_us(1);
				SDCK = 0;
				address = (address << 1); 
				delay_us(1);
				SDCK = 1;  
		}
		//write data
		for (i=0;i < 8; i++)
		{
				if (dataout & 0x80) 
					SDI = 1;
				else
					SDI = 0;
				delay_us(1);
				SDCK = 0;
				dataout = (dataout << 1); 
				delay_us(1);
				SDCK = 1;  
		}
		delay_us(1);
		SCS = 1;
}

unsigned char LD_ReadReg(unsigned char address)
{
		unsigned char i = 0; 
		unsigned char datain =0 ;
		unsigned char temp = 0; 
		unsigned char command=0x05;
		SCS = 0;
		delay_us(1);

		//write command
		for (i=0;i < 8; i++)
		{
			if (command & 0x80) 
				SDI = 1;
			else
				SDI = 0;
			delay_us(1);
			SDCK = 0;
			command = (command << 1);  
			delay_us(1);
			SDCK = 1;  
		}

		//write address
		for (i=0;i < 8; i++)
		{
			if (address & 0x80) 
				SDI = 1;
			else
				SDI = 0;
			delay_us(1);
			SDCK = 0;
			address = (address << 1); 
			delay_us(1);
			SDCK = 1;  
		}
		delay_us(1);

		//Read
		for (i=0;i < 8; i++)
		{
			datain = (datain << 1);
			temp = SDO;
			delay_us(1);
			SDCK = 0;  
			if (temp == 1)  
				datain |= 0x01; 
			delay_us(1);
			SDCK = 1;  
		}

		delay_us(1);
		SCS = 1;
		return datain;
}

void EXTI9_5_IRQHandler(void)
{
		if(EXTI_GetITStatus(LD3320_IRQEXITLINE)!= RESET ) 
		{
			ProcessInt(); 	
			EXTI_ClearFlag(LD3320_IRQEXITLINE);
			EXTI_ClearITPendingBit(LD3320_IRQEXITLINE);//清除LINE上的中断标志位  
		} 
}
