#include "EGR_BLL_Interface.h"

extern PWMInfo PWM_CCP_CH1;	// TIM2_CH1 -- PA15
extern PWMInfo PWM_CCP_CH2; //  TIM12_CH1 -- PB14
extern PWMInfo PWM_CCP_CH3; //  TIM9_CH1 -- PE5

/* EGR CAN Model define begin */
extern CAN_HandleTypeDef hcan1;
uint8_t EGR_CAN_Timeout_Flag = 0;
EGRCANInputInfo EGRCAN_InputInfo = {0};
EGRCANOutputInfo EGRCAN_OutputInfo = {0};
CanTxMsgTypeDef EGR_CANTxMsg;
CanRxMsgTypeDef EGR_CANRxMsg;
EGRISOCanRxMsgFrame egrcanISOCanRxMsgFrame = {0};
EGRISOCanTxMsgFrame egrcanISOCanTxMsgFrame = {0};
EGRCANCTRLMode egrcan_ctrl_mode = EGRCAN_INVAIDMODE;
int16_t inf_egrcan_target_pos_old = 0;

//static void EGR_CAN_ModelCalcPreprocess(void);
//static void EGR_CAN_Polling_CANInputInfo(EGRCANInputInfo *info);
//static EGRCANOutputInfo* EGR_CAN_GetModelOutputInfo(EGRCANOutputInfo* info);
//static void EGR_CAN_OutputPosInfoByCAN(EGRCANOutputInfo* info);
//static void EGR_CAN_UpdateDriverMode(void);
/* EGR CAN Model define end */

BLDCMInputInfo gBLDC_InputInfo_actual = {0};
BLDCMOutputInfo gBLDC_OutputInfo = {0};
PWMInfo gActivePWMchannel = {0};
//uint8_t gActivePWMchannelIdx = 0;
DisplayMode gDisplayMode = POSMODE;

int8_t gCurrent_Rotor_Phase = 0;
DRIVER_MODE g_currentDriveMode = DISABLE_MODE;
uint16_t u16PosPercent;
uint32_t u32PosPercent;
uint32_t dumpyRecv;
_Bool bValidDriveSignal = TRUE;
uint16_t recvBufDumpy; // SPIx recv buffer.

//BitAction bitPA9 = (BitAction)1; /* Task cost test code.*/

//uint32_t testTaskCounts = 10;
//uint32_t testActTaskCounts = 0;

const _Bool T_BLDC_CloseMode[6][6] =
{
	{1,	0,	0,	1,	0,	0},
	{1,	0,	0,	0,	0,	1},
	{0,	0,	1,	0,	0,	1},
	{0,	1,	1,	0,	0,	0},
	{0,	1,	0,	0,	1,	0},
	{0,	0,	0,	1,	1,	0}
};
const _Bool T_BLDC_OpenMode[6][6] =
{
	{0,	1,	1,	0,	0,	0},
	{0,	1,	0,	0,	1,	0},
	{0,	0,	0,	1,	1,	0},
	{1,	0,	0,	1,	0,	0},
	{1,	0,	0,	0,	0,	1},
	{0,	0,	1,	0,	0,	1}
};
const _Bool T_BLDC_SteadyMode[6][6] =
{
	{0,	0,	1,	0,	0,	1},
	{0,	1,	1,	0,	0,	0},
	{0,	1,	0,	0,	1,	0},
	{0,	0,	0,	1,	1,	0},
	{1,	0,	0,	1,	0,	0},
	{1,	0,	0,	0,	0,	1}
};

/* Private methods -------------------------------------------------------------*/
static void rt_OutputHall_GPIOInfo(const BLDCMOutputInfo* outputInfo);

static void rt_UpdateMotorMode(BLDCMInputInfo* inputInfoActual, int8_t motorCurPhase);
static _Bool rt_VerifyDriveMode(const _Bool (*T_BLDC_xMode)[6], 
								const BLDCMInputInfo* pInputInfo, 
								uint8_t currentRotorPhase,
								DRIVER_MODE currentDriverMode); 
