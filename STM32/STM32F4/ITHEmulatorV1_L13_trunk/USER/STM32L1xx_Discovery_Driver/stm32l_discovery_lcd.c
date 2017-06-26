/**
  ******************************************************************************
  * @file    stm32l_discovery_lcd.c
  * @author   jiangst.fnst@cn.fujitsu.com
  * @version 
  * @date    Feb-2014
  * @brief   This file includes driver for the glass LCD Module mounted on 
  *          STM32l discovery board MB963
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2014 FNST</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32l_discovery_lcd.h"
#include "discover_board.h"
#include "stm32l1xx_lcd.h"

/* this variable can be used for accelerate the scrolling exit when push user button */
volatile bool KeyPressed = 0; 
		
/* LCD BAR status: We don't write directly in LCD RAM for save the bar setting */
uint8_t t_bar[2]={0x0,0X0};
		
/*  =========================================================================
                                 LCD MAPPING
    =========================================================================
	                A
        __   ----------
COL |_|   |\    |J  /|
					 F| H   |  K  |B
				__   |  \  | /   |
COL |_| --G-- --M--
						 |   / | \   |
           E|  Q |  N |C
        __    | /   |P  \|   
DP   |_| -----------  
	                 D  

 An LCD character coding is based on the following matrix:
      { E , D , P , N   }
      { M , C , COL , DP}
      { B , A , K , J   }
      { G , F , Q , H   }

 The character 'A' for example is:
  -------------------------------
LSB   { 1 , 0 , 0 , 0   }
      { 1 , 1 , 0 , 0   }
      { 1 , 1 , 0 , 0   }
MSB   { 1 , 1 , 0 , 0   }
      -------------------
  'A' =  F    E   0   0 hexa

*/

/* Constant table for cap characters 'A' --> 'Z' */
const uint16_t CapLetterMap[26]=
    {
        /* A      B      C      D      E      F      G      H      I  */
        0xFE00,0x6714,0x1d00,0x4714,0x9d00,0x9c00,0x3f00,0xfa00,0x0014,
        /* J      K      L      M      N      O      P      Q      R  */
        0x5300,0x9841,0x1900,0x5a48,0x5a09,0x5f00,0xFC00,0x5F01,0xFC01,
        /* S      T      U      V      W      X      Y      Z  */
        0xAF00,0x0414,0x5b00,0x18c0,0x5a81,0x00c9,0x0058,0x05c0
    };

/* Constant table for number '0' --> '9' */
const uint16_t NumberMap[10]=
    {
        /* 0      1      2      3      4      5      6      7      8      9  */
        0x5F00,0x4200,0xF500,0x6700,0xEa00,0xAF00,0xBF00,0x04600,0xFF00,0xEF00
    };

static void LCD_Conv_Char_Seg(uint8_t* c,bool point,bool colon,uint8_t* digit);
static void Delay(__IO uint32_t nCount);
static void LCD_GLASS_Configure_GPIO(void);
void LCD_bar(void);
void LCD_GLASS_RCC_Configuration(void);
		
/**
  * @brief  Configures the LCD GLASS relative GPIO port IOs and LCD peripheral.
  * @param  None
  * @retval None
  */
void LCD_GLASS_Init(void)
{
  LCD_InitTypeDef LCD_InitStruct;
	LCD_GLASS_RCC_Configuration();
	LCD_GLASS_Configure_GPIO();

  LCD_InitStruct.LCD_Prescaler = LCD_Prescaler_4;
  LCD_InitStruct.LCD_Divider = LCD_Divider_17;
  LCD_InitStruct.LCD_Duty = LCD_Duty_1_4;
  LCD_InitStruct.LCD_Bias = LCD_Bias_1_3;
  LCD_InitStruct.LCD_VoltageSource = LCD_VoltageSource_Internal;
  /* Initialize the LCD */
  LCD_Init(&LCD_InitStruct);
  
  LCD_MuxSegmentCmd(ENABLE);
  
  /*!< Configure the Pulse On Duration */
  LCD_PulseOnDurationConfig(LCD_PulseOnDuration_4);
  /* To set contrast to mean value */
  LCD_ContrastConfig(LCD_Contrast_Level_4);
  
  LCD_DeadTimeConfig(LCD_DeadTime_0);
  /* Wait Until the LCD FCR register is synchronized */
  LCD_WaitForSynchro();
  
  /* Enable LCD peripheral */
  LCD_Cmd(ENABLE);
  
  /* Wait Until the LCD is enabled */
  while(LCD_GetFlagStatus(LCD_FLAG_ENS) == RESET)
  {
  }
  /*!< Wait Until the LCD Booster is ready */  
  while(LCD_GetFlagStatus(LCD_FLAG_RDY) == RESET)
  {
  } 

  LCD_BlinkConfig(LCD_BlinkMode_Off,LCD_BlinkFrequency_Div32);	  
  LCD_GLASS_Clear();
}

