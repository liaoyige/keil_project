#include "sys.h"
#include "usart.h"

#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
	int handle;

};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}
//�ض���fputc����
int fputc(int ch, FILE *f)
{
	while((USART3->SR & 0X40) == 0); //ѭ������,ֱ���������
	USART3->DR = (uint8_t) ch;
	return ch;
}
#endif

void USART3_SendData(uint8_t data)
{
	while((USART3->SR & 0X40) == 0);
	USART3->DR = data;
}

void USART3_SendString(uint8_t *DAT, uint8_t len)
{
		uint8_t i;
		for(i = 0; i < len; i++)
		{
				USART3_SendData(*DAT++);
		}
}

#if EN_USART3_RX  

uint8_t USART3_RX_BUF[USART3_REC_LEN];     														//���ջ���,���USART_REC_LEN���ֽ�.

uint16_t USART3_RX_STA = 0;     																						//����״̬���

void USART3_Init(uint32_t bound)
{
		//GPIO�˿�����
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);								//ʹ��USART3��GPIOCʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		//USART3_TX   GPIOC.10
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;											//�����������
		GPIO_Init(GPIOB, &GPIO_InitStructure);															//��ʼ��GPIO

		//USART3_RX	  GPIOC.11��ʼ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;								//��������
		GPIO_Init(GPIOB, &GPIO_InitStructure);															//��ʼ��GPIO

		//USART3 NVIC ����
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; 					//��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;									//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;											//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);																			//����ָ���Ĳ�����ʼ��VIC�Ĵ���

		//USART ��ʼ������

		USART_InitStructure.USART_BaudRate = bound;													//���ڲ�����
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;					//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;									//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;			//�շ�ģʽ

		USART_Init(USART3, &USART_InitStructure);													 //��ʼ������1
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);										 //�������ڽ����ж�
		USART_Cmd(USART3, ENABLE);                   											 //ʹ�ܴ���1	
}

void USART3_IRQHandler(void)                													//����1�жϷ������
{
		uint8_t Res;
		if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
		{
				Res = USART_ReceiveData(USART3);															//��ȡ���յ�������
		}
}
#endif

