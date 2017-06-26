#ifndef _TIMx_PWM_CAPTURE_H_
#define _TIMx_PWM_CAPTURE_H_

#include <stm32l1xx.h>
#include "Typedefs.h"

#define TIMx_CLK_FREQ (uint32_t)2000000 //Config the TIM Base Freqence is 4MHz.
#define VALID_PWM_FREQ_LOWER 90
#define VALID_PWM_FREQ_UPPER 2100

// function defines.
void TIMx_CCP_InitConfig(void);

// CCPs ISR Interface. 
void TIM2_CCP_ISRHandler(void);
void TIM4_CCP_ISRHandler(void);

// EXTI ISR Interface.
//void EXTI0_ISRHandler(void);
#endif
