#include "timer.h" 
#include "usart.h"

//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
//void TIM4_Int_Init(uint16_t arr,uint16_t psc)
//{	
//		NVIC_InitTypeDef NVIC_InitStructure;
//		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);													//TIM4ʱ��ʹ��    
//		
//		//��ʱ��TIM4��ʼ��
//		TIM_TimeBaseStructure.TIM_Period = arr; 																			//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
//		TIM_TimeBaseStructure.TIM_Prescaler = psc; 																		//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
//		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 											//����ʱ�ӷָ�:TDTS = Tck_tim
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 								  //TIM���ϼ���ģʽ
//		
//		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
//	 
//		TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); 																			//ʹ��ָ����TIM4�ж�,��������ж�
//		
//		TIM_Cmd(TIM4,ENABLE);																													//������ʱ��4
//		
//		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;											//��ռ���ȼ�0
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;														//�����ȼ�2
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;																//IRQͨ��ʹ��
//		
//		NVIC_Init(&NVIC_InitStructure);	
//}

//��ʱ��4�жϷ������		    
//void TIM4_IRQHandler(void)
//{ 	
//		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)														//�Ǹ����ж�
//		{	 			   
//				USART3_RX_STA |= 1<<15;																											//��ǽ������
//				TIM_ClearITPendingBit(TIM4, TIM_IT_Update );  														//���TIM4�����жϱ�־    
//				TIM_Cmd(TIM4, DISABLE);  																									//�ر�TIM4 
//		}	    
//}












