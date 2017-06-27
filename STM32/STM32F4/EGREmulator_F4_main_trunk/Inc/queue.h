#ifndef _QUEUE_H
#define _QUEUE_H

#include "stm32f4xx_hal.h"
#include <stdlib.h>
#include <string.h>

#define QUEUE_Operator_Failed 0
#define QUEUE_Operator_Success 1
#define MAXQSIZE 10

typedef struct{
	CanRxMsgTypeDef* base;
	int front;
	int rear;
}CircleQueue;

int8_t InitQueue(CircleQueue *Q);
int getQueueLength(CircleQueue Q);
int8_t EnQueue(CircleQueue *Q, CanRxMsgTypeDef msg);
int8_t DeQueue(CircleQueue *Q, CanRxMsgTypeDef *msg);
_Bool bEmptyQueue(CircleQueue *Q);
_Bool bFullQueue(CircleQueue *Q);

#endif
