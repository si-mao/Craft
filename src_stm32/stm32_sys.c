#include "system_define.h"
#include "stm32_config.h"


void SystemInit(void)
{
}

void RCC_Configuration(void)
{   
	/* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	if(RCC_WaitForHSEStartUp() == SUCCESS)   /* Wait till HSE is ready �ȴ��ⲿ��������*/
	{
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);
		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1);
		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);
		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);		 //9��Ƶ  8M*9=72M
		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);
		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}
}

void NVIC_Configuration(void)
{
	uint8_t NVIC_Init_List[] = NVIC_INIT_LIST_DATA;
	uint8_t i;
	NVIC_InitTypeDef	NVIC_InitStructure;
	#ifdef  VECT_TAB_RAM  									//���������ַѡ��
	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);  			//��0x20000000��ַ��Ϊ���������ַ(RAM)
	#else  
	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0); 		//��0x08000000��ַ��Ϊ���������ַ(FLASH)  
	#endif
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 		/*!< 2 bits for pre-emption priority 2 bits for subpriority */

	//SysTick�ж�
#if (NVIC_SYSTICK_EN == 1)
	NVIC_SetPriority(SysTick_IRQn, 0);  //��ռ���ȼ�0, ��Ӧ���ȼ�0
#endif

	i = 0;
	while(NVIC_Init_List[i] != 0xFF)
	{
		NVIC_InitStructure.NVIC_IRQChannel = NVIC_Init_List[i]; 		                   //�ж��¼�ΪTIM2
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_Init_List[i + 1];  //��ռ���ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_Init_List[i + 2];         //��Ӧ���ȼ�0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                //�����ж� 
		NVIC_Init(&NVIC_InitStructure);
		i += 3;
	}
}