/**
  * @brief  To initialize the LCD pins
  * @caller main
  * @param None
  * @retval None
  */
void LCD_GLASS_Configure_GPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
/* Enable GPIOs clock */ 	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH, ENABLE);

/* Configure Output for LCD */
/* Port A */
  GPIO_StructInit(&GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_8 | GPIO_Pin_9 |GPIO_Pin_10; //| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init( GPIOA, &GPIO_InitStructure);
	
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10,GPIO_AF_LCD) ;
  //GPIO_PinAFConfig(GPIOA, GPIO_PinSource15,GPIO_AF_LCD) ;  
	
/* Configure Output for LCD */
/* Port B */  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_9 \
                                 | GPIO_Pin_10 | GPIO_Pin_11; // GPIO_Pin_8 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init( GPIOB, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5,GPIO_AF_LCD) ;
  //GPIO_PinAFConfig(GPIOB, GPIO_PinSource8,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11,GPIO_AF_LCD) ;  
//  GPIO_PinAFConfig(GPIOB, GPIO_PinSource12,GPIO_AF_LCD) ;
//  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13,GPIO_AF_LCD) ;   
//  GPIO_PinAFConfig(GPIOB, GPIO_PinSource14,GPIO_AF_LCD) ;
//  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15,GPIO_AF_LCD) ;   
  
/* Configure Output for LCD */
/* Port C*/  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 \
                                 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_11 ; //GPIO_Pin_0 | GPIO_Pin_1                           
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init( GPIOC, &GPIO_InitStructure);  
  
  //GPIO_PinAFConfig(GPIOC, GPIO_PinSource0,GPIO_AF_LCD) ;
 // GPIO_PinAFConfig(GPIOC, GPIO_PinSource1,GPIO_AF_LCD) ; 
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource2,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource3,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource8,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10,GPIO_AF_LCD) ; 
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11,GPIO_AF_LCD) ;  

/* Disable GPIOs clock */ 	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC |
                        RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH, DISABLE);
}

/**
  * @brief  LCD contrast setting min-->max-->min by pressing user button
  * @param  None
  * @retval None
  */
void LCD_contrast()
{
  uint32_t contrast ;
  
  /* To get the actual contrast value in register */
  contrast = LCD->FCR & LCD_Contrast_Level_7;
  
  while ((GPIOC->IDR & USERBUTTON_GPIO_PIN) == 0x0)
  {
    contrast += LCD_Contrast_Level_1;	
    
    if (contrast > LCD_Contrast_Level_7)
     contrast=LCD_Contrast_Level_0;
  
    LCD_ContrastConfig(contrast);
    Delay(100);
  }
}

/**
  * @brief  Setting bar on LCD, writes bar value in LCD frame buffer 
  * @param  None
  * @retval None
  */
void LCD_bar()
{   
  LCD->RAM[LCD_RAMRegister_4] &= 0xffff5fff;
  LCD->RAM[LCD_RAMRegister_6] &= 0xffff5fff;
/* bar1 bar3 */
  LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)(t_bar[0]<<12);
  
/*bar0 bar2 */
  LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)(t_bar[1]<<12);
}

/**
  * @brief  Converts an ascii char to the a LCD digit.
  * @param  c: a char to display.
  * @param  point: a point to add in front of char
  *         This parameter can be: POINT_OFF or POINT_ON
  * @param  colon : flag indicating if a colon has to be add in front
  *         of displayed character.
  *         This parameter can be: COLUMN_OFF or COLUMN_ON.
	* @param 	digit array with segment 
  * @retval None
  */
