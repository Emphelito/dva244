#define _CRT_SECURE_NO_WARNINGS

#include "list.h"
#include <stdlib.h>
#include <assert.h>


static struct node* createListNode(const Data data)
{
    List tmp; //Creates node
    tmp = (List*)malloc(sizeof(List) * data); 
    if (tmp == NULL) return NULL;

    tmp->data = data; //Assigns data to node
    tmp->next = NULL; //Assigns Next node(NULL) to the node

    return tmp; //Returns the node
}

List createEmptyList(void)
{
    return NULL;
}

int isEmpty(const List list)
{
    if (list== NULL) return 1;
    return 0;
}

void addFirst(List *list, const Data data)
{
    List tmp; //Creates node
    tmp = createListNode(data); //function to allocate mem to node and initiate it
    if (tmp == NULL) return;

    tmp->next = *list; //this is so we can assign a new node to tmp
    *list = tmp; //the node we created is now the head

    assert(tmp->data == data);
}

void addLast(List *list, const Data data)
{
    List tmp = *list;

    if (tmp == NULL) { //Checks if list is empty
        tmp = (List*)malloc(sizeof(List) * data);
        if (tmp == NULL) return;

        tmp->data = data;
        tmp->next = NULL;

        *list = tmp;
    }
    else {
        while (tmp->next != NULL) { //Used to get to last node
            tmp = tmp->next;
        }

        tmp->next = (List*)malloc(sizeof(List) * data); 
        if (tmp == NULL) return;

        tmp->next->data = data;
        tmp->next->next = NULL;
    }
}

void removeFirst(List *list)
{
    assert(!isEmpty(*list));//Checks if list is empty

    List tmp = *list;

    tmp = tmp->next; //Used later to assign new head
    
    free(*list); //Freeing mem
    *list = NULL;

    *list = tmp; //New head
}


void removeLast(List *list)
{
    assert(!isEmpty(*list));//Checks if list is empty

    List tmp = *list;

    if (tmp->next == NULL) { //Used when list contains one item
        free(*list);
        *list = NULL;
    }
    else {
        while (tmp->next->next != NULL) { //Keeps going till last item
            tmp = tmp->next;
        }
        free(tmp->next);
        tmp->next = NULL;
    }
}

int removeElement(List *list, const Data data)
{
    List tmp = *list; 
    List tmp2; 

    if (*list == NULL)return 0; //Checks if list is empty

    if (tmp->data != data) {
        while (tmp->next->data != data) {
            tmp = tmp->next;
            if (tmp->next == NULL)return 0;//If elements does not exist return 0
        }
    }
    else { //If first element is what we are looking for
        removeFirst(list); 
        return 1;
    }

    tmp2 = tmp->next; //Holds node to be removed
    tmp->next = tmp2->next;
    free(tmp2);
    
    return 1; 
}

int search(const List list, const Data data)
{
    List tmp = list;

    while (tmp != NULL) {
        if (tmp->data == data) {
            return 1;
        }
       
        if (tmp->next == NULL) return 0;

        tmp = tmp->next;
    }
    return 0; 
}

int numberOfNodesInList(const List list)
{
    List tmp = list;
    int count = 0;

    while (tmp != NULL) {
        tmp = tmp->next;
        count++;
    }

    return count; 
}

void clearList(List *list)
{
    assert(!isEmpty(*list));

    List tmp = *list;
    int count = 0;

    count = numberOfNodesInList(tmp); //Return of fuction used in for loop

    for (int x = 0; x < count; x++) {
        tmp = tmp->next;
        free(*list);
        *list = NULL;
        assert(*list == NULL);
        *list = tmp; //assign new head
    }

    assert(isEmpty(*list)); //Checks that list is empty
}

void printList(const List list, FILE *textfile)
{
    assert(!isEmpty(list)); 

    List tmp = list;

    while (tmp != NULL) {
        fprintf(textfile, "%d \n", tmp->data);
        if(tmp->next != NULL)tmp = tmp->next;
    }
}

Data getFirstElement(const List list)
{
    assert(!isEmpty(list));

    List tmp = list;

    return tmp->data; 
}

Data getLastElement(const List list)
{
    assert(!isEmpty(list));

    List tmp = list;

    while (tmp->next != NULL) {
        tmp = tmp->next;
    }

    return tmp->data; 
}
