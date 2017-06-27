#include "queue.h"

int8_t InitQueue(CircleQueue *Q)
{
	int8_t status = QUEUE_Operator_Failed;
	Q->base = (CanRxMsgTypeDef*)malloc(sizeof(CanRxMsgTypeDef)*MAXQSIZE);
	if(Q->base)
	{
		Q->front = Q->rear = 0;
		status = QUEUE_Operator_Success;
	}
	
	return status;
}	

int getQueueLength(CircleQueue Q)
{
	return (Q.front-Q.rear+MAXQSIZE)%MAXQSIZE;
}

_Bool bEmptyQueue(CircleQueue *Q)
{
	return (Q->front == Q->rear);
}

_Bool bFullQueue(CircleQueue *Q)
{
	return ((Q->rear+1)%MAXQSIZE==Q->front);
}

int8_t EnQueue(CircleQueue *Q, CanRxMsgTypeDef msg)
{
	if(bFullQueue(Q))
	{
		return QUEUE_Operator_Failed;
	}
	
	Q->base[Q->rear] = msg;
	Q->rear = (Q->rear+1)%MAXQSIZE;
	return QUEUE_Operator_Success;
}

int8_t DeQueue(CircleQueue *Q, CanRxMsgTypeDef *msg)
{
	if(bEmptyQueue(Q))
	{
		return QUEUE_Operator_Failed;
	}
	
	memcpy(msg, &Q->base[Q->front], sizeof(CanRxMsgTypeDef));
	Q->front = (Q->front+1)%MAXQSIZE;
	return QUEUE_Operator_Success;
}