static void rt_ModelCalcPreprocess(void);
static void rt_Polling_GPIOInputInfo(BLDCMInputInfo* info);
static BLDCMOutputInfo* rt_GetModelOutputInfo(BLDCMOutputInfo* info);
static void rt_CompensationSpringTorque(void);
static void rt_universalBridgeInverter(const BLDCMInputInfo* inputInfoPre,
																						 ExtU_power_BLDC_EGR_T* inputInfoAfter,
																						 uint8_t duty,
																						 DRIVER_MODE currentDriveMode, 
																						 int8_t current_rotor_phase);

void rt_InitConfig()
{
	/* EGR Model Initialize*/
	power_BLDC_EGR_initialize();
	power_BLDC_EGR_step();
	HAL_SPI_TransmitReceive_DMA(&hspi2, (uint8_t*)(&power_BLDC_EGR_Y.postion_percent), (uint8_t*)(&recvBufDumpy), 1);
	/* EGR CAN Model Initialize */
	//VNT_initialize();
}

void rt_Polling_GPIOInputInfo(BLDCMInputInfo* inputInfo)
{
	if(inputInfo)
	{
		// Add the period verification of PWM as the driver period offen is a fixed value.
		inputInfo->OM_EBM_1H = HAL_GPIO_ReadPin(GPIO_GROUP_EBM_1H, GPIO_PIN_EBM_1H);
		inputInfo->OM_EBM_1L = PWM_CCP_CH1.DutyCycle > 0;
		inputInfo->OM_EBM_2H = HAL_GPIO_ReadPin(GPIO_GROUP_EBM_2H, GPIO_PIN_EBM_2H);
		inputInfo->OM_EBM_2L = PWM_CCP_CH2.DutyCycle > 0;
		inputInfo->OM_EBM_3H = HAL_GPIO_ReadPin(GPIO_GROUP_EBM_3H, GPIO_PIN_EBM_3H);
		inputInfo->OM_EBM_3L = PWM_CCP_CH3.DutyCycle > 0;
	}
}

BLDCMOutputInfo* rt_GetModelOutputInfo(BLDCMOutputInfo* info)
{
	if(info)
	{
		memcpy(info->Habc, power_BLDC_EGR_Y.Hallx, sizeof(power_BLDC_EGR_Y.Hallx));
		info->postion_percent = power_BLDC_EGR_Y.postion_percent >> 7;
		info->egr_bldc_phase_fnl = power_BLDC_EGR_Y.egr_bldc_phase_fnl;
		info->rotor_rpm = power_BLDC_EGR_Y.rotor_rpm >> 10;
		
		gCurrent_Rotor_Phase = info->egr_bldc_phase_fnl;		
		u16PosPercent = (uint16_t)(info->postion_percent);
		u32PosPercent = (uint32_t)(info->postion_percent);
	}

	return info;
}

static _Bool rt_VerifyDriveMode(const _Bool (*T_BLDC_xMode)[6], 
								const BLDCMInputInfo* pInputInfo, 
								uint8_t curRotorPhase,
								DRIVER_MODE currentDriveMode)
{
	_Bool ret = FALSE;
	if(curRotorPhase>0 && curRotorPhase<=6)
	{
		ret = ((*(*(T_BLDC_xMode+(curRotorPhase-1))+0) == pInputInfo->OM_EBM_1H) &&
			(*(*(T_BLDC_xMode+(curRotorPhase-1))+2) == pInputInfo->OM_EBM_2H) &&
			(*(*(T_BLDC_xMode+(curRotorPhase-1))+4) == pInputInfo->OM_EBM_3H) &&
			(*(*(T_BLDC_xMode+(curRotorPhase-1))+1) == pInputInfo->OM_EBM_1L) &&
			(*(*(T_BLDC_xMode+(curRotorPhase-1))+3) == pInputInfo->OM_EBM_2L) &&
			(*(*(T_BLDC_xMode+(curRotorPhase-1))+5) == pInputInfo->OM_EBM_3L));
	}

	return ret;
}