static void LCD_Conv_Char_Seg(uint8_t* c,bool point,bool colon, uint8_t* digit)
{
  uint16_t ch = 0 ;
  uint8_t i,j;
  
  switch (*c)
    {
    case ' ' : 
      ch = 0x00;
      break;
    
    case '*':
      ch = star;
      break;
                  
    case 'u':
      ch = C_UMAP;
      break;
    
    case 'm' :
      ch = C_mMap;
      break;
                  
    case 'n' :
      ch = C_nMap;
      break;					
                  
    case '-' :
      ch = C_minus;
      break;
      
    case '/' :
      ch = C_slatch;
      break;  
      
    case 'd' :
      ch = C_percent_1;
      break;  
		
    case 'q' :
			ch = C_percent_2;
      break;
		
    case 255 :
      ch = C_full;
      break ;
    
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':			
      ch = NumberMap[*c-0x30];		
      break;
          
    default:
      /* The character c is one letter in upper case*/
      if ( (*c < 0x5b) && (*c > 0x40) )
      {
        ch = CapLetterMap[*c-'A'];
      }
      /* The character c is one letter in lower case*/
      if ( (*c <0x7b) && ( *c> 0x60) )
      {
        ch = CapLetterMap[*c-'a'];
      }
      break;
  }
       
  /* Set the digital point can be displayed if the point is on */
  if (point)
  {
    ch |= 0x0002;
  }

  /* Set the "COL" segment in the character that can be displayed if the colon is on */
  if (colon)
  {
    ch |= 0x0020;
  }		

  for (i = 12,j=0 ;j<4; i-=4,j++)
  {
    digit[j] = (ch >> i) & 0x0f; //To isolate the less signifiant dibit
  }
}

/**
  * @brief  This function writes a char in the LCD frame buffer.
  * @param  ch: the character to display.
  * @param  point: a point to add in front of char
  *         This parameter can be: POINT_OFF or POINT_ON
  * @param  colon: flag indicating if a colon has to be add in front
  *         of displayed character.
  *         This parameter can be: COLUMN_OFF or COLUMN_ON.           
  * @param  position: position in the LCD of the caracter to write [0:7]
  * @retval None
  * @par    Required preconditions: The LCD should be cleared before to start the
  *         write operation.  
  */
