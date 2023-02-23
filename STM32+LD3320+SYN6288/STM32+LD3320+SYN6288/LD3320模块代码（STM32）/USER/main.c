#include "stm32f10x.h"
#include "delay.h"
#include "Reg_RW.h"
#include "LDChip.h"
#include "usart.h"
#include "syn6288.h"
void User_Modification(uint8_t dat);
uint8_t ASR_Status=0;
uint8_t nASR_Res=0;
uint8_t flag=0;

int main(void)
{	
		delay_init();	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	
		USART3_Init(9600);
	
		LD3320_GPIO_Init();	
		LD3320_EXTIX_Init();	
		LD3320_Reset();	
		ASR_Status = LD_ASR_NONE;																						//	初始状态：没有在作ASR
		while(1)
		{
				switch(ASR_Status)
				{
						case LD_ASR_RUNING:							
						case LD_ASR_ERROR:							
								 break;						
						case LD_ASR_NONE:
						{
								ASR_Status=LD_ASR_RUNING;
							
								if (RunASR()==0)																				//启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算
								{
									ASR_Status = LD_ASR_ERROR;
								}
								break;
						}
						case LD_ASR_FOUNDOK: 																				//	一次ASR识别流程结束，去取ASR识别结果
						{
								nASR_Res = LD3320_GetResult();															//获取结果												
								User_Modification(nASR_Res);
								ASR_Status = LD_ASR_NONE;
								break;
						}
						case LD_ASR_FOUNDZERO:
						default:
						{
								ASR_Status = LD_ASR_NONE;
								break;
						}
				} 
		}
}

void User_Modification(uint8_t data)
{
	if(data == CODE_CMD)																		// CODE_CMD:"xiao jie"的识别码
		{
				flag=1;
				SYN_FrameInfo(0, "[v16][m0][t5]主人，我在");
		}
		else if(flag)
		{
				flag=0;
				switch(data)				
				{
						case CODE_DMCS:																// CODE_DMCS:"ni hao ya"的识别码
							//回答“你好”	
							SYN_FrameInfo(0, "[v16][m0][t5]你好");
								break;
						case CODE_CSWB:																// CODE_CSWB:"zai gan ma ne"的识别码
							//回答“在学习”
							SYN_FrameInfo(0, "[v16][m0][t5]在学习");
								break;						
						case CODE_1KL1:																//CODE_1KL1:"kai deng"的识别码，往下以此类推
							//执行开灯操作，并回答“已为你打开灯”	
							SYN_FrameInfo(0, "[v16][m0][t5]已为你打开灯");
								break;
						case CODE_1KL2:	
							//执行关灯操作，并回答“已为你关闭灯”
							SYN_FrameInfo(0, "[v9][m0][t5]已为你关闭灯");
								break;
						case CODE_1KL3:	
							//执行调亮操作，并回答“已为你提高亮度”
							SYN_FrameInfo(0, "[v9][m0][t5]已为你提高亮度");
								break;
						case CODE_1KL4:	
							//执行调暗操作，并回答“已为你降低亮度”
							SYN_FrameInfo(0, "[v9][m0][t5]已为你降低亮度");
								break;
						case CODE_2KL1:	
							//回答“温度是XX摄氏度”
								break;
						case CODE_2KL2:	
							//回答“湿度是百分之XXRH”
								break;
						case CODE_2KL3:	
							//执行打开风扇操作，并回答“已为你打开风扇”
							SYN_FrameInfo(0, "[v9][m0][t5]已为你打开风扇");
								break;
						case CODE_2KL4:	
							//执行关闭风扇操作，并回答“已为你关闭风扇”
							SYN_FrameInfo(0, "[v9][m0][t5]已为你关闭风扇");
								break;								
						case CODE_3KL1:	
							//执行开门操作，并回答“已开门”
							SYN_FrameInfo(0, "[v9][m0][t5]已开门");
								break;
						case CODE_3KL2:	
							//执行开门操作，并回答“已关门”
							SYN_FrameInfo(0, "[v9][m0][t5]已关门");
								break;
						case CODE_5KL1:	
							
								break;
						
						default:break;
				}
		}
}
