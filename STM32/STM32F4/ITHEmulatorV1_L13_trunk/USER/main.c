/**
  ******************************************************************************
  * @file    main.c 
  * @author  
  * @version V1.1.0
  * @date    8-April-2014
  * @brief   Main program body
  ******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
RCC_ClocksTypeDef RCC_Clocks;
 
/* Private function prototypes -----------------------------------------------*/
static void SysTick_Configuration(void);
static void NVIC_Global_Configuration(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

int main(void)
{
	RCC_GetClocksFreq(&RCC_Clocks);
	NVIC_Global_Configuration();
	SysTick_Configuration();
	
	LCD_GLASS_Init();
	SPIx_InitConfig(SPI_Mode_Slave);
	rt_InitConfig();
  /* Infinite loop */
  while (1)
  {
		rt_DisplayAndSendPos();
	}
}

/**
  * @brief  Configures the SysTick.
  * @param  None
  * @retval None
  */
void SysTick_Configuration(void)
{
  /* Setup SysTick Timer for 100 msec interrupts  */
  //if (SysTick_Config((SystemCoreClock) / 10))
	// SystemCoreClock) / 1000 = 1ms
	// SystemCoreClock) / 1000000 = 1us
	if (SysTick_Config((RCC_Clocks.HCLK_Frequency) / 1000UL))
  { 
    /* Capture error */ 
    while (1);
  }
  
	NVIC_SetPriority(SysTick_IRQn, 0x0);
	//NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
	////SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // Stop systick timer.
}

void NVIC_Global_Configuration(void)
{
	//NVIC_SetPriorityGrouping(NVIC_PriorityGroup_1);
	uint32_t reg_value;
  uint32_t PriorityGroupTmp = (NVIC_PriorityGroup_1>>8 & (uint32_t)0x07);               /* only values 0..7 are used          */

  reg_value  =  SCB->AIRCR;                                                   /* read old register configuration    */
  reg_value &= ~(SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIGROUP_Msk);             /* clear bits to change               */
  reg_value  =  (reg_value                                 |
                ((uint32_t)0x5FA << SCB_AIRCR_VECTKEY_Pos) |
                (PriorityGroupTmp << 8));                                     /* Insert write key and priorty group */
  SCB->AIRCR =  reg_value;
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Sleep(__IO uint32_t nTime)
{
  TimingDelay = nTime;
  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    --TimingDelay;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
