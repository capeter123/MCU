/**
  ******************************************************************************
  * @file    Project/USER/main.h 
  * @author  
  * @version V1.1.0
  * @date    8-April-2014
  * @brief   Header for main.c module
  ******************************************************************************/
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include <stm32l1xx.h>
#include <stdio.h>
#include "stm32l_discovery_lcd.h"
#include "stm32l_discovery_spi.h"
#include "TIMx_PWM_Capture.h"
#include "stm32l_discovery_DAC.h"
#include "Model_ert_rtw.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void TimingDelay_Decrement(void);
void Sleep(__IO uint32_t nTime);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
