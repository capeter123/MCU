#ifndef CCP_CAN_INTERFACE_H_
#define CCP_CAN_INTERFACE_H_
#include "stm32f4xx_hal.h"
#include "ccp.h"
#include "canstat.h"
#include "queue.h"

#define USE_STD_CAN
//#define USE_EXT_CAN

#define EGR_CAN_INTERFACE CAN1
#define CCP_CAN_INTERFACE CAN2
#define EGR_CAN_HANDLE		hcan1
#define CCP_CAN_HANDLE 		hcan2

#define CAN_RXMSG_DLC 8	
#define CANFilter_ID_STD(can_id) (((u32)(can_id << 5) << 16) |  ((CAN_ID_STD | CAN_RTR_DATA | 0x0)&0xFFFF))
#define CANFilter_Mask_STD (((u32)(0x7FF << 5) << 16) |  (0x06&0xFFFF))
/* CAN Indication LED */
#define CAN_LED_GPIO GPIOD
#define CAN_COM_LED_PIN GPIO_PIN_13
#define CAN_BUSON_LED_PIN GPIO_PIN_15

#define CAN_Filter_FIFOx CAN_FilterFIFO0

#ifdef USE_STD_CAN
/*Standard CAN Filter ID Demo*/
#define CCP_CAN_ID 0x120  /* Standard CAN ID */
#define CCP_CAN_FILTER_DEFUALT_ID (((uint32_t)(CCP_CRO_ID << 5) << 16) |  ((CAN_ID_STD | CAN_RTR_DATA | 0x0)&0xFFFF))
#define CCP_CAN_FILTER_DEFUALT_MASK (((uint32_t)(0x7FF << 5) << 16) |  (0x06&0xFFFF))

#define EGR_CAN_STD_ID 0x1BF
#define EGR_CAN_FILTER_DEFUALT_ID (((uint32_t)(EGR_CAN_STD_ID << 5) << 16) |  ((CAN_ID_STD | CAN_RTR_DATA | 0x0)&0xFFFF))
#define EGR_CAN_FILTER_DEFUALT_MASK (((uint32_t)(0x7FF << 5) << 16) |  (0x06&0xFFFF))

#else
	#define EGR_CAN_EXT_ID 0x18FF1002
/*Extend CAN Filter ID Demo*/
//#define CAN_ID 0x18FF1000U  /* Standard CAN ID */
//#define CAN_FILTER_DEFUALT_ID ((CAN_ID << 0x3) | CAN_ID_EXT | CAN_RTR_DATA | 0x0)
//#define CAN_FILTER_DEFUALT_MASK (0xFFFFFFFF)
#endif
/* USER CODE END Define */

extern void ccpBootInit(CAN_TypeDef*, int cro_id, int cto_id);
extern int ccpBootTransmitCRMPossible(void);
extern void ccpBootTransmitCRM(unsigned char* msg);
extern int ccpBootReceiveCro(unsigned char* msg);

extern canStatus canRead(CAN_TypeDef* canType, CanRxMsgTypeDef* pRxMessage);
extern canStatus canReadWait(CAN_TypeDef* canType, CanRxMsgTypeDef* pRxMessage, uint32_t  timeout);
extern canStatus canWrite(CAN_HandleTypeDef *canType, CanTxMsgTypeDef* pTxMessage);

//CCP_BYTE ccpSend(CCP_BYTEPTR msg);
CCP_MTABYTEPTR ccpGetPointer(CCP_BYTE addr_ext, CCP_DWORD addr);
void ccpUserBackground(void);
void disable_interrupt(void);
void enable_interrupt(void);
void TIM10_ISR_CCP_DAQ_1ms_Task(void);
#endif
