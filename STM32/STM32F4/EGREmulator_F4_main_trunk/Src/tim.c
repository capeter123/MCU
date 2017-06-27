/**
  ******************************************************************************
  * File Name          : TIM.c
  * Date               : 30/10/2015 09:49:41
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "EGR_BLL_Interface.h"
#include "Task_Scheduler.h"

PWMInfo PWM_CCP_CH1 = {0};	// TIM2_CH1 -- PA15
PWMInfo PWM_CCP_CH2 = {0}; //  TIM12_CH1 -- PB14
PWMInfo PWM_CCP_CH3 = {0}; //  TIM9_CH1 -- PE5

static _Bool bHasUpdateARR = FALSE;
static uint32_t u32PWMFreqFnl = 0;

static void CalcPWMInfo(TIM_HandleTypeDef* htim, PWMInfo* ch_info);
/* USER CODE END 0 */

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim9;
TIM_HandleTypeDef htim12;

/* TIM2 init function */
void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_SlaveConfigTypeDef sSlaveConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_IC_InitTypeDef sConfigIC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 42;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0xFFFF;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim2);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

  HAL_TIM_IC_Init(&htim2);

  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sSlaveConfig.TriggerFilter = 0;
  HAL_TIM_SlaveConfigSynchronization(&htim2, &sSlaveConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1);

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_2);

}
/* TIM6 init function */
void MX_TIM6_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig;

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 84;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 100;
  HAL_TIM_Base_Init(&htim6);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig);

}
/* TIM9 init function */
void MX_TIM9_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_SlaveConfigTypeDef sSlaveConfig;
  TIM_IC_InitTypeDef sConfigIC;

  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 84;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 0xFFFF;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim9);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig);

  HAL_TIM_IC_Init(&htim9);

  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sSlaveConfig.TriggerFilter = 0;
  HAL_TIM_SlaveConfigSynchronization(&htim9, &sSlaveConfig);

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  HAL_TIM_IC_ConfigChannel(&htim9, &sConfigIC, TIM_CHANNEL_1);

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  HAL_TIM_IC_ConfigChannel(&htim9, &sConfigIC, TIM_CHANNEL_2);

}
/* TIM12 init function */
void MX_TIM12_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_SlaveConfigTypeDef sSlaveConfig;
  TIM_IC_InitTypeDef sConfigIC;

  htim12.Instance = TIM12;
  htim12.Init.Prescaler = 42;
  htim12.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim12.Init.Period = 0xFFFF;
  htim12.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim12);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim12, &sClockSourceConfig);

  HAL_TIM_IC_Init(&htim12);

  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sSlaveConfig.TriggerFilter = 0;
  HAL_TIM_SlaveConfigSynchronization(&htim12, &sSlaveConfig);

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  HAL_TIM_IC_ConfigChannel(&htim12, &sConfigIC, TIM_CHANNEL_1);

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  HAL_TIM_IC_ConfigChannel(&htim12, &sConfigIC, TIM_CHANNEL_2);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_base->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* Peripheral clock enable */
    __TIM2_CLK_ENABLE();
  
    /**TIM2 GPIO Configuration    
    PA15     ------> TIM2_CH1 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 5);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspInit 1 */
		__HAL_TIM_URS_ENABLE(htim_base);
		htim_base->Instance->CR1 |= TIM_CR1_ARPE;
  /* USER CODE END TIM2_MspInit 1 */
  }
  else if(htim_base->Instance==TIM6)
  {
  /* USER CODE BEGIN TIM6_MspInit 0 */

  /* USER CODE END TIM6_MspInit 0 */
    /* Peripheral clock enable */
    __TIM6_CLK_ENABLE();

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
  /* USER CODE BEGIN TIM6_MspInit 1 */

  /* USER CODE END TIM6_MspInit 1 */
  }
  else if(htim_base->Instance==TIM9)
  {
  /* USER CODE BEGIN TIM9_MspInit 0 */

  /* USER CODE END TIM9_MspInit 0 */
    /* Peripheral clock enable */
    __TIM9_CLK_ENABLE();
  
    /**TIM9 GPIO Configuration    
    PE5     ------> TIM9_CH1 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM9;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 0, 7);
    HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
  /* USER CODE BEGIN TIM9_MspInit 1 */
		__HAL_TIM_URS_ENABLE(htim_base);
		htim_base->Instance->CR1 |= TIM_CR1_ARPE;
  /* USER CODE END TIM9_MspInit 1 */
  }
  else if(htim_base->Instance==TIM12)
  {
  /* USER CODE BEGIN TIM12_MspInit 0 */

  /* USER CODE END TIM12_MspInit 0 */
    /* Peripheral clock enable */
    __TIM12_CLK_ENABLE();
  
    /**TIM12 GPIO Configuration    
    PB14     ------> TIM12_CH1 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_TIM12;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 0, 6);
    HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
  /* USER CODE BEGIN TIM12_MspInit 1 */
		__HAL_TIM_URS_ENABLE(htim_base);
		htim_base->Instance->CR1 |= TIM_CR1_ARPE;
  /* USER CODE END TIM12_MspInit 1 */
  }
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef* htim_ic)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_ic->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* Peripheral clock enable */
    __TIM2_CLK_ENABLE();
  
    /**TIM2 GPIO Configuration    
    PA15     ------> TIM2_CH1 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 5);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspInit 1 */
		__HAL_TIM_URS_ENABLE(htim_ic);
		htim_ic->Instance->CR1 |= TIM_CR1_ARPE;
  /* USER CODE END TIM2_MspInit 1 */
  }
  else if(htim_ic->Instance==TIM9)
  {
  /* USER CODE BEGIN TIM9_MspInit 0 */

  /* USER CODE END TIM9_MspInit 0 */
    /* Peripheral clock enable */
    __TIM9_CLK_ENABLE();
  
    /**TIM9 GPIO Configuration    
    PE5     ------> TIM9_CH1 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM9;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 0, 7);
    HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
  /* USER CODE BEGIN TIM9_MspInit 1 */
		__HAL_TIM_URS_ENABLE(htim_ic);
		htim_ic->Instance->CR1 |= TIM_CR1_ARPE;
  /* USER CODE END TIM9_MspInit 1 */
  }
  else if(htim_ic->Instance==TIM12)
  {
  /* USER CODE BEGIN TIM12_MspInit 0 */

  /* USER CODE END TIM12_MspInit 0 */
    /* Peripheral clock enable */
    __TIM12_CLK_ENABLE();
  
    /**TIM12 GPIO Configuration    
    PB14     ------> TIM12_CH1 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_TIM12;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 0, 6);
    HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
  /* USER CODE BEGIN TIM12_MspInit 1 */
		__HAL_TIM_URS_ENABLE(htim_ic);
		htim_ic->Instance->CR1 |= TIM_CR1_ARPE;
  /* USER CODE END TIM12_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __TIM2_CLK_DISABLE();
  
    /**TIM2 GPIO Configuration    
    PA15     ------> TIM2_CH1 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM2_IRQn);

  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
  else if(htim_base->Instance==TIM6)
  {
  /* USER CODE BEGIN TIM6_MspDeInit 0 */

  /* USER CODE END TIM6_MspDeInit 0 */
    /* Peripheral clock disable */
    __TIM6_CLK_DISABLE();

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);

  /* USER CODE BEGIN TIM6_MspDeInit 1 */

  /* USER CODE END TIM6_MspDeInit 1 */
  }
  else if(htim_base->Instance==TIM9)
  {
  /* USER CODE BEGIN TIM9_MspDeInit 0 */

  /* USER CODE END TIM9_MspDeInit 0 */
    /* Peripheral clock disable */
    __TIM9_CLK_DISABLE();
  
    /**TIM9 GPIO Configuration    
    PE5     ------> TIM9_CH1 
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_5);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM1_BRK_TIM9_IRQn);

  /* USER CODE BEGIN TIM9_MspDeInit 1 */

  /* USER CODE END TIM9_MspDeInit 1 */
  }
  else if(htim_base->Instance==TIM12)
  {
  /* USER CODE BEGIN TIM12_MspDeInit 0 */

  /* USER CODE END TIM12_MspDeInit 0 */
    /* Peripheral clock disable */
    __TIM12_CLK_DISABLE();
  
    /**TIM12 GPIO Configuration    
    PB14     ------> TIM12_CH1 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_14);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM8_BRK_TIM12_IRQn);

  /* USER CODE BEGIN TIM12_MspDeInit 1 */

  /* USER CODE END TIM12_MspDeInit 1 */
  }
}

