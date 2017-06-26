#include "TIMx_PWM_Capture.h"

PWMInfo PWM_CCP_CH1 = {0}; // TIM2_CH1 -- PA(0)
PWMInfo PWM_CCP_CH2 = {0}; // TIM4_CH2 -- PB(07)
//extern DisplayMode gDisplayMode;  
//static _Bool bFirstCapture = 1;
static _Bool bHasUpdateARR = FALSE;
static uint32_t u32PWMFreqOld = 0;

void TIMx_RCC_Config(void);
void TIMx_NVIC_Config(void);
void TIMx_GPIO_Config(void);
void TIMx_Mode_Config(void);
void EXTI0_Config(void);
static PWMInfo CalcPWMInfo(TIM_TypeDef* TIMx, uint16_t TIMx_IT);
void TIMx_CCPMode_Configuration(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_InputTriggerSource);

void TIMx_CCP_InitConfig(void)
{
	TIMx_RCC_Config();
	TIMx_NVIC_Config();
	TIMx_GPIO_Config();
	//EXTI0_Config(); // Not support the Duty display mode as PA0 has be occured.
	TIMx_Mode_Config();
}

void TIMx_RCC_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
}

void TIMx_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIMx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_OM_IDM1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	
	GPIO_Init(GPIO_GROUP_OM_IDM1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_OM_IDM2;
	GPIO_Init(GPIO_GROUP_OM_IDM2, &GPIO_InitStructure);
	
	// Enabled Remap AFIO
	GPIO_PinAFConfig(GPIO_GROUP_OM_IDM1, AFIO_PIN_SOURCE_OM_IDM1, AFIO_TIMx_OM_IDM1);
	GPIO_PinAFConfig(GPIO_GROUP_OM_IDM2, AFIO_PIN_SOURCE_OM_IDM2, AFIO_TIMx_OM_IDM2);
}

void EXTI0_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStrcuture;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	// EXTI NVIC Config.
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_Init(&NVIC_InitStructure);
	
	// GPIO Config.
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	EXTI_InitStrcuture.EXTI_Line = EXTI_Line0;
	EXTI_InitStrcuture.EXTI_LineCmd = ENABLE;
	EXTI_InitStrcuture.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStrcuture.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStrcuture);
}

void TIMx_Mode_Config(void)
{
	TIMx_CCPMode_Configuration(TIM2, TIM_Channel_1, TIM_TS_TI1FP1);
	TIMx_CCPMode_Configuration(TIM4, TIM_Channel_2, TIM_TS_TI2FP2);
}

/**PRIVATE FUNCTIONS--------------------------------*/
/**
  * @brief  This function configuration the TIMx with CCP Mode.
  * @param  TIMx, TIM_Channel
  * @retval None
  */
void TIMx_CCPMode_Configuration(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_InputTriggerSource)
{
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 3000;
	TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock/TIMx_CLK_FREQ;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_PWMIConfig(TIMx, &TIM_ICInitStructure);
	
	TIM_SelectInputTrigger(TIMx, TIM_InputTriggerSource);
	//TIM_SelectMasterSlaveMode(TIMx, TIM_MasterSlaveMode_Enable);
	TIM_SelectSlaveMode(TIMx, TIM_SlaveMode_Reset);
	TIM_ARRPreloadConfig(TIMx, ENABLE);
	
	TIM_UpdateRequestConfig(TIMx, TIM_UpdateSource_Regular);
	if(TIM_Channel_1 == TIM_Channel)
		TIM_ITConfig(TIMx, TIM_IT_CC1 | TIM_IT_Update, ENABLE);
	else if(TIM_Channel_2 == TIM_Channel)
		TIM_ITConfig(TIMx, TIM_IT_CC2 | TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIMx, ENABLE);
}

/**
  * @brief  This function calculate the PWMInfo according to CCR1 and CCR2.
  * @param  TIMx, TIMx_IT
  * @retval PWMInfo
  */
