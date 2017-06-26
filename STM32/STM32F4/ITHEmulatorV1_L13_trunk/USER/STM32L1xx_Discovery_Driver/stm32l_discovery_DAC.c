 /**
  ******************************************************************************
  * @file    stm32l_discovery_DAC.c
  * @author   jiangst.fnst@cn.fujitsu.com
  * @version 
  * @date    April-2014
  * @brief   This file contains all the functions prototypes for the glass LCD
  *          firmware driver.
  ******************************************************************************/
	
#include "stm32l_discovery_DAC.h"

static void DAC_RCC_Config(void);
static void DAC_GPIO_Config(void);
static void DAC_Mode_Config(void);

void DAC_InitConfig(void)
{
	DAC_RCC_Config();
	DAC_GPIO_Config();
	DAC_Mode_Config();
}

static void DAC_RCC_Config(void)
{
	/* Enable GPIOA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/* Enable DAC1 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
}
	
static void DAC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_IA_IDMPOS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_GROUP_IA_IDMPOS, &GPIO_InitStructure);
}

static void DAC_Mode_Config(void)
{
	DAC_InitTypeDef DAC_InitStructure;
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	DAC_Cmd(DAC_Channel_1, ENABLE);
}

void DAC_OutputData(uint16_t data)
{
	DAC_SetChannel1Data(DAC_Align_12b_R, data);
}
