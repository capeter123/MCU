/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-January-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "stm32f4xx_bsp.h"

/** @addtogroup STM32F4xx_StdPeriph_Templates
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SD_FATFS_TEST 1
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef    RCC_Clocks;
FATFS fs;
FRESULT fr;
FIL fil;
BYTE fbuffer[512] = {0};
BYTE fs_work_buf[4096] __attribute__((aligned(4))) = {0};
const char* write_content = "Hello Fatfs R0.11, mod by four gun.\r\n";
UINT nBytes;

/* Private function prototypes -----------------------------------------------*/
static void Delay(__IO uint32_t nTime);
static void SysTick_Configuration(void);
static void NVIC_Gloabl_Configuration(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files (startup_stm32f40xx.s/startup_stm32f427x.s) before to branch to 
       application main. 
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */ 
	RCC_GetClocksFreq(&RCC_Clocks);
	NVIC_Gloabl_Configuration();
	SysTick_Configuration();
	LED_GPIO_Init();
	USART1_Config();
#if (SD_FATFS_TEST == 0)
	SD_TestAll();
#else
	fr = f_mount(&fs, "0:", 1);
	if(FR_NO_FILESYSTEM == fr)
	{
		printf("Not found a valid filesystem, format it now...\r\n");
		fr = f_mkfs("0:", 0, 4096);
		if(FR_OK == fr)
		{
			printf("mkfs finished, you should reset the mcu...\r\n");
		}
	}
	else if(FR_OK == fr)
	{
		printf("Mount FileSystem Successfully!\r\n");
		printf("Fatfs Write Test>>>\r\n");
		fr = f_open(&fil, "0:/DEMO1.txt", FA_CREATE_ALWAYS | FA_WRITE);
		if(FR_OK == fr)
		{
			fr = f_write(&fil, write_content, strlen(write_content), &nBytes);
			if(FR_OK == fr)
				printf(" File write success, [%d] bytes.\r\n", nBytes);
		}
		else if(FR_EXIST == fr)
		{
			printf("File has exist, nothing to do!\r\n");
		}
		
		f_close(&fil);
		
		printf("Fatfs Read test>>>\r\n");
		fr = f_open(&fil, "0:/DEMO1.txt", FA_OPEN_EXISTING | FA_READ);
		if(FR_OK == fr)
		{
			memset(fbuffer, 0, sizeof(fbuffer));
			fr = f_read(&fil, fbuffer, sizeof(fbuffer), &nBytes);
			if(FR_OK == fr)
			{
				printf("%s\r\n", fbuffer);
			}
			else
			{
				printf("Read failed\r\n");
			}
			
			f_close(&fil);
		}
	}
	
	f_mount(NULL, "0:", 0);
#endif
	while(1)
	{
		GPIO_ToggleBits(LEDs_PORT, LED1_PIN);
		Delay(1000);
		//printf("Hello World\r\n");
	}
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  uwTimingDelay = nTime;

  while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
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
static void SysTick_Configuration(void)
{
	/* SystemFrequency / 1000    => 1ms 
	   SystemFrequency / 1000000 => 1us */
	if(SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000))
	{
		while(1)
		{
		}
	}
	
	NVIC_SetPriority(SysTick_IRQn, 0x8);
}

static void NVIC_Gloabl_Configuration(void)
{
	uint32_t reg_value;
	uint32_t PriorityGroupTemp = ((NVIC_PriorityGroup_1 >> 8) & 0x07);
	reg_value = SCB->AIRCR;
	reg_value &= ~(SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIGROUP_Msk);
	reg_value = (reg_value |
				((uint32_t)0x05FA << SCB_AIRCR_VECTKEY_Pos) |
				(PriorityGroupTemp << 8));
	SCB->AIRCR = reg_value;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
