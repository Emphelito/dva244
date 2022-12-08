#define _CRT_SECURE_NO_WARNINGS

#include "list.h"
#include <stdlib.h>
#include <assert.h>


static struct node* createListNode(const Data data)
{
    List tmp;
    tmp = (List*)malloc(sizeof(List) + 10); //Fix size needed
    if (tmp == NULL) return NULL;

    tmp->data = data;
    tmp->next = NULL;

    return tmp;
}

List createEmptyList(void)
{
    return NULL;
}

int isEmpty(const List list)
{
    if (list == NULL) return 1;
    return 0;
}

void addFirst(List* list, const Data data)
{
    List tmp;
    tmp = createListNode(data);

    tmp->next = *list;
    *list = tmp;

    assert(tmp->data == data);
}

void addLast(List* list, const Data data)
{
    List tmp = *list;

    if (tmp == NULL) {
        tmp = (List*)malloc(sizeof(List) + 10);
        tmp->data = data;
        tmp->next = NULL;
        *list = tmp;
    }
    else {
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }

        tmp->next = (List*)malloc(sizeof(List) + 10); //Fix size needed
        assert(tmp->next != NULL);

        tmp->next->data = data;
        tmp->next->next = NULL;
    }
}

void removeFirst(List* list)
{
    assert(!isEmpty(*list));

    List tmp = *list;
    tmp = tmp->next;

    free(*list);
    *list = NULL;

    *list = tmp;
}


void removeLast(List* list)
{
    assert(!isEmpty(*list));

    List tmp = *list;

    if (tmp->next == NULL) {
        free(*list);
        *list = NULL;
    }
    else {
        while (tmp->next->next != NULL) {
            tmp = tmp->next;
        }
        free(tmp->next);
        tmp->next = NULL;
    }
}

int removeElement(List* list, const Data data)
{
    List tmp = *list;
    List tmp2;

    if (*list == NULL)return 0;

    if (tmp->data != data) {
        while (tmp->next->data != data) {
            tmp = tmp->next;
            if (tmp->next == NULL)return 0;
        }
    }
    else {
        removeFirst(list);
        return 1;
    }

    tmp2 = tmp->next;
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
    return 0; //Ersatt med ratt returvarde
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

void clearList(List* list)
{
    assert(!isEmpty(*list));

    List tmp = *list;
    int count = 0;

    count = numberOfNodesInList(tmp);

    for (int x = 0; x < count; x++) {
        tmp = tmp->next;
        free(*list);
        *list = NULL;
        assert(*list == NULL);
        *list = tmp;
    }

    assert(isEmpty(*list));
}

void printList(const List list, FILE* textfile)
{
    assert(!isEmpty(list));

    List tmp = list;

    while (tmp != NULL) {
        fprintf(textfile, "%d \n", tmp->data);
        if (tmp->next != NULL)tmp = tmp->next;
    }
}

Data getFirstElement(const List list)
{
    assert(!isEmpty(list));

    List tmp = list;

    return tmp->data; //Ersatt med ratt returvarde
}

Data getLastElement(const List list)
{
    assert(!isEmpty(list));

    List tmp = list;

    while (tmp->next != NULL) {
        tmp = tmp->next;
    }

    return tmp->data; //Ersatt med ratt returvarde
}