void rt_OutputHall_GPIOInfo(const BLDCMOutputInfo* outputInfo)
{
	HAL_GPIO_WritePin(GPIO_GROUP_EBM_POS1, GPIO_PIN_EBM_POS1, (GPIO_PinState)(outputInfo->Habc[0]));
	HAL_GPIO_WritePin(GPIO_GROUP_EBM_POS2, GPIO_PIN_EBM_POS2, (GPIO_PinState)(outputInfo->Habc[1]));
	HAL_GPIO_WritePin(GPIO_GROUP_EBM_POS3, GPIO_PIN_EBM_POS3, (GPIO_PinState)(outputInfo->Habc[2]));
}

static void rt_UpdateMotorMode(BLDCMInputInfo* inputInfoActual, int8_t motorCurPhase)
{
	//Determine the current drive mode take advantage of duty and input level senquences.
	g_currentDriveMode = INVALID_MODE;	
	if(!(inputInfoActual->OM_EBM_1H & inputInfoActual->OM_EBM_2H & inputInfoActual->OM_EBM_3H))
	{
		if(RESET == (inputInfoActual->OM_EBM_1H | inputInfoActual->OM_EBM_2H | inputInfoActual->OM_EBM_3H))
		{
			if(RESET == (inputInfoActual->OM_EBM_1L | inputInfoActual->OM_EBM_2L | inputInfoActual->OM_EBM_3L))
			{ //Brake or Disable mode.
				g_currentDriveMode = PWM_CCP_CH1.DutyCycle>0?BRAKE_MODE:DISABLE_MODE; // It is be noted that not limited the Channel 1 here, which can also be instead in CH2 or CH3.
			}
			else if(SET == (inputInfoActual->OM_EBM_1L & inputInfoActual->OM_EBM_2L & inputInfoActual->OM_EBM_3L))
			{
				//g_currentDriveMode = BRAKE_MODE; // remove break mode.
				g_currentDriveMode = DISABLE_MODE;
			}
		}
		else if(SET == (inputInfoActual->OM_EBM_1H ^ inputInfoActual->OM_EBM_2H ^ inputInfoActual->OM_EBM_3H))
		{
			// Committed by jst because it will result P/O intersect display as a lower duty, so cancle steady mode.
			if(rt_VerifyDriveMode(T_BLDC_SteadyMode, inputInfoActual, motorCurPhase, STEADY_MODE))
			{
				g_currentDriveMode = STEADY_MODE;
			}
			
			if(rt_VerifyDriveMode(T_BLDC_OpenMode, inputInfoActual, motorCurPhase, OPEN_MODE))
			{
				g_currentDriveMode = OPEN_MODE;
			}
			else if(rt_VerifyDriveMode(T_BLDC_CloseMode, inputInfoActual, motorCurPhase, CLOSE_MODE))
			{
				g_currentDriveMode = CLOSE_MODE;
			}
		}
		
		gActivePWMchannel.DutyCycle = MAX3(PWM_CCP_CH1.DutyCycle, PWM_CCP_CH2.DutyCycle, PWM_CCP_CH3.DutyCycle);
	}
}

static void rt_ModelCalcPreprocess()
{
	bValidDriveSignal = TRUE;
	//power_BLDC_EGR_U.OM_EBM1 = power_BLDC_EGR_U.OM_EBM2 = power_BLDC_EGR_U.OM_EBM3 = 0;
	switch(g_currentDriveMode)
	{
		case OPEN_MODE:
			if(gBLDC_OutputInfo.postion_percent >= UPPER_OPEN_POS && gActivePWMchannel.DutyCycle>=20)
				bValidDriveSignal = FALSE;
			else
			{
				rt_universalBridgeInverter(&gBLDC_InputInfo_actual, 
																	 &power_BLDC_EGR_U, 
																	 gActivePWMchannel.DutyCycle, 
																	 g_currentDriveMode,
																	 gCurrent_Rotor_Phase);
				/* Follow code is a prevention measures for the zero position study process. */
				if(gActivePWMchannel.DutyCycle <= VALID_DRIVER_DUTY && gBLDC_OutputInfo.postion_percent >= VALVE_STOP_POS)
					power_BLDC_EGR_U.OM_EBM1 = power_BLDC_EGR_U.OM_EBM2 = power_BLDC_EGR_U.OM_EBM3 = 0;
			}
			break;
		
		case STEADY_MODE:
		case INVALID_MODE:
			bValidDriveSignal = FALSE;
			break;
		
		default:	//CLOSE_MODE, BRAKE_MODE and DISABLE_MODE.
			if(gBLDC_OutputInfo.postion_percent <= MOTOR_STOP_POS)
				bValidDriveSignal = FALSE;
			else
			{
				rt_universalBridgeInverter(&gBLDC_InputInfo_actual, 
																		 &power_BLDC_EGR_U, 
																		 gActivePWMchannel.DutyCycle, 
																		 g_currentDriveMode, 
																		 gCurrent_Rotor_Phase);
			}
			
			break;
	}
	
	rt_CompensationSpringTorque();
}

