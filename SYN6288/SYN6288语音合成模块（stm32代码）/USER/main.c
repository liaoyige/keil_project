#include "sys.h"
#include "delay.h"
#include "stdio.h"
#include "usart.h"
#include "syn6288.h"

int main(void)
{
		delay_init();
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
		USART3_Init(9600);
		while(1)
		{
				SYN_FrameInfo(0, "[v9][m0][t5]����,��ӭ����");
			
				for(int i = 0;i <3; i++)
				{
						delay_ms(1500);
				}
		}
}