void LCD_GLASS_WriteChar(uint8_t* ch, bool point, bool colon, uint8_t position)
{
  uint8_t digit[4];     /* Digit frame buffer */
   
/* To convert displayed character in segment in array digit */
  LCD_Conv_Char_Seg(ch,point,colon,digit);

 
  switch (position)
  {
    /* Position 1 on LCD (Digit1)*/
    case 1:
      LCD->RAM[LCD_RAMRegister_0] &= 0xcffffffc;
      LCD->RAM[LCD_RAMRegister_2] &= 0xcffffffc;
      LCD->RAM[LCD_RAMRegister_4] &= 0xcffffffc;
      LCD->RAM[LCD_RAMRegister_6] &= 0xcffffffc;

      LCD->RAM[LCD_RAMRegister_0] |= ((digit[0]& 0x0c) << 26 ) | (digit[0]& 0x03) ; // 1G 1B 1M 1E	    
      LCD->RAM[LCD_RAMRegister_2] |= ((digit[1]& 0x0c) << 26 ) | (digit[1]& 0x03) ; // 1F 1A 1C 1D 
      LCD->RAM[LCD_RAMRegister_4] |= ((digit[2]& 0x0c) << 26 ) | (digit[2]& 0x03) ; // 1Q 1K 1Col 1P                                                                                                                                    
      LCD->RAM[LCD_RAMRegister_6] |= ((digit[3]& 0x0c) << 26 ) | (digit[3]& 0x03) ; // 1H 1J 1DP 1N

      break;
    
    /* Position 2 on LCD (Digit2)*/
    case 2:
      LCD->RAM[LCD_RAMRegister_0] &= 0xf3ffff03;
      LCD->RAM[LCD_RAMRegister_2] &= 0xf3ffff03;      
      LCD->RAM[LCD_RAMRegister_4] &= 0xf3ffff03;
      LCD->RAM[LCD_RAMRegister_6] &= 0xf3ffff03;
      
      LCD->RAM[LCD_RAMRegister_0] |= ((digit[0]& 0x0c) << 24 )|((digit[0]& 0x02) << 6 )|((digit[0]& 0x01) << 2 ) ; // 2G 2B 2M 2E	  
      LCD->RAM[LCD_RAMRegister_2] |= ((digit[1]& 0x0c) << 24 )|((digit[1]& 0x02) << 6 )|((digit[1]& 0x01) << 2 ) ; // 2F 2A 2C 2D
      LCD->RAM[LCD_RAMRegister_4] |= ((digit[2]& 0x0c) << 24 )|((digit[2]& 0x02) << 6 )|((digit[2]& 0x01) << 2 ) ; // 2Q 2K 2Col 2P
      LCD->RAM[LCD_RAMRegister_6] |= ((digit[3]& 0x0c) << 24 )|((digit[3]& 0x02) << 6 )|((digit[3]& 0x01) << 2 ) ; // 2H 2J 2DP 2N
      
      break;
    
    /* Position 3 on LCD (Digit3)*/
    case 3:
      LCD->RAM[LCD_RAMRegister_0] &= 0xfcfffcff;
      LCD->RAM[LCD_RAMRegister_2] &= 0xfcfffcff;
      LCD->RAM[LCD_RAMRegister_4] &= 0xfcfffcff;
      LCD->RAM[LCD_RAMRegister_6] &= 0xfcfffcff;

      LCD->RAM[LCD_RAMRegister_0] |= ((digit[0]& 0x0c) << 22 ) | ((digit[0]& 0x03) << 8 ) ; // 3G 3B 3M 3E	
      LCD->RAM[LCD_RAMRegister_2] |= ((digit[1]& 0x0c) << 22 ) | ((digit[1]& 0x03) << 8 ) ; // 3F 3A 3C 3D
      LCD->RAM[LCD_RAMRegister_4] |= ((digit[2]& 0x0c) << 22 ) | ((digit[2]& 0x03) << 8 ) ; // 3Q 3K 3Col 3P
      LCD->RAM[LCD_RAMRegister_6] |= ((digit[3]& 0x0c) << 22 ) | ((digit[3]& 0x03) << 8 ) ; // 3H 3J 3DP 3N
      
      break;
    
    /* Position 4 on LCD (Digit4)*/
    case 4:
      LCD->RAM[LCD_RAMRegister_0] &= 0xffcff3ff;
      LCD->RAM[LCD_RAMRegister_2] &= 0xffcff3ff;
      LCD->RAM[LCD_RAMRegister_4] &= 0xffcff3ff;
      LCD->RAM[LCD_RAMRegister_6] &= 0xffcff3ff;
      
      LCD->RAM[LCD_RAMRegister_0] |= ((digit[0]& 0x0c) << 18 ) | ((digit[0]& 0x03) << 10 ) ; // 4G 4B 4M 4E	
      LCD->RAM[LCD_RAMRegister_2] |= ((digit[1]& 0x0c) << 18 ) | ((digit[1]& 0x03) << 10 ) ; // 4F 4A 4C 4D
      LCD->RAM[LCD_RAMRegister_4] |= ((digit[2]& 0x0c) << 18 ) | ((digit[2]& 0x03) << 10 ) ; // 4Q 4K 4Col 4P
      LCD->RAM[LCD_RAMRegister_6] |= ((digit[3]& 0x0c) << 18 ) | ((digit[3]& 0x03) << 10 ) ; // 4H 4J 4DP 4N
      
      break;
    
    /* Position 5 on LCD (Digit5)*/
    case 5:
      LCD->RAM[LCD_RAMRegister_0] &= 0xfff3cfff;
      LCD->RAM[LCD_RAMRegister_2] &= 0xfff3cfff;
      LCD->RAM[LCD_RAMRegister_4] &= 0xfff3efff;
      LCD->RAM[LCD_RAMRegister_6] &= 0xfff3efff;

      LCD->RAM[LCD_RAMRegister_0] |= ((digit[0]& 0x0c) << 16 ) | ((digit[0]& 0x03) << 12 ) ; // 5G 5B 5M 5E	
      LCD->RAM[LCD_RAMRegister_2] |= ((digit[1]& 0x0c) << 16 ) | ((digit[1]& 0x03) << 12 ) ; // 5F 5A 5C 5D
      LCD->RAM[LCD_RAMRegister_4] |= ((digit[2]& 0x0c) << 16 ) | ((digit[2]& 0x01) << 12 ) ; // 5Q 5K   5P 
      LCD->RAM[LCD_RAMRegister_6] |= ((digit[3]& 0x0c) << 16 ) | ((digit[3]& 0x01) << 12 ) ; // 5H 5J   5N
      
      break;
    
    /* Position 6 on LCD (Digit6)*/
    case 6:
      LCD->RAM[LCD_RAMRegister_0] &= 0xfffc3fff;
      LCD->RAM[LCD_RAMRegister_2] &= 0xfffc3fff;
      LCD->RAM[LCD_RAMRegister_4] &= 0xfffc3fff;
      LCD->RAM[LCD_RAMRegister_6] &= 0xfffc3fff;

      LCD->RAM[LCD_RAMRegister_0] |= ((digit[0]& 0x04) << 15 ) | ((digit[0]& 0x08) << 13 ) | ((digit[0]& 0x03) << 14 ) ; // 6B 6G 6M 6E	
      LCD->RAM[LCD_RAMRegister_2] |= ((digit[1]& 0x04) << 15 ) | ((digit[1]& 0x08) << 13 ) | ((digit[1]& 0x03) << 14 ) ; // 6A 6F 6C 6D
      LCD->RAM[LCD_RAMRegister_4] |= ((digit[2]& 0x04) << 15 ) | ((digit[2]& 0x08) << 13 ) | ((digit[2]& 0x01) << 14 ) ; // 6K 6Q    6P 
      LCD->RAM[LCD_RAMRegister_6] |= ((digit[3]& 0x04) << 15 ) | ((digit[3]& 0x08) << 13 ) | ((digit[3]& 0x01) << 14 ) ; // 6J 6H   6N
      
      break;
    
     default:
      break;
  }

/* Refresh LCD  bar */
  LCD_bar();
}


