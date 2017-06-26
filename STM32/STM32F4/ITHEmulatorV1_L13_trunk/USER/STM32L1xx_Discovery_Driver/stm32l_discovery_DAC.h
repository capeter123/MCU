 /**
  ******************************************************************************
  * @file    stm32l_discovery_DAC.h
  * @author   jiangst.fnst@cn.fujitsu.com
  * @version 
  * @date    April-2014
  * @brief   This file contains all the functions prototypes for the glass LCD
  *          firmware driver.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2014 FNST</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __stm32l_discovery_DAC
#define __stm32l_discovery_DAC
#include <stm32l1xx.h>
#include "Typedefs.h"

void DAC_InitConfig(void);
void DAC_OutputData(uint16_t data);

#endif
