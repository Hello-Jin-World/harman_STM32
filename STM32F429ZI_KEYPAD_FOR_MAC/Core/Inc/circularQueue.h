#ifndef CIRCULARQUEUE_H_
#define CIRCULARQUEUE_H_

#include "stm32f4xx_hal.h"

#define TRUE 1
#define FALSE 0

#define QUE_LEN 100
typedef uint8_t Data;

typedef struct _cQueue
{
	uint8_t front;
	uint8_t rear;
	Data queArr[QUE_LEN];
} CQueue;

typedef CQueue Queue;

void QueueInit(Queue* pq);
uint8_t QIsEmpty(Queue* pq); // check queue is empty state
void Enqueue(Queue* pq, Data data);
Data Dequeue(Queue* pq);
Data QPeek (Queue* pq) ;


#endif /* CIRCULARQUEUE_H_ */