/**
  * @brief  This function writes a char in the LCD RAM.
  * @param  ch: The character to dispaly.
  * @param  point: A point to add in front of char.
  *          This parameter  can be one of the following values:  
  *              @arg POINT_OFF: No point to add in front of char.
  *              @arg POINT_ON: Add a point in front of char.
  * @param  apostrophe: Flag indicating if a apostrophe has to be add in front 
  *                     of displayed character.
  *          This parameter  can be one of the following values:
  *              @arg APOSTROPHE_OFF: No apostrophe to add in back of char.
  *              @arg APOSTROPHE_ON: Add an apostrophe in back of char.
  * @param  position: Position in the LCD of the caracter to write.
  *                   This parameter can be any value in range [0:7].
  * @retval None
  */
void LCD_GLASS_DisplayChar(uint8_t* ch, bool point,bool apostrophe, uint8_t position)
{
  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }
  /*!< LCD Write Char */
  LCD_GLASS_WriteChar(ch, point, apostrophe, position);

  /*!< Requesy LCD RAM update */
  LCD_UpdateDisplayRequest();  
}

/**
  * @brief  This function writes a char in the LCD RAM.
  * @param  ptr: Pointer to string to display on the LCD Glass.
  * @retval None
  */
void LCD_GLASS_DisplayString(uint8_t* ptr)
{
  uint8_t i = 0x01;

	/* wait for LCD Ready */  
  while( LCD_GetFlagStatus (LCD_FLAG_UDR) != RESET) ;
	
  /* Send the string character by character on lCD */
  while ((*ptr != 0) & (i < 8))
  {
    /* Display one character on LCD */
    LCD_GLASS_WriteChar(ptr, POINT_OFF, COLUMN_OFF , i);

    /* Point on the next character */
    ptr++;

    /* Increment the character counter */
    i++;
  }

	/* Update the LCD display */
  LCD_UpdateDisplayRequest();
}

/**
  * @brief  This function writes a char in the LCD RAM.
  * @param  ptr: Pointer to string to display on the LCD Glass.
  * @retval None
  * @par    Required preconditions: Char is ASCCI value "Ored" with decimal point or Column flag
  */
void LCD_GLASS_DisplayStrDeci(uint16_t* ptr)
{
  uint8_t i = 0x01;
  uint8_t char_tmp;

	/* TO wait LCD Ready */  
  while( LCD_GetFlagStatus (LCD_FLAG_UDR) != RESET) ;
  
  /* Send the string character by character on lCD */
  while ((*ptr != 0) & (i < 8))
  {			
    char_tmp = (*ptr) & 0x00ff;
    
    switch ((*ptr) & 0xf000)
    {
      case DOT:
          /* Display one character on LCD with decimal point */
          LCD_GLASS_WriteChar(&char_tmp, POINT_ON, COLUMN_OFF, i);
          break;
      case DOUBLE_DOT:
          /* Display one character on LCD with decimal point */
          LCD_GLASS_WriteChar(&char_tmp, POINT_OFF, COLUMN_ON, i);
          break;
      default:
          LCD_GLASS_WriteChar(&char_tmp, POINT_OFF, COLUMN_OFF, i);		
          break;
    }/* Point on the next character */
    ptr++;
    
    /* Increment the character counter */
    i++;
  }
	/* Update the LCD display */
  LCD_UpdateDisplayRequest();
}

