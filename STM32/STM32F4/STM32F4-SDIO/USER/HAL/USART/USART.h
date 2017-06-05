#ifndef _USART_H_
#define _USART_H_

#define USART1_BUFFER_SIZE       (256)
#define TIMEOUT_MAX              10000 /* Maximum timeout value */

void USART1_DMA_Config(void);
void USART1_Config(void);

extern unsigned char DebugBuff[];
#endif
