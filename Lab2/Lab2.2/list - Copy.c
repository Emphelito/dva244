#define _CRT_SECURE_NO_WARNINGS

#include "list.h"
#include <stdlib.h>
#include <assert.h>

/*Det Šr helt tillŒtet att lŠgga till egna hjŠlpfunktioner men de befintliga funktionerna fŒr inte Šndras*/

/*Hjalpfunktion till add
  Allokerar minne for en ny nod
  om allokeringen lyckades initieras data samt pekare (pekare initieras till NULL).
  Den nya noden (eller NULL) returneras.*/
static struct node* createListNode(const Data data)
{
    List tmp = NULL;
    tmp = (List*)malloc(sizeof(List)+ 10); //Fix size needed

    tmp->data = data;
    tmp->next = NULL;

    return tmp; //Ersatt med ratt returvarde
}

/*Returnera en tom lista - funktionen ar fardig*/
List createEmptyList(void)
{
    return NULL;
}


/*Ar listan tom?
  Returnerar 1 om den Šr tom, annars 0*/
int isEmpty(const List list)
{
    List tmp = list;
    if (list== NULL) return 1;
    return 0; //ersatt med ratt returvarde
}

/*Lagg till nod forst i listan*/
/*Postcondition: Det nya datat ligger forst i listan (testa med assert)*/
void addFirst(List *list, const Data data)
{
    List tmp;
    tmp = createListNode(data);

    tmp->next = *list;
    *list = tmp;
    //Anropa createListNode for att skapa den nya noden
    //Glom inte att testa att den nya noden faktiskt kunde skapas/tilldelas minne innan du fortsatter
    //Tank pa att listan kan vara tom nar en ny nod laggs till
    
}

void addLast(List *list, const Data data)
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
        tmp->next->data = data;
        tmp->next->next = NULL;
    }
}

/*Ta bort forsta noden i listan
  Precondition: listan ar inte tom (testa med assert)
  Noden ska lankas ur och minnet frigoras, resten av listan ska finnas kvar*/
void removeFirst(List *list)
{
    List tmp = *list;
    tmp = tmp->next;
    
    free(*list);
    *list = NULL;

    *list = tmp;
}

/*Ta bort sista noden i listan
  Precondition: listan ar inte tom (testa med assert)t*/
void removeLast(List *list)
{
    List tmp = *list;
    assert(!isEmpty(*list));
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
    



    //Glom inte att frigora minnet for den nod som lankas ur listan.
    //Tank pa att den nod som nu ar sist inte pekar nagonstans, dess pekare maste nollstallas
}

/*Ta bort data ur listan (forsta forekomsten)
  Returnera 1 om datat finns, annars 0
  Tips, nar du hittar ratt nod kan du anvanda en av de ovanstaende funktionerna for att ta bort noden*/
int removeElement(List *list, const Data data)
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

/*Finns data i listan?
  Om datat finns returneras 1, annars 0
  Tank pa att listan kan vara tom*/
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

/*Returnera antalet noder i listan*/
int numberOfNodesInList(const List list)
{
    List tmp = list;
    int count = 0;

    while (tmp != NULL) {
        tmp = tmp->next;
        count++;
    }

    return count; //Ersatt med ratt returvarde
}

/*Ta bort alla noder ur listan
  Glom inte att frigora minnet
  Postcondition: Listan ar tom, *list Šr NULL (testa med assert)*/
void clearList(List *list)
{
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

    /*while (tmp != NULL) {
        if(tmp->next != NULL)tmp = tmp->next;
        free(*list);
        *list = NULL;
        assert(*list == NULL);
        *list = tmp;
    }*/
}

/*Skriv ut listan
  Vid anropet kan man ange stdout som argument 2 for att skriva ut pŒ skarmen.
  Anvanda fprintf for att skriva ut.
  Den har typen av utskriftfunktion blir mer generell da man kan valja att skriva ut till skarmen eller till fil.*/
void printList(const List list, FILE *textfile)
{
    
}

/*Returnera forsta datat i listan
  Precondition: listan ar inte tom (testa med assert)*/
Data getFirstElement(const List list)
{
    List tmp = list;

    return tmp->data; //Ersatt med ratt returvarde
}

/*Returnera sista datat i listan
  Precondition: listan ar inte tom (testa med assert)*/
Data getLastElement(const List list)
{
    List tmp = list;

    while (tmp->next != NULL) {
        tmp = tmp->next;
    }

    return tmp->data; //Ersatt med ratt returvarde
}