void rt_CompensationSpringTorque()
{
	if(DISABLE_MODE == g_currentDriveMode && gBLDC_OutputInfo.postion_percent>VALVE_STOP_POS + 5)
	{
		power_BLDC_EGR_P.SpringCom = 30<<4;
	}
	else
		power_BLDC_EGR_P.SpringCom = 0;
//	if(g_currentDriveMode == BRAKE_MODE )
//	{
//		power_BLDC_EGR_P.SpringCom = (gBLDC_OutputInfo.rotor_rpm<<10) >>3;
//	}
//	else
//		power_BLDC_EGR_P.SpringCom = 0;
}

static void rt_universalBridgeInverter(const BLDCMInputInfo* inputInfoPre,
																						 ExtU_power_BLDC_EGR_T* inputInfoAfter,
																						 uint8_t duty,
																						 DRIVER_MODE currentDriveMode, 
																						 int8_t current_rotor_phase)
{
	duty = duty>100?100:duty;
	switch(currentDriveMode)
	{
		case OPEN_MODE:
			switch(current_rotor_phase)
			{
				case 1:
					inputInfoAfter->OM_EBM1 = VDD*duty;
					inputInfoAfter->OM_EBM2 = 0;
					inputInfoAfter->OM_EBM3 = VDD*duty>>1;
					break;
				
				case 2:
					inputInfoAfter->OM_EBM1 = VDD*duty;
					inputInfoAfter->OM_EBM2 = VDD*duty>>1;
					inputInfoAfter->OM_EBM3 = 0;
					break;
				
				case 3:
					inputInfoAfter->OM_EBM1 = VDD*duty>>1;
					inputInfoAfter->OM_EBM2 = VDD*duty;
					inputInfoAfter->OM_EBM3 = 0;
					break;
				
				case 4:
					inputInfoAfter->OM_EBM1 = 0;
					inputInfoAfter->OM_EBM2 = VDD*duty;
					inputInfoAfter->OM_EBM3 = VDD*duty>>1;
					break;
				
				case 5:
					inputInfoAfter->OM_EBM1 = 0;
					inputInfoAfter->OM_EBM2 = VDD*duty>>1;
					inputInfoAfter->OM_EBM3 = VDD*duty;
					break;
				
				case 6:
					inputInfoAfter->OM_EBM1 = VDD*duty>>1;
					inputInfoAfter->OM_EBM2 = 0;
					inputInfoAfter->OM_EBM3 = VDD*duty;
					break;
			}
			break;
		
		case CLOSE_MODE:
			switch(current_rotor_phase)
			{
				case 1:
					inputInfoAfter->OM_EBM1 = 0;
					inputInfoAfter->OM_EBM2 = VDD*duty;
					inputInfoAfter->OM_EBM3 = VDD*duty>>1;
					break;
				
				case 2:
					inputInfoAfter->OM_EBM1 = 0;
					inputInfoAfter->OM_EBM2 = VDD*duty>>1;
					inputInfoAfter->OM_EBM3 = VDD*duty;
					break;
				
				case 3:
					inputInfoAfter->OM_EBM1 = VDD*duty>>1;
					inputInfoAfter->OM_EBM2 = 0;
					inputInfoAfter->OM_EBM3 = VDD*duty;
					break;
				
				case 4:
					inputInfoAfter->OM_EBM1 = VDD*duty;
					inputInfoAfter->OM_EBM2 = 0;
					inputInfoAfter->OM_EBM3 = VDD*duty>>1;
					break;
				
				case 5:
					inputInfoAfter->OM_EBM1 = VDD*duty;
					inputInfoAfter->OM_EBM2 = VDD*duty>>1;
					inputInfoAfter->OM_EBM3 = 0;
					break;
				
				case 6:
					inputInfoAfter->OM_EBM1 = VDD*duty>>1;
					inputInfoAfter->OM_EBM2 = VDD*duty;
					inputInfoAfter->OM_EBM3 = 0;
					break;
			}
			break;
		
		case BRAKE_MODE:
			inputInfoAfter->OM_EBM1 = inputInfoAfter->OM_EBM2 = inputInfoAfter->OM_EBM3 = VDD*duty>>1;
			break;
		
		default:
			inputInfoAfter->OM_EBM1 = inputInfoAfter->OM_EBM2 = inputInfoAfter->OM_EBM3 = 0;
			break;
	}
	
	inputInfoAfter->OM_EBM1 = (inputInfoAfter->OM_EBM1 << 10)/100;
	inputInfoAfter->OM_EBM2 = (inputInfoAfter->OM_EBM2 << 10)/100;
	inputInfoAfter->OM_EBM3 = (inputInfoAfter->OM_EBM3 << 10)/100;
}

