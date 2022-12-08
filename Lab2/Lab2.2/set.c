#include "Set.h"
#include <assert.h>

/*Funktionen ar fardig*/
Set initializeSet(void)
{
	return createEmptyList();
}

void addToSet(Set* set, const Data element)
{
    if (*set == NULL) {
        addFirst(set, element);
    }
    else if (isInSet(*set, element) == 0) {
        addFirst(set, element);
    }
    assert(isInSet(*set, element) == 1);
}

/* Postcondition: 'element' finns INTE i set (tips: anvand isInSet() for att verifiera) */
void removeFromSet(Set* set, const Data element)
{
    removeElement(set, element);

    assert(isInSet(*set, element) == 0);
}


int isInSet(const Set set, const Data element)
{
    List tmp = set;

    return search(tmp, element);
}

/* Anvands for testning och felsokning */
void printSet(const Set set, FILE *textfile)
{
    
}

