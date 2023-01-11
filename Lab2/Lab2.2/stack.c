#include "stack.h"
#include<assert.h>

/*Funktionen ar fardig*/
Stack initializeStack(void)
{
    return createEmptyList();
}

int stackIsEmpty(const Stack stack)
{
    return isEmpty(stack);
}

/* Postcondition 'data' ligger overst på stacken */
void push(Stack* stack, const Data data)
{
    addFirst(stack, data);
    assert(getFirstElement(*stack) == data);
}

/* Precondition: stacken far inte vara tom */
void pop(Stack* stack)
{
    if (stackIsEmpty(stack) != 1) {
        removeFirst(stack);
    }
}

/* Precondition: stacken far inte vara tom */
Data peekStack(const Stack stack)
{
    if (stackIsEmpty(stack) != 1) {
        return getFirstElement(stack);
    }
}

/* Anvands for testning och felsokning
 Tips: det kan vara bra att ocksa notera i urskriften vart toppen pa stacken ar */
void printStack(const Stack stack, FILE *textFile)
{
    printList(stack, textFile);
}
