#include "timer.h" 
#include "usart.h"

//arr：自动重装值。
//psc：时钟预分频数		 
//void TIM4_Int_Init(uint16_t arr,uint16_t psc)
//{	
//		NVIC_InitTypeDef NVIC_InitStructure;
//		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);													//TIM4时钟使能    
//		
//		//定时器TIM4初始化
//		TIM_TimeBaseStructure.TIM_Period = arr; 																			//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
//		TIM_TimeBaseStructure.TIM_Prescaler = psc; 																		//设置用来作为TIMx时钟频率除数的预分频值
//		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 											//设置时钟分割:TDTS = Tck_tim
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 								  //TIM向上计数模式
//		
//		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
//	 
//		TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); 																			//使能指定的TIM4中断,允许更新中断
//		
//		TIM_Cmd(TIM4,ENABLE);																													//开启定时器4
//		
//		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;											//抢占优先级0
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;														//子优先级2
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;																//IRQ通道使能
//		
//		NVIC_Init(&NVIC_InitStructure);	
//}

//定时器4中断服务程序		    
//void TIM4_IRQHandler(void)
//{ 	
//		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)														//是更新中断
//		{	 			   
//				USART3_RX_STA |= 1<<15;																											//标记接收完成
//				TIM_ClearITPendingBit(TIM4, TIM_IT_Update );  														//清除TIM4更新中断标志    
//				TIM_Cmd(TIM4, DISABLE);  																									//关闭TIM4 
//		}	    
//}












