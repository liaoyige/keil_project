#include "Reg_RW.h"
#include "LDChip.h"
#include "delay.h"

extern uint8_t ASR_Status;
uint8_t  nLD_Mode=LD_MODE_IDLE;
uint8_t  ucRegVal;



//���������� 	 ��λLDģ��

void LD3320_Reset(void)
{
	RSTB=1;
	delay_ms(5);
	RSTB=0;
	delay_ms(5);
	RSTB=1;
	
	delay_ms(5);
	SCS=0;
	delay_ms(5);
	SCS=1;
	delay_ms(5);
}
/************************************************************************
���������� LDģ�������ʼ��
˵���� �ú���Ϊ�������ã�һ�㲻��Ҫ�޸ģ�
**************************************************************************/
void LD3320_Init_Common(void)
{
	LD_ReadReg(0x06);  
	LD_WriteReg(0x17, 0x35);

	delay_ms(5);
	LD_ReadReg(0x06);  

	LD_WriteReg(0x89, 0x03); 
	delay_ms(5);
	LD_WriteReg(0xCF, 0x43);
	delay_ms(5);
	LD_WriteReg(0xCB, 0x02);
	
	/*PLL setting*/
	LD_WriteReg(0x11, LD_PLL_11);
	if (nLD_Mode == LD_MODE_MP3)
	{
		LD_WriteReg(0x1E,0x00);
		LD_WriteReg(0x19, LD_PLL_MP3_19); 
		LD_WriteReg(0x1B, LD_PLL_MP3_1B);		
		LD_WriteReg(0x1D, LD_PLL_MP3_1D);
	}
	else
	{
		LD_WriteReg(0x1E,0x00);
		LD_WriteReg(0x19, LD_PLL_ASR_19); 
		LD_WriteReg(0x1B, LD_PLL_ASR_1B);
	  LD_WriteReg(0x1D, LD_PLL_ASR_1D);
	}	
	delay_ms(5);
	
	LD_WriteReg(0xCD, 0x04);
	LD_WriteReg(0x17, 0x4c);
	delay_ms(5);
	LD_WriteReg(0xB9, 0x00);
	LD_WriteReg(0xCF, 0x4F);
	LD_WriteReg(0x6F, 0xFF);
}

/************************************************************************
���������� 	 LDģ�� ASR���ܳ�ʼ��
����˵����	 �ú���Ϊ�������ã�һ�㲻��Ҫ�޸ģ�
**************************************************************************/
void LD_Init_ASR(void)
{
	nLD_Mode=LD_MODE_ASR_RUN;
	LD3320_Init_Common();

	LD_WriteReg(0xBD, 0x00);
	LD_WriteReg(0x17, 0x48);
	delay_ms(5);

	LD_WriteReg(0x3C, 0x80);
	LD_WriteReg(0x3E, 0x07);
	LD_WriteReg(0x38, 0xff); 
	LD_WriteReg(0x3A, 0x07);
	
	LD_WriteReg(0x40, 0);
	LD_WriteReg(0x42, 8);
	LD_WriteReg(0x44, 0); 
	LD_WriteReg(0x46, 8);
	delay_ms(5);
}

/************************************************************************
���������� 	�жϴ�����
˵����	��LDģ����յ���Ƶ�ź�ʱ��������ú�����
						�ж�ʶ���Ƿ��н�������û�д������ü�
            ����׼����һ�ε�ʶ��
**************************************************************************/
void ProcessInt(void)
{
		uint8_t nAsrResCount=0;

		ucRegVal = LD_ReadReg(0x2B);
		LD_WriteReg(0x29,0) ;
		LD_WriteReg(0x02,0) ;
		if((ucRegVal & 0x10)&&LD_ReadReg(0xb2)==0x21&&LD_ReadReg(0xbf)==0x35)			/*ʶ��ɹ�*/
		{	
				nAsrResCount = LD_ReadReg(0xba);
			
				if(nAsrResCount>0 && nAsrResCount<=4) 
				{
						ASR_Status=LD_ASR_FOUNDOK;
				}
				else
				{
						ASR_Status=LD_ASR_FOUNDZERO;
				}	
		}															 /*û��ʶ����*/
		else
		{	 
				ASR_Status=LD_ASR_FOUNDZERO;
		}
			
		LD_WriteReg(0x2b, 0);
		LD_WriteReg(0x1C,0);/*д0:ADC������*/
		LD_WriteReg(0x29,0);
		LD_WriteReg(0x02,0);
		LD_WriteReg(0x2B,0);
		LD_WriteReg(0xBA,0);	
		LD_WriteReg(0xBC,0);	
		LD_WriteReg(0x08,1);	 /*���FIFO_DATA*/
		LD_WriteReg(0x08,0);	/*���FIFO_DATA�� �ٴ�д0*/
}

/************************************************************************
����������  ���LDģ���Ƿ����
�� �� ֵ�� 	flag��1-> ����
����˵����	none
**************************************************************************/
uint8_t LD_Check_ASRBusyFlag_b2(void)
{
	uint8_t j,i;
	uint8_t flag = 0;
	for (j=0; j<5; j++)
	{
		i=LD_ReadReg(0xb2);
		if ( i== 0x21)
		{
			flag = 1;						
			break;
		}
		delay_ms(20);		
	}
	return flag;
}

//���������� 	����ASR

