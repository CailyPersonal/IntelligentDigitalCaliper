
/* ʵ�ֹ��ܣ���ȡ����ÿ����һ���������������жϴ�������,����ÿ���͵�һ�������ж���λ */
/*	����һ��������50ms��Χ֮��,��, 50ms�ڴ������жϴ���*/

#include "stm32f10x.h"
void RCC_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(void);
void NVIC_Configuration(void);
void EXTI_PA1_Config(void);
void TIM2_Configuration(void);
void TIM3_Configuration(void);

u8 ReceiveCount ;

int main(void)
{
	RCC_Configuration();
	GPIO_Configuration();
	USART_Configuration();
	NVIC_Configuration();
	EXTI_PA1_Config();
	TIM2_Configuration();
	TIM3_Configuration();
	
	ReceiveCount=0;
	
	while(1)
	{		
	}
}

void RCC_Configuration(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA| RCC_APB2Periph_GPIOB
													| RCC_APB2Periph_AFIO,ENABLE);
}

void GPIO_Configuration(void)
{
	/* config PA9Ϊ��� */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;									//��AF_PPģʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* config PA10Ϊ���� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	/* config PB7,PB7���Ž�������*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	GPIO_PinRemapConfig(GPIO_Remap_USART1,DISABLE);//Remap
	
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //Enable interrupt
	
	USART_Cmd(USART1,ENABLE);				//*****
	USART_ClearFlag(USART1,USART_FLAG_TC);
}

void NVIC_Configuration(void)
{
	 NVIC_InitTypeDef NVIC_InitStructure;
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	 NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
	 NVIC_Init(&NVIC_InitStructure);
	
	 NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	
	 NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);
}

void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	TIM_DeInit(TIM2);
	TIM_InternalClockConfig(TIM2);
	
	/*��ʱ50ms*/
	TIM_TimeBaseStructure.TIM_Period = 69;
	TIM_TimeBaseStructure.TIM_Prescaler = 35999;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE );			//ʹ�ܻ�ʧ��ָ����TIM�ж�
	
	TIM_Cmd(TIM2, ENABLE);
}

void TIM3_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	TIM_DeInit(TIM3);
	TIM_InternalClockConfig(TIM3);
	
	/*��ʱ400΢�� */
	TIM_TimeBaseStructure.TIM_Period = 71;
	TIM_TimeBaseStructure.TIM_Prescaler = 399;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE );		
	
	TIM_Cmd(TIM3, ENABLE);
}

void EXTI_PA1_Config(void)				//PA1���Ž�ʱ����
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 EXTI_InitTypeDef EXTI_InitStructure;

	 /* EXTI line gpio config(PA1) */
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // ���뷽ʽ
	 GPIO_Init(GPIOA, &GPIO_InitStructure);

	 /* EXTI line(PA1) mode config */
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	 EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //

	 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	 EXTI_Init(&EXTI_InitStructure);
	}

/* ��ʱ���жϺ��������ӳ��� */
void  TIM2_IRQHandler(void) 
{
		
		TIM_Cmd(TIM2, DISABLE);	
		
		USART_SendData(USART1,ReceiveCount);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		ReceiveCount=0;			//ReceiveCount���¸�ֵ
	
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);	
	
	/* �ظ����裬���¼�ʱ */
		TIM_Cmd(TIM2, ENABLE);	
		TIM_Cmd(TIM3, ENABLE);	
}

void  TIM3_IRQHandler(void) 
{
		TIM_Cmd(TIM3, DISABLE);	
	
		/*���ⲿ�ж�*/
		EXTI->IMR = 0x02;
		EXTI->EMR = 0x02;
	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

/* �ⲿ�жϺ��������ӳ��� */
void EXTI1_IRQHandler (void)
{
		TIM_Cmd(TIM3, DISABLE);	
		ReceiveCount++;
	
//	EXTI_ClearFlag(EXTI_Line1);
		EXTI_ClearITPendingBit(EXTI_Line1); 
		
		/*�ر��ⲿ�ж�*/
		EXTI->IMR = 0x00;
		EXTI->EMR = 0x00;
}