void rt_DisplayAndSendPos(void)
{
//#ifndef USE_DMA_TxRx
//	SPIx_IRQ_RWBytes(&u16PosPercent, 1, SET);
//#endif
}

void rt_Model_ISRHandler(void)
{
//	if(gBLDC_OutputInfo.postion_percent<100)
//			++testTaskCounts;
	
	rt_Polling_GPIOInputInfo(&gBLDC_InputInfo_actual);
	rt_UpdateMotorMode(&gBLDC_InputInfo_actual, gCurrent_Rotor_Phase);
	rt_ModelCalcPreprocess();
	if(bValidDriveSignal)
	{
		power_BLDC_EGR_step();
//		if(gBLDC_OutputInfo.postion_percent<100)
//				++testActTaskCounts;
	}
		
	rt_GetModelOutputInfo(&gBLDC_OutputInfo);
	rt_OutputHall_GPIOInfo(&gBLDC_OutputInfo);
	
//	bitPA9 ^= 1;
//	GPIO_WriteBit(GPIOA, GPIO_Pin_9, bitPA9);
}

/*-------------------------------------------------*
**---- EGR CAN Model logic private functions-------*
**------------------------------------------------*/
/*-------------------------------------------------*
**---- EGR CAN Model logic private functions-------*
**------------------------------------------------*/
//void EGR_CAN_DisplayAndSendPos(void)
//{
//	HAL_CAN_Transmit(&EGR_CAN_HANDLE, 10);
//}

//void EGR_CAN_DaemonTask(void)
//{
//	if(EGR_CAN_Timeout_Flag == SET)
//	{
//		memset(EGR_CANRxMsg.Data, 0, EGR_CANRxMsg.DLC);
//		return;
//	}
//	
//	EGR_CAN_Timeout_Flag = SET;
//}

//void EGR_CAN_Model_ISRHandler(void)
//{
//	EGR_CAN_Polling_CANInputInfo(&EGRCAN_InputInfo);
//	EGR_CAN_UpdateDriverMode();
//	EGR_CAN_ModelCalcPreprocess();
//	if(egrcan_ctrl_mode != EGRCAN_INVAIDMODE)
//	{
//		EGR_CAN_step();
//	}
//	
//	EGR_CAN_GetModelOutputInfo(&EGRCAN_OutputInfo);
//	EGR_CAN_OutputPosInfoByCAN(&EGRCAN_OutputInfo);
//}