void LD_AsrStart(void)
{
	LD_Init_ASR();
}
/************************************************************************
���������� 	����ASR
�� �� ֵ�� 	1�������ɹ�
**************************************************************************/
uint8_t LD_AsrRun(void)
{
	LD_WriteReg(0x35, MIC_VOL);
	LD_WriteReg(0x1C, 0x09);
	LD_WriteReg(0xBD, 0x20);
	LD_WriteReg(0x08, 0x01);
	delay_ms( 5 );
	LD_WriteReg(0x08, 0x00);
	delay_ms( 5 );

	if(LD_Check_ASRBusyFlag_b2() == 0)
	{
		return 0;
	}

	LD_WriteReg(0xB2, 0xff);
	LD_WriteReg(0x37, 0x06);
	delay_ms( 5 );
  LD_WriteReg(0x37, 0x06);
	delay_ms( 5 );
	LD_WriteReg(0x1C, 0x0b);
	LD_WriteReg(0x29, 0x10);	
	LD_WriteReg(0xBD, 0x00);
	return 1;
}
/************************************************************************
���������� ��LDģ����ӹؼ���
�� �� ֵ�� flag��1->��ӳɹ�
˵���� �û��޸�.
					 1���������¸�ʽ���ƴ���ؼ��ʣ�ͬʱע���޸�sRecog ��pCode ����ĳ���
					 �Ͷ�Ӧ����k��ѭ���á�ƴ������ʶ������һһ��Ӧ�ġ�
					 2�������߿���ѧϰ"����ʶ��оƬLD3320�߽��ؼ�.pdf"��
           ���������������մ�����÷������ṩʶ��Ч����
**************************************************************************/
uint8_t LD_AsrAddFixed(void)
{
		uint8_t k, flag;
		uint8_t nAsrAddLength;
		
		#define DATE_A 14   
		#define DATE_B 20			
		/*��ӹؼ���*/
		uint8_t sRecog[DATE_A][DATE_B] = {
																			"xiao jie",\
																			"ni hao ya",\
																			"zai gan ma ne",\
			
																			"kai deng",\
																			"guan deng",\
																			"tai an le",\
																			"tai liang le",\
			
																			"wen du duo shao",\
																			"shi du duo shao",\
																			"da kai feng shan",\
																			"guan bi feng shan",\
			
																			"kai men",\
																			"guan men",\
																			"hou tui",\
																	 };	
	/*���ʶ����*/
		uint8_t pCode[DATE_A] = {
														CODE_CMD,\
														CODE_DMCS,\
														CODE_CSWB,\
													
														CODE_1KL1,\
														CODE_1KL2,\
														CODE_1KL3,\
														CODE_1KL4,\
			
														CODE_2KL1,\
														CODE_2KL2,\
														CODE_2KL3,\
														CODE_2KL4,\
			
														CODE_3KL1,\
														CODE_3KL2,\
														CODE_5KL1,
													};		
		flag = 1;
		for (k=0; k<DATE_A; k++)
		{
				
				if(LD_Check_ASRBusyFlag_b2() == 0)
				{
						flag = 0;
						break;
				}
				
				LD_WriteReg(0xc1, pCode[k] );
				LD_WriteReg(0xc3, 0 );
				LD_WriteReg(0x08, 0x04);
				delay_ms(1);
				LD_WriteReg(0x08, 0x00);
				delay_ms(1);

				for (nAsrAddLength=0; nAsrAddLength<DATE_B; nAsrAddLength++)
				{
						if (sRecog[k][nAsrAddLength] == 0)
							break;
						LD_WriteReg(0x5, sRecog[k][nAsrAddLength]);
				}
				LD_WriteReg(0xb9, nAsrAddLength);
				LD_WriteReg(0xb2, 0xff);
				LD_WriteReg(0x37, 0x04);
		}
			return flag;
}

/************************************************************************
���������� 	����ASRʶ������
�� �� ֵ��  asrflag��1->�����ɹ��� 0��>����ʧ��
˵����	ʶ��˳������:
						1��RunASR()����ʵ����һ��������ASR����ʶ������
						2��LD_AsrStart() ����ʵ����ASR��ʼ��
						3��LD_AsrAddFixed() ����ʵ������ӹؼ����ﵽLD3320оƬ��
						4��LD_AsrRun()	����������һ��ASR����ʶ������					
						�κ�һ��ASRʶ�����̣�����Ҫ�������˳�򣬴ӳ�ʼ����ʼ��
**************************************************************************/
uint8_t RunASR(void)
{
	uint8_t i=0;
	uint8_t asrflag=0;
	for (i=0; i<5; i++)			//	��ֹ����Ӳ��ԭ����LD3320оƬ����������������һ������5������ASRʶ������
	{
		LD_AsrStart();
		delay_ms(5);
		if (LD_AsrAddFixed()==0)
		{
			LD3320_Reset();			//	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
			delay_ms(5);			//	���ӳ�ʼ����ʼ����ASRʶ������
			continue;
		}
		delay_ms(5);
		if (LD_AsrRun() == 0)
		{
			LD3320_Reset();			//	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
			delay_ms(5);			//	���ӳ�ʼ����ʼ����ASRʶ������
			continue;
		}	
		asrflag=1;
		break;					//	ASR���������ɹ����˳���ǰforѭ������ʼ�ȴ�LD3320�ͳ����ж��ź�
	}
	return asrflag;
}

/************************************************************************
���������� 	��ȡʶ����
�� �� ֵ�� 	LD_ReadReg(0xc5 )��  ��ȡ�ڲ��Ĵ�������ʶ���롣
**************************************************************************/
uint8_t LD3320_GetResult(void)
{	
	return LD_ReadReg(0xc5 );
}



