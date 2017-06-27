#include "Task_Scheduler.h"
#include "EGR_BLL_Interface.h"
#include "CCP_CAN_Interface.h"

/********************************************************************************************************************/
/* EXTERN VARIABLE                                                                                                         	*/
/********************************************************************************************************************/
_Bool task_1ms_flag;
_Bool task_5ms_flag;
_Bool task_10ms_flag;
_Bool task_1000ms_flag;

uint32_t u32_count_1ms;
uint32_t u32_count_5ms;
uint32_t u32_count_10ms;
uint32_t u32_count_1000ms;

uint8_t ccp_receive_buf[8];

/* HAL System tick 1ms ISR Handler*/
void HAL_SYSTICK_Callback(void)
{
		/* 1ms event */
		task_1ms_flag = TASK_EXECUTE_ENABLE;
		
		/* 5ms event */
		++u32_count_5ms;
		if(u32_count_5ms >= 5)
		{
			task_5ms_flag = TASK_EXECUTE_ENABLE;
			u32_count_5ms = 0;
		}
		
		/* 10ms event */
		++u32_count_10ms;
		if(u32_count_10ms >= 10)
		{
			task_10ms_flag = TASK_EXECUTE_ENABLE;
			u32_count_10ms = 0;
		}
		
		/* 1000ms event */
		++u32_count_1000ms;
		if(u32_count_1000ms >= 1000)
		{
			task_1000ms_flag = TASK_EXECUTE_ENABLE;
			u32_count_1000ms = 0;
		}
}

void Task_Scheduler(void)
{
	/********************************************************************
	*>> 1ms TASK
	*******************************************************************/
	if(TASK_EXECUTE_ENABLE == task_1ms_flag)
	{
		// TODO 1ms task.
		task_1ms_flag = TASK_EXECUTE_DISABLE;
		// DAQ0 Task.
		//ccpDaq(CCP_DAQ_EVENT_NO);
	}
	/********************************************************************
	*>> 5ms TASK
	*******************************************************************/
	if(TASK_EXECUTE_ENABLE == task_5ms_flag)
	{
		// TODO 5ms task.
		task_5ms_flag = TASK_EXECUTE_DISABLE;
		ccpBackground ();
		if(ccpBootReceiveCro(ccp_receive_buf))
		{
			ccpCommand (ccp_receive_buf);
		}
		
		CCP_DISABLE_INTERRUPT;
		if (ccpBootTransmitCRMPossible ())
		{
			ccpSendCallBack ();
		}

		CCP_ENABLE_INTERRUPT;
	}
	/********************************************************************
	*>> 10ms TASK
	*******************************************************************/
	if(TASK_EXECUTE_ENABLE == task_10ms_flag)
	{
		// TODO 10ms task - CCP Task.
		task_10ms_flag = TASK_EXECUTE_DISABLE;
	}
	/********************************************************************
	*>> 1000ms TASK
	*******************************************************************/
	if(TASK_EXECUTE_ENABLE == task_10ms_flag)
	{	
		//TODO 1s task
		task_1000ms_flag = TASK_EXECUTE_DISABLE;
	}
}
