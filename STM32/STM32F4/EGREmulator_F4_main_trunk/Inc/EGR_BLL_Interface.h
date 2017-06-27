#ifndef _EGR_BLL_INTERFACE_H_
#define _EGR_BLL_INTERFACE_H_

#include "power_BLDC_EGR.h"
#include "CCP_CAN_Interface.h"
#include "rtwtypes.h"
#include "Typedefs.h"
#include "stm32f4_discovery.h"
#include "spi.h"
#include <string.h>

#define MAX3(a,b,c) ((a>b?a:b)>c?(a>b?a:b):c)

#define VDD 48  // (V)
#define UPPER_OPEN_POS 125
#define MOTOR_STOP_POS -25
#define VALVE_STOP_POS 2
#define VALID_DRIVER_DUTY 16

#define TIMx_CLK_FREQ (uint32_t)2000000 //Config the TIM Base Freqence is 2MHz.
#define VALID_PWM_FREQ_LOWER 40
#define VALID_PWM_FREQ_UPPER 510
/* Private typedef -----------------------------------------------------------*/
typedef enum DRIVER_MODE
{
	INVALID_MODE = -1,
	DISABLE_MODE = 0,
	CLOSE_MODE,
	OPEN_MODE,
	STEADY_MODE,
	BRAKE_MODE
}DRIVER_MODE;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Public methods -------------------------------------------------------------*/
void rt_InitConfig(void);
void rt_DisplayAndSendPos(void);
void rt_Model_ISRHandler(void);

/* EGR_CAN Model Interface */
void EGR_CAN_InitConfig(void);
void EGR_CAN_DisplayAndSendPos(void);
void EGR_CAN_DaemonTask(void);
void EGR_CAN_Model_ISRHandler(void);
#endif