static PWMInfo CalcPWMInfo(TIM_TypeDef* TIMx, uint16_t TIMx_IT)
{
	uint16_t ICCRxValue = 0;
	PWMInfo pwmInfo = {0};
	assert_param(IS_TIM_IT(TIMx_IT));
		
	if(TIM_IT_CC1 == TIMx_IT)
	{
		ICCRxValue = TIM_GetCapture1(TIMx);
		if(ICCRxValue!=0)
		{
			pwmInfo.DutyCycle = (TIM_GetCapture2(TIMx)*100)/ICCRxValue;
			pwmInfo.Frequency = TIMx_CLK_FREQ/ICCRxValue;
			if(pwmInfo.Frequency >= VALID_PWM_FREQ_LOWER && pwmInfo.Frequency <= VALID_PWM_FREQ_UPPER)
			{
				if(!bHasUpdateARR && u32PWMFreqOld == pwmInfo.Frequency)
				{
					/* update all timer's ARR (that is TIM2, TIM4) after the system has studied 
						* the frequence on the fly (I think the genuine value if the timer has captured the same freq 
					  * within two sample action), and 2000 is the freq-band protection region. */
					TIM2->ARR = TIM4->ARR = ICCRxValue + 1000;		
					bHasUpdateARR = TRUE;
				}
				
				u32PWMFreqOld = pwmInfo.Frequency;
			}
////			if(pwmInfo.Frequency >= VALID_PWM_FREQ_LOWER && pwmInfo.Frequency <= VALID_PWM_FREQ_UPPER)
////			{
////				if(!bFirstCapture && !bHasUpdateARR)
////				{
////					TIMx->ARR = ICCRxValue + 1000;	// 2000 is the freq-band protection region.
////					bHasUpdateARR = 1;
////				}
////				
////				bFirstCapture = 0; // ignored the first sample of frequence.
////			}
		}
		else
		{
			pwmInfo.DutyCycle = pwmInfo.Frequency = 0;
		}
	}
	else if(TIM_IT_CC2 == TIMx_IT)
	{
		ICCRxValue = TIM_GetCapture2(TIMx);
		if(ICCRxValue!=0)
		{
			pwmInfo.DutyCycle = ((TIM_GetCapture1(TIMx)+1)*100)/ICCRxValue;
			pwmInfo.Frequency = TIMx_CLK_FREQ/ICCRxValue;
			if(pwmInfo.Frequency >= VALID_PWM_FREQ_LOWER && pwmInfo.Frequency <= VALID_PWM_FREQ_UPPER)
			{
				if(!bHasUpdateARR && u32PWMFreqOld == pwmInfo.Frequency)
				{
					/* update all timer's ARR (that is TIM2, TIM4) after the system has studied 
						* the frequence on the fly (I think the genuine value if the timer has captured the same freq 
					  * within two sample action), and 2000 is the freq-band protection region. */
					TIM2->ARR = TIM4->ARR = ICCRxValue + 1000;		
					bHasUpdateARR = TRUE;
				}
				
				u32PWMFreqOld = pwmInfo.Frequency;
			}
		}
		else
		{
			pwmInfo.DutyCycle = pwmInfo.Frequency = 0;
		}
	}
	
	return pwmInfo;
}

void TIM2_CCP_ISRHandler(void)
{
	if(SET == TIM_GetITStatus(TIM2, TIM_IT_CC1))
	{
		PWM_CCP_CH1 = CalcPWMInfo(TIM2, TIM_IT_CC1);
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	}
	else if(SET == TIM_GetITStatus(TIM2, TIM_IT_Update))
	{
		//supply the measurement of duty in the circumstance of duty == 100%.
		PWM_CCP_CH1.DutyCycle = (GPIO_ReadInputDataBit(GPIO_GROUP_OM_IDM1, GPIO_PIN_OM_IDM1) == SET ? 100:0);
		PWM_CCP_CH1.Frequency = 0;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}


void TIM4_CCP_ISRHandler(void)
{
	if(SET == TIM_GetITStatus(TIM4, TIM_IT_CC2))
	{
		PWM_CCP_CH2 = CalcPWMInfo(TIM4, TIM_IT_CC2);
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
	}
	else if(SET == TIM_GetITStatus(TIM4, TIM_IT_Update))
	{
		PWM_CCP_CH2.DutyCycle = (GPIO_ReadInputDataBit(GPIO_GROUP_OM_IDM2, GPIO_PIN_OM_IDM2) == SET ? 100:0);
		PWM_CCP_CH2.Frequency = 0;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}

//void EXTI0_ISRHandler(void)
//{
//	if(SET == EXTI_GetITStatus(EXTI_Line0))
//	{
//		if(POSMODE == gDisplayMode)
//			gDisplayMode = DUTYMODE;
//		else
//			gDisplayMode = POSMODE;
//		
//		EXTI_ClearITPendingBit(EXTI_Line0);
//	}
//}
