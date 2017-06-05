#include "LED.h"
#include "HAL_BSP_CONFIG.h"

void LED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(LEDs_RCC_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LEDs_PORT,&GPIO_InitStructure);
	
	/* close all leds at first.*/
	GPIO_SetBits(LEDs_PORT, LED1_PIN | LED2_PIN);
}
