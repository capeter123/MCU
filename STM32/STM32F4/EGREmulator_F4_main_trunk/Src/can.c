/**
  ******************************************************************************
  * File Name          : CAN.c
  * Date               : 30/10/2015 09:49:40
  * Description        : This file provides code for the configuration
  *                      of the CAN instances.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "can.h"

#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "CCP_CAN_Interface.h"

canStatus __IO canComStatus = canERR_RESERVED;
CircleQueue CANRxMsgQueue;
CanTxMsgTypeDef CCP_CANTxMsg;
CanRxMsgTypeDef CCP_CANRxMsg;
uint32_t ccp_dto_id;
uint32_t ccp_cro_id;
/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 6;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SJW = CAN_SJW_2TQ;
  hcan1.Init.BS1 = CAN_BS1_6TQ;
  hcan1.Init.BS2 = CAN_BS2_7TQ;
  hcan1.Init.TTCM = DISABLE;
  hcan1.Init.ABOM = ENABLE;
  hcan1.Init.AWUM = DISABLE;
  hcan1.Init.NART = DISABLE;
  hcan1.Init.RFLM = DISABLE;
  hcan1.Init.TXFP = DISABLE;
  HAL_CAN_Init(&hcan1);

}
/* CAN2 init function */
void MX_CAN2_Init(void)
{

  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 6;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SJW = CAN_SJW_2TQ;
  hcan2.Init.BS1 = CAN_BS1_6TQ;
  hcan2.Init.BS2 = CAN_BS2_7TQ;
  hcan2.Init.TTCM = DISABLE;
  hcan2.Init.ABOM = ENABLE;
  hcan2.Init.AWUM = DISABLE;
  hcan2.Init.NART = DISABLE;
  hcan2.Init.RFLM = DISABLE;
  hcan2.Init.TXFP = DISABLE;
  HAL_CAN_Init(&hcan2);

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hcan->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */
		CAN_FilterConfTypeDef sFilterConfig;
		hcan->pRxMsg = &CCP_CANRxMsg;
		hcan->pTxMsg = &CCP_CANTxMsg;
  /* USER CODE END CAN1_MspInit 0 */
    /* Peripheral clock enable */
    __CAN1_CLK_ENABLE();
  
    /**CAN1 GPIO Configuration    
    PD0     ------> CAN1_RX
    PD1     ------> CAN1_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 1, 2);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */
		/*# Configure the CAN Filter#*/
		sFilterConfig.FilterNumber = 0;
		sFilterConfig.BankNumber = 14;
		sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
		sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
		sFilterConfig.FilterFIFOAssignment = CAN_FilterFIFO0;
		sFilterConfig.FilterActivation = ENABLE;
		sFilterConfig.FilterIdHigh = EGR_CAN_FILTER_DEFUALT_ID >> 16;
		sFilterConfig.FilterIdLow = EGR_CAN_FILTER_DEFUALT_ID & 0xFFFF;
		sFilterConfig.FilterMaskIdHigh = EGR_CAN_FILTER_DEFUALT_MASK >> 16;
		sFilterConfig.FilterMaskIdLow = EGR_CAN_FILTER_DEFUALT_MASK & 0xFFFF;
		HAL_CAN_ConfigFilter(hcan, &sFilterConfig);
  /* USER CODE END CAN1_MspInit 1 */
  }
  else if(hcan->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspInit 0 */
		CAN_FilterConfTypeDef sFilterConfig;
		hcan->pRxMsg = &CCP_CANRxMsg;
		hcan->pTxMsg = &CCP_CANTxMsg;
  /* USER CODE END CAN2_MspInit 0 */
    /* Peripheral clock enable */
    __CAN2_CLK_ENABLE();
    __CAN1_CLK_ENABLE();
  
    /**CAN2 GPIO Configuration    
    PB5     ------> CAN2_RX
    PB6     ------> CAN2_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 1, 3);
    HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
  /* USER CODE BEGIN CAN2_MspInit 1 */
		/*# Configure the CAN Filter#*/
		sFilterConfig.FilterNumber = 14;
		sFilterConfig.BankNumber = 14;
		sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
		sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
		sFilterConfig.FilterFIFOAssignment = CAN_FilterFIFO0;
		sFilterConfig.FilterActivation = ENABLE;
		sFilterConfig.FilterIdHigh = CCP_CAN_FILTER_DEFUALT_ID >> 16;
		sFilterConfig.FilterIdLow = CCP_CAN_FILTER_DEFUALT_ID & 0xFFFF;
		sFilterConfig.FilterMaskIdHigh = CCP_CAN_FILTER_DEFUALT_MASK >> 16;
		sFilterConfig.FilterMaskIdLow = CCP_CAN_FILTER_DEFUALT_MASK & 0xFFFF;
		HAL_CAN_ConfigFilter(hcan, &sFilterConfig);
  /* USER CODE END CAN2_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* hcan)
{

  if(hcan->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __CAN1_CLK_DISABLE();
  
    /**CAN1 GPIO Configuration    
    PD0     ------> CAN1_RX
    PD1     ------> CAN1_TX 
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0|GPIO_PIN_1);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);

  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
  else if(hcan->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspDeInit 0 */

  /* USER CODE END CAN2_MspDeInit 0 */
    /* Peripheral clock disable */
    __CAN2_CLK_DISABLE();
    __CAN1_CLK_DISABLE();
  
    /**CAN2 GPIO Configuration    
    PB5     ------> CAN2_RX
    PB6     ------> CAN2_TX 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5|GPIO_PIN_6);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(CAN2_RX0_IRQn);

  /* USER CODE BEGIN CAN2_MspDeInit 1 */

  /* USER CODE END CAN2_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
/* CCP - CAN Interface function*/
void ccpBootInit(CAN_TypeDef* can_type, int cro_id, int dto_id)
{
	ccp_cro_id = cro_id;
	ccp_dto_id = dto_id;
	InitQueue(&CANRxMsgQueue); /* New a queue for saving the received messages. */
}

int ccpBootTransmitCRMPossible(void)
{
	return (__HAL_CAN_GET_FLAG(&CCP_CAN_HANDLE,CAN_TSR_TME) == TRUE);
}
	
void ccpBootTransmitCRM(unsigned char* msg)
{
	memcpy(CCP_CANTxMsg.Data, msg, 8);
	CCP_CANTxMsg.DLC = 8;
	CCP_CANTxMsg.IDE = CAN_ID_STD;
	CCP_CANTxMsg.RTR = CAN_RTR_DATA;
	CCP_CANTxMsg.StdId = ccp_dto_id;
	HAL_CAN_Transmit(&CCP_CAN_HANDLE, 0);
}

int ccpBootReceiveCro(unsigned char* msg)
{
	canStatus s = canRead(CCP_CAN_HANDLE.Instance, &CCP_CANRxMsg);
	if(canOK == s)
	{
		memcpy(msg, CCP_CANRxMsg.Data, CCP_CANRxMsg.DLC);
		return TRUE;
	}
	
	return FALSE;
}

canStatus canRead(CAN_TypeDef* canType, CanRxMsgTypeDef* pRxMessage)
{
	canStatus status = canERR_NOMSG;
	if(DeQueue(&CANRxMsgQueue, pRxMessage) == QUEUE_Operator_Success)
	{
		status = canOK;
	}
	
	return status;
}

canStatus canWrite(CAN_HandleTypeDef *canHandle, CanTxMsgTypeDef* pTxMessage)
{
	canStatus txStatus = canERR_TX;
	HAL_StatusTypeDef status = HAL_CAN_Transmit(canHandle, 0);
	if(HAL_OK == status)
		txStatus = canOK;
	
	return txStatus;
}

/**
  * @brief  Recieve complete callback in non blocking mode 
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* CanHandle)
{
	if(CCP_CAN_INTERFACE == CanHandle->Instance)
	{
		canComStatus = canOK;
		EnQueue(&CANRxMsgQueue, CCP_CANRxMsg);
		if(HAL_CAN_Receive_IT(CanHandle, CAN_FIFO0) != HAL_OK)
		{
			// TODO Error handler or indicate.
		}
	}
	else if( EGR_CAN_INTERFACE == CanHandle->Instance)
	{
		// TODO EGR_CAN Message.
	}
}
/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
