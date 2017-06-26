/**
  ******************************************************************************
  * @file    Project/USER/Typedef.h 
  * @author  
  * @version V1.1.0
  * @date    8-April-2014
  * @brief   Header for the defines of variable type. 
  ******************************************************************************/

#ifndef _TYPEDEFS_H__
#define _TYPEDEFS_H__
#include <stm32l1xx.h>
#include "rtwtypes.h"

#define GPIO_GROUP_OM_IDM1 GPIOA
#define GPIO_GROUP_OM_IDM2 GPIOB

#define GPIO_PIN_OM_IDM1 GPIO_Pin_15
#define GPIO_PIN_OM_IDM2 GPIO_Pin_7

#define GPIO_GROUP_IA_IDMPOS GPIOA

#define GPIO_PIN_IA_IDMPOS GPIO_Pin_4

#define AFIO_PIN_SOURCE_OM_IDM1 GPIO_PinSource15
#define AFIO_PIN_SOURCE_OM_IDM2 GPIO_PinSource7

#define AFIO_TIMx_OM_IDM1 GPIO_AF_TIM2
#define AFIO_TIMx_OM_IDM2 GPIO_AF_TIM4

typedef enum DisplayMode
{
	POSMODE,
	DUTYMODE
}DisplayMode;

typedef struct PWMInfo
{
	__IO uint8_t DutyCycle;
	__IO uint32_t Frequency;
}PWMInfo;

typedef struct BLDCMInputInfo
{
	__IO PWMInfo OM_IDM1;
	__IO PWMInfo OM_IDM2;
}ITHInputInfo;

typedef struct BLDCMOutputInfo
{
  int8_t ith_degree;                   /* '<Root>/ith_degree' */
  int8_t ith_pos;                      /* '<Root>/ith_pos' */
  uint16_t ith_pos_da;                 /* '<Root>/ith_pos_da' */
}ITHOutputInfo;

#endif
