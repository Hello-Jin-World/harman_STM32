#include <stdio.h>
#include <stdlib.h>
#include "circularQueue.h"

Queue keypad_queue;

void QueueInit(Queue* pq) // 텅 빈 경우 front와 rear은 동일위치 가리킴
{
	pq->front = 0 ;
	pq->rear = 0;
}

uint8_t QIsEmpty(Queue* pq)
{
	if(pq->front == pq->rear) // 큐가 텅 비었다면 TRUE
		return TRUE;
	else
		return FALSE;
}

uint8_t NextPosIdx(uint8_t pos)
{
	if(pos == QUE_LEN - 1)
		return 0;
	else
		return pos+1;
}

void Enqueue(Queue* pq, Data data)
{
	if(NextPosIdx(pq->rear) == pq->front)
	{
		printf( "Queue Memory Error!");
		exit(-1) ;
	}
	
	pq->rear = NextPosIdx(pq->rear);
	pq->queArr[pq->rear] = data;
}

Data Dequeue(Queue* pq)
{
	if(QIsEmpty(pq))
	{
		printf( "Queue Memory Error! " ) ;
		exit(-1);
	}
	
	pq->front = NextPosIdx(pq->front);
	return pq->queArr[pq->front];
}

Data QPeek(Queue* pq)
{
	if(QIsEmpty(pq))
	{
		printf( "Queue Memory Error!" ) ;
		exit(-1);
	}
	
	return pq->queArr[NextPosIdx(pq->front)];
}
