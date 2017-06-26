#ifndef _MODEL_ERT_RTW_H_
#define _MODEL_ERT_RTW_H_

#include "stm32l1xx.h"
#include "engine_throttle.h"
#include "main.h"

#include "rtwtypes.h"
#include "Typedefs.h"

#define MAX3(a,b,c) ((a>b?a:b)>c?(a>b?a:b):c)

#define VDD 24  // (V)
//#define UPPER_OPEN_POS 107
//#define MOTOR_STOP_POS -17
//#define VALVE_STOP_POS 0
//#define ZEROSTUDY_PROTECT_DUTY 20
//#define VALID_DRIVER_DUTY 40

/* Private typedef -----------------------------------------------------------*/
typedef enum DRIVER_MODE
{
	INVALID_MODE = -1,
	DISABLE_MODE = 0,
	CLOSE_MODE,
	OPEN_MODE
}DRIVER_MODE;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Public methods -------------------------------------------------------------*/
void rt_InitConfig(void);
void rt_DisplayAndSendPos(void);
void rt_Model_ISRHandler(void);
void rt_Display_ISRHandler(void);
#endif
