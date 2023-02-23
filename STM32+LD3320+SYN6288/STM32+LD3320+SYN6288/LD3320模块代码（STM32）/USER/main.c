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
		ASR_Status = LD_ASR_NONE;																						//	��ʼ״̬��û������ASR
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
							
								if (RunASR()==0)																				//����һ��ASRʶ�����̣�ASR��ʼ����ASR��ӹؼ��������ASR����
								{
									ASR_Status = LD_ASR_ERROR;
								}
								break;
						}
						case LD_ASR_FOUNDOK: 																				//	һ��ASRʶ�����̽�����ȥȡASRʶ����
						{
								nASR_Res = LD3320_GetResult();															//��ȡ���												
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
	if(data == CODE_CMD)																		// CODE_CMD:"xiao jie"��ʶ����
		{
				flag=1;
				SYN_FrameInfo(0, "[v16][m0][t5]���ˣ�����");
		}
		else if(flag)
		{
				flag=0;
				switch(data)				
				{
						case CODE_DMCS:																// CODE_DMCS:"ni hao ya"��ʶ����
							//�ش���á�	
							SYN_FrameInfo(0, "[v16][m0][t5]���");
								break;
						case CODE_CSWB:																// CODE_CSWB:"zai gan ma ne"��ʶ����
							//�ش���ѧϰ��
							SYN_FrameInfo(0, "[v16][m0][t5]��ѧϰ");
								break;						
						case CODE_1KL1:																//CODE_1KL1:"kai deng"��ʶ���룬�����Դ�����
							//ִ�п��Ʋ��������ش���Ϊ��򿪵ơ�	
							SYN_FrameInfo(0, "[v16][m0][t5]��Ϊ��򿪵�");
								break;
						case CODE_1KL2:	
							//ִ�йصƲ��������ش���Ϊ��رյơ�
							SYN_FrameInfo(0, "[v9][m0][t5]��Ϊ��رյ�");
								break;
						case CODE_1KL3:	
							//ִ�е������������ش���Ϊ��������ȡ�
							SYN_FrameInfo(0, "[v9][m0][t5]��Ϊ���������");
								break;
						case CODE_1KL4:	
							//ִ�е������������ش���Ϊ�㽵�����ȡ�
							SYN_FrameInfo(0, "[v9][m0][t5]��Ϊ�㽵������");
								break;
						case CODE_2KL1:	
							//�ش��¶���XX���϶ȡ�
								break;
						case CODE_2KL2:	
							//�ش�ʪ���ǰٷ�֮XXRH��
								break;
						case CODE_2KL3:	
							//ִ�д򿪷��Ȳ��������ش���Ϊ��򿪷��ȡ�
							SYN_FrameInfo(0, "[v9][m0][t5]��Ϊ��򿪷���");
								break;
						case CODE_2KL4:	
							//ִ�йرշ��Ȳ��������ش���Ϊ��رշ��ȡ�
							SYN_FrameInfo(0, "[v9][m0][t5]��Ϊ��رշ���");
								break;								
						case CODE_3KL1:	
							//ִ�п��Ų��������ش��ѿ��š�
							SYN_FrameInfo(0, "[v9][m0][t5]�ѿ���");
								break;
						case CODE_3KL2:	
							//ִ�п��Ų��������ش��ѹ��š�
							SYN_FrameInfo(0, "[v9][m0][t5]�ѹ���");
								break;
						case CODE_5KL1:	
							
								break;
						
						default:break;
				}
		}
}
