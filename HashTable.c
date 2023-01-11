#define _CRT_SECURE_NO_WARNINGS // Behovs for vissa funktioner i visual studio
#include "HashTable.h"
#include "Bucket.h"
#include<assert.h>
#include<stdlib.h>
#include<stdio.h>

static int hash(Key key, int tablesize)
{
    return key % tablesize; //Getting index
}

//If key already exsists linearProbe() is never called, see first if in insertElement()
static int linearProbe(const HashTable* htable, Key key, unsigned int *col)
{
    int index = hash(key, htable->size);
    int start = hash(key, htable->size);

    if (htable->table[index].key == key) { //If correct index is empty just return index
        return index;
    }

    while (htable->table[index].key != NULL) { //Till a empty spot is found
        index = (index + 1) % htable->size;
        *col += 1;

        if (start == index) return htable->size; //Could not return NULL
    }
    return index; 
}

HashTable createHashTable(unsigned int size)
{
    HashTable htable;

    htable.size = size;

    htable.table = malloc(sizeof(struct Bucket) * size); //Allocating mem

    for (int i = 0; i < size; i++) {
        htable.table[i].key = NULL; //So that empty indexes can be found
    }

    return htable;
}

unsigned int insertElement(HashTable* htable, const Key key, const Value value)
{
    int col = 0;

    if (lookup(htable, key) != NULL) {
        int index = hash(key, htable->size);
        while (htable->table[index].key != key) { //Till duplicate is found
            index = (index + 1) % htable->size;
        }
        htable->table[index].key = key;
        htable->table[index].value = value;
        return col;
    }
    int index = linearProbe(htable, key, &col); //Gettint index and collisons

    if (index == htable->size) { //Using size instead of NULL because NULL was changed to 0 for some reason. linearProbe returns size if table is full
        return col;
    }

    htable->table[index].key = key;
    htable->table[index].value = value;

    assert(lookup(htable, key) != NULL); //Checking if key exsists
    return col; //Collision
}

/* Tar bort datat med nyckel "key" */
void deleteElement(HashTable* htable, const Key key)
{
    int index = hash(key, htable->size);

    while (htable->table[index].key != NULL) { //Checking till key is found or index is NULL
        if (htable->table[index].key == key) {
            htable->table[index].key = NULL;
            assert(lookup(htable, key) == NULL); //Checking that key is deleted
            int i = (index + 1) % htable->size;

            while (htable->table[i].key != NULL) { //Updating indexes of keys
                Value tmp = htable->table[i].value;
                Key tmpK = htable->table[i].key;
                htable->table[i].key = NULL;
                insertElement(htable, tmpK, tmp);
                i = (i + 1) % htable->size;
            }

            return;
        }

        index = (index + 1) % htable->size;
    }
    assert(lookup(htable, key) == NULL);
}

const Value* lookup(const HashTable* htable, const Key key)
{
    Value* tmpPtr;

    int index = hash(key, htable->size);
    int start = hash(key, htable->size);

    if (htable->table[index].key == key) { //If key is in correct index
        tmpPtr = &htable->table[index].value;
        return tmpPtr;
    }

    while (htable->table[index].key != NULL) { //If key is not in correct index
        if (htable->table[index].key == key) {
            tmpPtr = &htable->table[index].value;
            return tmpPtr;
        }
        index = (index + 1) % htable->size;
        if (start == index) return NULL;
    }

    return NULL; //If key does not exist
}

void freeHashTable(HashTable* htable)
{
    free(htable->table); //Free mem
	
    htable->table = NULL; //Reset to NULL
    htable->size = 0; //Reset size to 0

    assert(getSize(htable) == 0); //Prob obselete
}

unsigned int getSize(const HashTable* htable)
{
    return htable->size; //Gets size of given table
}

void printHashTable(const HashTable* htable) //Prints table
{
    for (int i = 0; i < htable->size; i++) {
        if (htable->table[i].key != NULL) {
            printPerson(&htable->table[i].value, i);
        }
    }
}
