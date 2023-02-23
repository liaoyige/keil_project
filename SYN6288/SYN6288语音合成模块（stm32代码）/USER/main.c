#include "sys.h"
#include "delay.h"
#include "stdio.h"
#include "usart.h"
#include "syn6288.h"

int main(void)
{
		delay_init();
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	
		USART3_Init(9600);
		while(1)
		{
				SYN_FrameInfo(0, "[v9][m0][t5]主人,欢迎回来");
			
				for(int i = 0;i <3; i++)
				{
						delay_ms(1500);
				}
		}
}