//static EGRCANOutputInfo* EGR_CAN_GetModelOutputInfo(EGRCANOutputInfo* info)
//{
//	if(info)
//	{
//		info->egrcan_actual_pos = egr_can_act_pos;
//		info->egrcan_actual_dc = egr_can_act_dc;
//	}
//	
//	inf_egrcan_target_pos_old = EGRCAN_InputInfo.egrcan_target_pos;
//	/* Determination the error msg type */
//	switch(egrcan_ctrl_mode)
//	{
//		case EGRCAN_CLOSEMODE:
//			info->egrcan_error_msg = EGRCAN_RECOVETY_OK;
//			break;
//		case EGRCAN_INVAIDMODE:
//			info->egrcan_error_msg = EGRCAN_TARGET_INVALID;
//			break;
//		default:
//			info->egrcan_error_msg = EGRCAN_NO_ERR;
//			break;
//	}
//	
//	return info;
//}

//static void EGR_CAN_Polling_CANInputInfo(EGRCANInputInfo *info)
//{
//#ifdef USE_STD_CAN
//	memcpy(&egrcanISOCanRxMsgFrame, EGR_CANRxMsg.Data, EGR_CANRxMsg.DLC);
//	info->egrcan_target_pos = egrcanISOCanRxMsgFrame.inf_egr_bldc_target_pos_1;
//#else
////		memcpy(&vntPosJ1939CanRxMsgFrame, EGR_CANRxMsg.Data, EGR_CANRxMsg.DLC);
////		info->vnt_target_pos = ((uint16_t)(vntPosJ1939CanRxMsgFrame.inf_trb_trg_pos_hb & 0x03)<<8 | vntPosJ1939CanRxMsgFrame.inf_trb_trg_pos_lb)*0.1f;
//#endif		
//}

//static void EGR_CAN_ModelCalcPreprocess()
//{
//	egr_can_target_pos = EGRCAN_InputInfo.egrcan_target_pos;
//}

//static void EGR_CAN_OutputPosInfoByCAN(EGRCANOutputInfo* info)
//{
//	/*Packed EGR CAN Data info CAN data frame.*/
//	egrcanISOCanTxMsgFrame.can_rqst_egr_act_dc_1 = (uint8_t)info->egrcan_actual_dc;
//	egrcanISOCanTxMsgFrame.can_rqst_egr_act_pos_1 = (uint8_t)info->egrcan_actual_pos;
//	//egrcanISOCanTxMsgFrame.can_rqst_egr_inside_temp_1 = ubSOCTemp;
//	egrcanISOCanTxMsgFrame.can_rqst_egr_err_msg_1_hB = (uint8_t)((info->egrcan_error_msg & 0xFF00) >> 8);
//	egrcanISOCanTxMsgFrame.can_rqst_egr_err_msg_1_lB = (uint8_t)(info->egrcan_error_msg & 0x00FF);
//	egrcanISOCanTxMsgFrame.can_rqst_egr_init_status_1 = 0;
//	egrcanISOCanTxMsgFrame.can_rqst_egr_zpos_step_cnt_1 = 0x1C;
//	
//	EGR_CANTxMsg.DLC = CAN_RXMSG_DLC;
//	EGR_CANTxMsg.RTR = CAN_RTR_DATA;
//#ifdef USE_STD_CAN
//	EGR_CANTxMsg.StdId = CCP_DTO_ID;
//	EGR_CANTxMsg.IDE = CAN_ID_STD;
//	memcpy(EGR_CANTxMsg.Data, &egrcanISOCanTxMsgFrame, CAN_RXMSG_DLC);
//#else /*USE_CAN2 J1939*/

//#endif	
//}

//static void EGR_CAN_UpdateDriverMode(void)
//{
//	/* 2. Determination the egrcan control mode */
//	int16_t pos_diff = (int16_t)EGRCAN_InputInfo.egrcan_target_pos - inf_egrcan_target_pos_old;
//	if(EGRCAN_InputInfo.egrcan_target_pos >= K_EGR_CAN_TARGET_POS_MAX)
//	{
//		egrcan_ctrl_mode = EGRCAN_INVAIDMODE;
//		return;
//	}
//	
//	if(pos_diff >= 0)
//	{
//		if(pos_diff == 0)
//		{
//			egrcan_ctrl_mode = EGRCAN_DISABLEMODE;
//		}
//		else
//		{
//			egrcan_ctrl_mode = EGRCAN_OPENMODE;
//		}
//	}
//	else
//	{
//			egrcan_ctrl_mode = EGRCAN_CLOSEMODE;
//	}
//}
