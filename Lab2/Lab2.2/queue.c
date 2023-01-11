#include "queue.h"
#include<assert.h>

/*Har ar det viktigt att ateranvanda koden som du redan skrivit for den lankade listan.
  Ateranvandning betyder inte att man kopierar koden eller skriver av den, ateranvander
  gor man genom att anropa redan implementerade funktioner.*/

/*Funktionen ar fardig*/
Queue initializeQueue(void)
{
    return NULL;
}

int queueIsEmpty(const Queue queue)
{
    if (queue == NULL)return 1;
    return 0;	// Ersatt med ratt returvarde
}

/*Postcondition: data ar tillagt sist i kon*/
void enqueue(Queue* queue, const Data data)
{
    addLast(queue, data);

    assert(getLastElement(*queue) == data);
}

/* Precondition: kon är ej tom */
void dequeue(Queue* queue)
{
    assert(isEmpty(queue) != 1);
    removeFirst(queue);
}

/* Precondition: kon är ej tom */
Data peekQueue(const Queue queue)
{
    assert(isEmpty(queue) != 1);

    return getFirstElement(queue);	// Ersatt med ratt returvarde
}


/* Anvands for testning och felsokning
   Tips: det kan vara bra att ocksa notera i utskriften vart borjan respektive slutet på kon är */
void printQueue(const Queue queue, FILE *textfile)
{
    printList(queue, textfile);
}


