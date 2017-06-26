#include "Model_ert_rtw.h"

extern PWMInfo PWM_CCP_CH1; // TIM2_CH1 -- PA(00)
extern PWMInfo PWM_CCP_CH2; // TIM4_CH2 -- PB(07)

ITHInputInfo gITH_InputInfo_actual = {0};
ITHOutputInfo gITH_OutputInfo = {0};
PWMInfo gActivePWMchannel = {0};
uint8_t gActivePWMchannelIdx = 0;
DisplayMode gDisplayMode = POSMODE;
DRIVER_MODE g_currentDriveMode = DISABLE_MODE;
_Bool bValidDriveSignal = TRUE;
uint8_t u8PosPercent;

/* Private methods -------------------------------------------------------------*/
static void MODEL_RCC_Config(void);
static void TIM10_NVIC_Config(void);
static void TIM10_Mode_Config(void);

static void rt_OutputPosDAInfo(const ITHOutputInfo* outputInfo);
static void rt_UpdateMotorMode(ITHInputInfo* inputInfoActual);
static void rt_ModelCalcPreprocess(void);
static void rt_Polling_GPIOInputInfo(ITHInputInfo* info);
static ITHOutputInfo* rt_GetModelOutputInfo(ITHOutputInfo* info);
static void rt_universalBridgeInverter(const ITHInputInfo* inputInfoPre,
																									 ExtU_engine_throttle_T* inputInfoAfter,
																									 uint8_t duty,
																									 DRIVER_MODE currentDriveMode);
//static void rt_CompensationSpringTorque(void);

void rt_InitConfig()
{
	engine_throttle_initialize();
	MODEL_RCC_Config();
	TIM10_NVIC_Config();
	TIMx_CCP_InitConfig(); // Config PWM capture registers.
	DAC_InitConfig();
	TIM10_Mode_Config();
}

void MODEL_RCC_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
}

void TIM10_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM10_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 32;
	TIM_TimeBaseInitStructure.TIM_Period = 10; // Timer task period = 1e-5s for systemCoreFreq = 32MHz. 
	TIM_TimeBaseInit(TIM10, &TIM_TimeBaseInitStructure);

	TIM_ITConfig(TIM10, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM10, ENABLE);
}

void rt_Polling_GPIOInputInfo(ITHInputInfo* inputInfo)
{
	if(inputInfo)
	{
		// Add the period verification of PWM as the driver period offen is a fixed value.
		inputInfo->OM_IDM1 = PWM_CCP_CH1;
		inputInfo->OM_IDM2 = PWM_CCP_CH2;
	}
}

ITHOutputInfo* rt_GetModelOutputInfo(ITHOutputInfo* info)
{
	if(info)
	{
		info->ith_pos = (int8_t)(engine_throttle_Y.ith_pos >> 24);
		info->ith_degree = (int8_t)(engine_throttle_Y.ith_degree >> 24);
		u8PosPercent = (uint8_t)(info->ith_pos);
		info->ith_pos_da =  engine_throttle_Y.ith_pos_da>>4; /* DA value saturation limitation.*/
	}

	return info;
}

void rt_OutputPosDAInfo(const ITHOutputInfo* outputInfo)
{
	DAC_OutputData(outputInfo->ith_pos_da);
}

static void rt_UpdateMotorMode(ITHInputInfo* inputInfoActual)
{
	//Determine the current drive mode take advantage of duty and input level senquences.
	g_currentDriveMode = DISABLE_MODE;
	gActivePWMchannel.DutyCycle = gActivePWMchannel.Frequency = 0;
	gActivePWMchannelIdx = 0;
	if((inputInfoActual->OM_IDM1).DutyCycle>0 && (inputInfoActual->OM_IDM2).DutyCycle==0)
	{
		g_currentDriveMode = OPEN_MODE;
		gActivePWMchannel = PWM_CCP_CH1;
		gActivePWMchannelIdx = 1;
	}
	else if((inputInfoActual->OM_IDM1).DutyCycle==0 && (inputInfoActual->OM_IDM2).DutyCycle>0)
	{
		g_currentDriveMode = CLOSE_MODE;
		gActivePWMchannel = PWM_CCP_CH2;
		gActivePWMchannelIdx = 2;
	}
}

static void rt_universalBridgeInverter(const ITHInputInfo* inputInfoPre,
																									 ExtU_engine_throttle_T* inputInfoAfter,
																									 uint8_t duty,
																									 DRIVER_MODE currentDriveMode)
{
	switch(currentDriveMode)
	{
		case CLOSE_MODE:
			inputInfoAfter->U = (VDD*duty<<7)/100;
			break;
		case OPEN_MODE:
			inputInfoAfter->U = -(VDD*duty<<7)/100;
			break;
		default:
			break;
	}
}

static void rt_ModelCalcPreprocess()
{
	bValidDriveSignal = TRUE;
	//engine_throttle_U.U = 0;
	switch(g_currentDriveMode)
	{
		case OPEN_MODE:
		case CLOSE_MODE:
			rt_universalBridgeInverter(&gITH_InputInfo_actual, 
																					&engine_throttle_U, 
																					gActivePWMchannel.DutyCycle, 
																					g_currentDriveMode);
			break;
				
		case INVALID_MODE:
			bValidDriveSignal = FALSE;
			break;
		
		default:
			engine_throttle_U.U = 0;
			break;
	}
	
	//rt_CompensationSpringTorque();
}

//static void rt_CompensationSpringTorque(void)
//{
//	engine_throttle_P.springTorqueCompensation = 0;
//	switch(g_currentDriveMode)
//	{
//		case DISABLE_MODE: // The reason of only deposed DISABLE_MODE is accelerate the ITH throttle return.
//			if(gITH_OutputInfo.ith_pos > 0)
//				engine_throttle_P.springTorqueCompensation = 10; // It shold pay attention to that 50 may is not a ideal value.
//			break;
//		default:
//			break;
//	}
//}

void rt_DisplayAndSendPos(void)
{
	if(POSMODE == gDisplayMode)
	{
		switch(g_currentDriveMode)
		{
		case CLOSE_MODE:
			LCD_GLASS_Printf("C:%3d",gITH_OutputInfo.ith_pos);
			break;
		case OPEN_MODE:
			LCD_GLASS_Printf("O:%3d",gITH_OutputInfo.ith_pos);
			break;
		default: 
			LCD_GLASS_Printf("D:%3d",gITH_OutputInfo.ith_pos);
			break;
		}
	}
	else
	{
		LCD_GLASS_Printf("*:%3d", gActivePWMchannel.DutyCycle);
	}

	SPIx_IRQ_RWBytes(&u8PosPercent, 1, SET);
	//SPIx_DMA_RWData(&u32PosPercent, &dumpyRecv);
}

void rt_Model_ISRHandler(void)
{
	if(TIM_GetITStatus(TIM10,TIM_IT_Update) == SET)
	{	
		rt_Polling_GPIOInputInfo(&gITH_InputInfo_actual);
		rt_UpdateMotorMode(&gITH_InputInfo_actual);
		rt_ModelCalcPreprocess();
		if(bValidDriveSignal)
		{
			engine_throttle_step();
		}
		
		rt_GetModelOutputInfo(&gITH_OutputInfo);
		rt_OutputPosDAInfo(&gITH_OutputInfo);
		
		TIM_ClearITPendingBit(TIM10, TIM_IT_Update);
	}
}
