#ifndef _TYPEDEFS_H__
#define _TYPEDEFS_H__
#include "stm32f4xx_hal.h"
#include "rtwtypes.h"

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
	__IO _Bool OM_EBM_1H;
	__IO _Bool OM_EBM_1L;
	__IO _Bool OM_EBM_2H;
	__IO _Bool OM_EBM_2L;
	__IO _Bool OM_EBM_3H;
	__IO _Bool OM_EBM_3L;
}BLDCMInputInfo;

typedef struct BLDCMOutputInfo
{
	boolean_T Habc[3];                     /* '<Root>/Hallx' */
  __IO int8_t postion_percent;              /* '<Root>/postion_percent' */
  int8_t egr_bldc_phase_fnl;           /* '<Root>/egr_bldc_phase_fnl' */
	int32_T rotor_rpm;
}BLDCMOutputInfo;

/*EGR CAN DEFINES*/
typedef enum
{
	EGRCAN_DISABLEMODE,
	EGRCAN_OPENMODE,
	EGRCAN_CLOSEMODE,
	EGRCAN_INVAIDMODE,
}EGRCANCTRLMode;

typedef enum
{
	EGRCAN_NO_ERR = 0x0,
	EGRCAN_RECOVETY_OK = 0x80,
	EGRCAN_RECOVERY_ERR = 0xC0,
	EGRCAN_TARGET_INVALID = 0x40
}EGRCANErrorType;

typedef struct EGRCANInputInfo
{
  uint8_t egrcan_target_pos;               /* '<Root>/vnt_target_pos' */
}EGRCANInputInfo;

typedef struct EGRCANOutputInfo
{
  real_T egrcan_actual_pos;               /* '<Root>/vnt_actual_pos' */
	real_T egrcan_actual_dc;
	EGRCANErrorType egrcan_error_msg;
}EGRCANOutputInfo;

typedef struct EGRISOCanRxMsgFrame
{
	uint8_t inf_egr_bldc_target_pos_1;  // Byte1
	
	uint8_t reservedByte2;	 					// Byte2
	uint8_t reservedByte3;	 					// Byte3
	uint8_t reservedByte4;	 					// Byte4
	uint8_t reservedByte5;	 					// Byte5

	uint8_t reservedByte6_b0:1;				// Byte6
	uint8_t engine_type:4;
	uint8_t inf_eng_cranking_canegr_stat:1; 
	uint8_t reservedByte8_b6:2;
	
	uint8_t reservedByte7;	 					// Byte7
	uint8_t reservedByte8;	 					// Byte8
}EGRISOCanRxMsgFrame;

typedef struct EGRISOCanTxMsgFrame
{
	uint8_t can_rqst_egr_act_pos_1;  // Byte 1
	 
	uint8_t can_rqst_egr_err_msg_1_hB; // Byte 2
	
	uint8_t can_rqst_egr_err_msg_1_lB; // Byte 3
	
	uint8_t can_rqst_egr_init_status_1:1; // Byte 4
	uint8_t reservedByte4:7;
	
	uint8_t can_rqst_egr_act_dc_1;		// Byte 5
	
	uint8_t can_rqst_egr_zpos_step_cnt_1; // Byte 6
	
	uint8_t can_rqst_egr_inside_temp_1;  // Byte 7
	
	uint8_t reservedByte8;
}EGRISOCanTxMsgFrame;

#endif