void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef* htim_ic)
{

  if(htim_ic->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __TIM2_CLK_DISABLE();
  
    /**TIM2 GPIO Configuration    
    PA15     ------> TIM2_CH1 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM2_IRQn);

  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
  else if(htim_ic->Instance==TIM9)
  {
  /* USER CODE BEGIN TIM9_MspDeInit 0 */

  /* USER CODE END TIM9_MspDeInit 0 */
    /* Peripheral clock disable */
    __TIM9_CLK_DISABLE();
  
    /**TIM9 GPIO Configuration    
    PE5     ------> TIM9_CH1 
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_5);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM1_BRK_TIM9_IRQn);

  /* USER CODE BEGIN TIM9_MspDeInit 1 */

  /* USER CODE END TIM9_MspDeInit 1 */
  }
  else if(htim_ic->Instance==TIM12)
  {
  /* USER CODE BEGIN TIM12_MspDeInit 0 */

  /* USER CODE END TIM12_MspDeInit 0 */
    /* Peripheral clock disable */
    __TIM12_CLK_DISABLE();
  
    /**TIM12 GPIO Configuration    
    PB14     ------> TIM12_CH1 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_14);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM8_BRK_TIM12_IRQn);

  /* USER CODE BEGIN TIM12_MspDeInit 1 */

  /* USER CODE END TIM12_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
/**
  * @brief  This function calculate the PWMInfo according to CCR1 and CCR2.
  * @param  TIMx, TIMx_IT
  * @retval PWMInfo
  */
static void CalcPWMInfo(TIM_HandleTypeDef* htim, PWMInfo* ch_info)
{
	uint32_t ICCRxValue = 0;
	PWMInfo pwmInfo = {0};
		
	if(HAL_TIM_ACTIVE_CHANNEL_1 == htim->Channel)
	{
		ICCRxValue = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		if(ICCRxValue!=0)
		{
			pwmInfo.DutyCycle = (HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2)*100)/ICCRxValue;
			pwmInfo.Frequency = TIMx_CLK_FREQ/ICCRxValue;
			if(!bHasUpdateARR && pwmInfo.Frequency >= VALID_PWM_FREQ_LOWER && pwmInfo.Frequency <= VALID_PWM_FREQ_UPPER)
			{
				if( u32PWMFreqFnl == pwmInfo.Frequency)
				{
					/* update all timer's ARR (that is TIM9, TIM12) after the system has studied 
						* the frequence on the fly (I think the genuine value if the timer has captured the same freq 
					  * within two sample action), and 2000 is the freq-band protection region. */
					EBM_1L_TIM->ARR = EBM_2L_TIM->ARR = EBM_3L_TIM->ARR = ICCRxValue + 1000;		
					bHasUpdateARR = TRUE;
				}
				
				u32PWMFreqFnl = pwmInfo.Frequency;
			}
		}
		else
		{
			pwmInfo.DutyCycle = pwmInfo.Frequency = 0;
		}
	}
	else if(HAL_TIM_ACTIVE_CHANNEL_2 == htim->Channel)
	{
		ICCRxValue = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		if(ICCRxValue!=0)
		{
			pwmInfo.DutyCycle = ((HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1)+1)*100)/ICCRxValue;
			pwmInfo.Frequency = TIMx_CLK_FREQ/ICCRxValue;
			if(!bHasUpdateARR &&  pwmInfo.Frequency >= VALID_PWM_FREQ_LOWER && pwmInfo.Frequency <= VALID_PWM_FREQ_UPPER)
			{
				if(u32PWMFreqFnl == pwmInfo.Frequency)
				{
					/* update all timer's ARR (that is TIM9, TIM12) after the system has studied 
						* the frequence on the fly (I think the genuine value if the timer has captured the same freq 
					  * within two sample action), and 2000 is the freq-band protection region. */
					EBM_1L_TIM->ARR = EBM_2L_TIM->ARR = EBM_3L_TIM->ARR = ICCRxValue + 1000;
					bHasUpdateARR = TRUE;
				}
				
				u32PWMFreqFnl = pwmInfo.Frequency;
			}
		}
		else
		{
			pwmInfo.DutyCycle = pwmInfo.Frequency = 0;
		}
	}
	
	ch_info->DutyCycle = pwmInfo.DutyCycle;
	ch_info->Frequency = u32PWMFreqFnl;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	/* 100us Period Timer ISR Handler */
	if(htim->Instance == TIM6)
	{
		/* 100us event */
		rt_Model_ISRHandler(); // Execute EGR BLDC Model Task.
	}
	
	if(htim->Instance == EBM_1L_TIM)
	{
		PWM_CCP_CH1.DutyCycle = (HAL_GPIO_ReadPin(GPIO_GROUP_EBM_1L, GPIO_PIN_EBM_1L) == GPIO_PIN_SET ? 100:0);
		PWM_CCP_CH1.Frequency = 0;
	}
	
	if(htim->Instance == EBM_2L_TIM)
	{
		PWM_CCP_CH2.DutyCycle = (HAL_GPIO_ReadPin(GPIO_GROUP_EBM_2L, GPIO_PIN_EBM_2L) == GPIO_PIN_SET ? 100:0);
		PWM_CCP_CH2.Frequency = 0;
	}
	
	if(htim->Instance == EBM_3L_TIM)
	{
		PWM_CCP_CH3.DutyCycle = (HAL_GPIO_ReadPin(GPIO_GROUP_EBM_3L, GPIO_PIN_EBM_3L) == GPIO_PIN_SET ? 100:0);
		PWM_CCP_CH3.Frequency = 0;
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(HAL_TIM_ACTIVE_CHANNEL_1 == htim->Channel)
	{
		if(EBM_1L_TIM == htim->Instance)
			 CalcPWMInfo(htim, &PWM_CCP_CH1);
		else if(EBM_2L_TIM == htim->Instance)
			CalcPWMInfo(htim, &PWM_CCP_CH2);
		else if(EBM_3L_TIM == htim->Instance)
			CalcPWMInfo(htim, &PWM_CCP_CH3);
	}
}

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