void LCD_GLASS_Printf(char* fmt, ...)
{
	uint8_t i=0;
	char buf[8] = {0}; // only surported 8 char.
	uint8_t *pStr = (uint8_t*)buf;
	uint8_t char_next;
	uint8_t percents[] = {'d','/','q'};

	va_list ap;
	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);
	
	while( LCD_GetFlagStatus (LCD_FLAG_UDR) != RESET) ;

	for(i=1; i<8 && *pStr!=0; ++i,++pStr)
	{
		char_next = *(pStr+1) & 0x00ff;
		if(':'==char_next || '.'==char_next)
		{
			switch(char_next)
			{
				case ':':
					LCD_GLASS_DisplayChar(pStr++,POINT_OFF,COLUMN_ON,i);
					break;
				case '.':
					LCD_GLASS_DisplayChar(pStr++,POINT_ON,COLUMN_OFF,i);
					break;
				default: break;
			}
		}
		else
		{
			switch(*pStr)
			{
				case '%':
					LCD_GLASS_DisplayChar(percents,POINT_OFF,COLUMN_OFF,i++);
					LCD_GLASS_DisplayChar(percents+1,POINT_OFF,COLUMN_OFF,i++);
					LCD_GLASS_DisplayChar(percents+2,POINT_OFF,COLUMN_OFF,i);
					break;
				default:
					LCD_GLASS_DisplayChar(pStr,POINT_OFF,COLUMN_OFF,i);
					break;
			}
		}
	}
}

/**
  * @brief  This function Clear the whole LCD RAM.
  * @param  None
  * @retval None
  */
void LCD_GLASS_Clear(void)
{
  uint32_t counter = 0;
	
  /* TO wait LCD Ready */  
  while( LCD_GetFlagStatus (LCD_FLAG_UDR) != RESET) ;
  
  for (counter = LCD_RAMRegister_0; counter <= LCD_RAMRegister_15; counter++)
  {
    LCD->RAM[counter] = 0;
  }

  /* Update the LCD display */
  LCD_UpdateDisplayRequest();
}

/**
  * @brief  Display a string in scrolling mode
  * @param  ptr: Pointer to string to display on the LCD Glass.
  * @param  nScroll: Specifies how many time the message will be scrolled
  * @param  ScrollSpeed : Speciifes the speed of the scroll, low value gives
  *         higher speed 
  * @retval None
  * @par    Required preconditions: The LCD should be cleared before to start the
  *         write operation.
  */
void LCD_GLASS_ScrollSentence(uint8_t* ptr, uint16_t nScroll, uint16_t ScrollSpeed)
{
  uint8_t Repetition;
  uint8_t Char_Nb;
  uint8_t* ptr1;
  uint8_t str[7]="";
  uint8_t Str_size;
  
  if (ptr == 0) return;

/* To calculate end of string */
  for (ptr1=ptr,Str_size = 0 ; *ptr1 != 0; Str_size++,ptr1++) ;
  
  ptr1 = ptr;
  
  LCD_GLASS_DisplayString(ptr);
  Delay(ScrollSpeed);
          
/* To shift the string for scrolling display*/
  for (Repetition=0; Repetition<nScroll; Repetition++)
  {
    for (Char_Nb=0; Char_Nb<Str_size; Char_Nb++)
    {
      *(str) =* (ptr1+((Char_Nb+1)%Str_size));
      *(str+1) =* (ptr1+((Char_Nb+2)%Str_size));
      *(str+2) =* (ptr1+((Char_Nb+3)%Str_size));
      *(str+3) =* (ptr1+((Char_Nb+4)%Str_size));
      *(str+4) =* (ptr1+((Char_Nb+5)%Str_size));
      *(str+5) =* (ptr1+((Char_Nb+6)%Str_size));
      LCD_GLASS_Clear();
      LCD_GLASS_DisplayString(str);
  
  /* user button pressed stop the scrolling sentence */
      if (KeyPressed)
              return;   		
      Delay(ScrollSpeed);
    }	
  }
}

void LCD_GLASS_RCC_Configuration(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_LCD, ENABLE);
  /* Allow access to the RTC */
  PWR_RTCAccessCmd(ENABLE);

  /* Reset Backup Domain */
  RCC_RTCResetCmd(ENABLE);
  RCC_RTCResetCmd(DISABLE);

  /*!< LSE Enable */
  RCC_LSEConfig(RCC_LSE_ON);

  /*!< Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}
  /*!< LCD Clock Source Selection */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
*/
static void Delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (0xFF * nCount); index != 0; index--)
  {
  }
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
