#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "BSTree.h"
#define _CRT_SECURE_NO_WARNINGS


static struct treeNode* findNode(const BSTree tree, int data) { //Tidig funktion som inte är rekursiv

	//int last, currentLast;
	BSTree tmp = tree;

	while (tmp != NULL) { //while som fortsätter tills den hittar värdet eller till det inte finns någon nod kvar
		if (data == tmp->data) {
			return tmp;
		}
		else if (data > tmp->data) tmp = tmp->right;
		else if (data < tmp->data) tmp = tmp->left;
	}
}
static struct treeNode* prevNode(const BSTree tree, int data) { //Icke rekursiv som hittar värdet på föräldern till noden
	BSTree tmp = tree;
	BSTree prev = tree;
	while (tmp != NULL) {
		if (tmp->data == data) {
			return prev;
		}
		else if (data > tmp->data) {
			prev = tmp;
			tmp = tmp->right;
		}
		else if (data < tmp->data) {
			prev = tmp;
			tmp = tmp->left;
		}
	}
}
static void singleDelete(BSTree* node, BSTree* previous) { //Dubbla grenar
	BSTree tmp = *node;
	BSTree prev = *previous;
	if (prev == tmp) { //När rotten ska tas bort men det finns 1 barn
		if (tmp->left != NULL) {
			tmp = tmp->left;
			prev->data = tmp->data;
			free(tmp);
			prev->left = NULL;
		}
		else {
			tmp = tmp->right;
			prev->data = tmp->data;
			free(tmp);
			prev->right = NULL;
		}
		return;
	}
	if (tmp->left != NULL) {
		if (prev->left != NULL && prev->left->data == tmp->data) {
			prev->left = tmp->left;
			free(tmp);
			tmp = NULL;
		}
		else {
			prev->right = tmp->left;
			free(tmp);
			tmp = NULL;
		}
	}
	else {
		if (prev->left->data == tmp->data) {
			prev->left = tmp->right;
			free(tmp);
			tmp = NULL;
		}
		else {
			prev->right = tmp->right;
			free(tmp);
			tmp = NULL;
		}
	}
}
static void leafDelete(BSTree* node, BSTree* previous) { 
	//Just unlink and remove
	BSTree tmp = *node;
	BSTree prev = *previous;
	if (prev->left == tmp) {//Kollar om löver är på vänster eller höger sida av sin förälder
		free(tmp);
		prev->left = NULL;
	}
	else if(prev->right == tmp) {
		free(tmp);
		prev->right = NULL;
	}
	else {
		free(prev);
		tmp = NULL;
	}

}
static void doubleBranchDelete(BSTree* node, const BSTree tree) {
	BSTree tmp = *node; //Noden som ska tas bort
	BSTree rep = *node; //Noden som ska bytta ut noden som ska tas bort
	int placeholder = 0;
	if (tmp->data == rep->data) rep = rep->left;

	while (1) {
		if (rep->data > tmp->data) {
			if (rep->left != NULL) rep = rep->left; //Om det går att gå ner åt vänster, gör det
			else {
				placeholder = rep->data;
				if (rep->left != NULL) { //Om den har 1 barn kalla single()
					BSTree prev = prevNode(tree, rep->data);
					singleDelete(&rep, prev);
					return;
				}
				else { //Inger barn använd leaf()
					BSTree prev = prevNode(tree, rep->data);
					leafDelete(&rep, prev);
				}
				tmp->data = placeholder;
				return;
			}
		}
		else if (rep->data < tmp->data) {
			if (rep->right != NULL) rep = rep->right;//Om det går att gå ner åt höger, gör det
			else {
				placeholder = rep->data; //Assigna värdet från lövet till en placeholder
				if (rep->left != NULL) {
					BSTree prev = prevNode(tree, rep->data);
					singleDelete(&rep, prev);
					return;
				}
				else {
					BSTree prev = prevNode(tree, rep->data);
					leafDelete(&rep, &prev);
				}
				tmp->data = placeholder; //Använd placeholder
				return;
			}
		}
	}
}

static struct treeNode* createNode(int data)
{
	BSTree tmp = NULL;

	tmp = (BSTree*)malloc(200);
	assert(tmp != NULL);

	tmp->data = data;
	tmp->left = NULL;
	tmp->right = NULL;

	return tmp; 
}
static void sortArray(int* array, const BSTree tree, int* pos) { //Hjälpfunktion till writeSortedToArray

	if (tree == NULL) {
		return;
	}
	
	sortArray(array, tree->left, pos); //Går igenom det vänstra trädet/gren

	array[*pos] = tree->data; 
	(*pos)++; //Använder en pointer för att kunna ändra värdet i alla instanser



	sortArray(array, tree->right, pos);//Går igenom det högra trädet/gren
}
/* Returnerar en dynamiskt allokerad array som innehaller tradets data sorterat */
static int* writeSortedToArray(const BSTree tree)
{
	int* sortedArray = NULL;
	BSTree tmp = tree;
	int i = 0; 

	if (isEmpty(tree) == 1)return 0;
	
	int nrNodes = numberOfNodes(tree); //Tar reda på mängden noder som finns

	sortedArray = (int*)malloc(nrNodes * sizeof(int)); //Allokerar minne för sortedArray
	if (sortedArray == NULL) return;

	sortArray(sortedArray, tmp, &i);//Använder hjälpfunktion
    
    return sortedArray; 
}

/* Bygger upp ett sorterat, balanserat trad fran en sorterad array */
static void buildTreeSortedFromArray(BSTree* tree, const int arr[], int size)
{
	if (size < 0) return;

	int mid = (0 + size) / 2; //Delar med 2 för att komma till mitten

	insertSorted(tree, arr[mid]);//Använder insertSorted() för att lägga till noder

	buildTreeSortedFromArray(tree, arr, mid -1); //flyttar mid med ett steg åt vänster

	buildTreeSortedFromArray(tree, arr + mid + 1, size - mid - 1); //Flyttar arr åt höger, ända sättet som jag kunda komma åt allt åt höger om orginela mitten
}


/* Implementation av tradet, funktionerna i interfacet */

/* Skapar ett tomt trad - denna funktion ar fardig */
BSTree emptyTree(void)
{
	return NULL;
}

/* Returnerar 1 ifall tradet ar tomt, 0 annars */
int isEmpty(const BSTree tree)
{
	if (tree == NULL)return 1;
	return 0; // Ersatt med ratt returvarde
}

/* Satter in 'data' sorterat i *tree
 Post-condition: data finns i tradet*/
void insertSorted(BSTree* tree, int data)
{
	BSTree tmp = *tree;
	if (data < 0) return;
	if (isEmpty(*tree) == 1) {
		*tree = createNode(data);
		return;
	}
	do {
		if (data < tmp->data) { //Kollar om värdet är mindre än tmp
			if (tmp->left == NULL) { //Ifall tmp är tom skapa en nod
				tmp->left = createNode(data);
				return;
			}
			else {
				tmp = tmp->left;
			}
		}
		if (data > tmp->data) {
			if (tmp->right == NULL) {
				tmp->right = createNode(data);
				return;
			}
			else {
				tmp = tmp->right;
			}
		}
		if (data == tmp->data) return;//Dubblet

	} while (data != tmp->data);//Do while för att jag tänkte addera dubbleter till trädet från början

	assert(find(*tree, data) == 1);
}

void printPreorder(const BSTree tree, FILE *textfile)
{
	if (tree != NULL) {
		fprintf(textfile, "%d ", tree->data);//Skriver data innan den har bytt pos 
		printPreorder(tree->left, textfile);
		printPreorder(tree->right, textfile);
	}
}

void printInorder(const BSTree tree, FILE *textfile)
{
	if (tree != NULL) {
		printInorder(tree->left, textfile); 
		fprintf(textfile, "%d ", tree->data);//Skriver data efter den har gått igenom vänster delträd
		printInorder(tree->right, textfile);
	}
}

void printPostorder(const BSTree tree, FILE *textfile)
{
	if (tree != NULL) {
		printPostorder(tree->left, textfile);
		printPostorder(tree->right, textfile);
		fprintf(textfile, "%d ", tree->data);;//Skriver data efter den har gått igenom alla delträd
	}
}

int find(const BSTree tree, int data)
{
	BSTree tmp = tree;

	if (isEmpty(tree) == 1)return 0;
	tmp = findNode(tmp, data); //Hjälpfunktion som används av andra funktioner också eftersom den retunerar noden
	if (tmp == NULL) return 0;
	else if (data == tmp->data) return 1; //Kollar om den retunerade noden har samma värde som de vi letar efter

	return 0; 
}

void removeElement(BSTree* tree, int data)
{
	if (isEmpty(*tree) == 1)return;

	BSTree tmp = *tree;
	BSTree prev = prevNode(tmp, data); //Hittar noden som kom innan noden som ska tas bort

	tmp = findNode(tmp, data); //Får noden från hjälpfunktion

	if (tmp->right == NULL && tmp->left == NULL) { //Om det är ett löv
		if (prev == tmp) { //om det är rooten
			free(*tree);
			*tree = NULL;
			return;
		}
		leafDelete(&tmp, &prev); //Ta bort vanligt löv
	}
	else if (tmp->right != NULL && tmp->left != NULL) doubleBranchDelete(&tmp, *tree); //Ta bort gren(två barn)
	else if (tmp->right != NULL || tmp->left != NULL) singleDelete(&tmp, &prev); //Ta bort gren(ett barn)
}

/* Returnerar hur manga noder som totalt finns i tradet */
int numberOfNodes(const BSTree tree)
{
	if (isEmpty(tree) == 1)return 0;
	BSTree tmp = tree;
	int amount = 1; //För att det alltid ska läggas till 1 om noden inte är tom
	
	if (tmp == NULL){
		return 0;
	}
	else if(tmp != NULL) {
		amount += numberOfNodes(tmp->right); //Går rekrusivt igenom alla noder som inte är tomma och retunerar 1.
		amount += numberOfNodes(tmp->left);

		return amount;
	}
	
}

/* Returnerar hur djupt tradet ar */
int depth(const BSTree tree)
{
	if (isEmpty(tree) == 1)return 0;

	BSTree tmp = tree;
	int rightDepth, leftDepth;

	if (tmp == NULL) return 0;
	else {
		rightDepth = depth(tmp->right); //Djupet på höger delträd
		leftDepth = depth(tmp->left); //Djupet på vänster delträd

		if (leftDepth > rightDepth) return (leftDepth + 1);
		else return(rightDepth + 1); //Kunde ha använt max() men tänkte inte på det. kollar vilken som  är störst och lägger till 1(rotten)
	}
}

int minDepth(const BSTree tree)
{
	BSTree tmp = tree;

	if (tmp == NULL) return 0;

	double min = log2(numberOfNodes(tree) + 1); //Log2 av mängde noder + 1

	min = ceil(min); //Rundar uppåt ex. 2.2 = 3

	return min;
}

void balanceTree(BSTree* tree)
{
	int nrNodes = 0; 
	int* sortedArray;

	if (isEmpty(*tree) == 1)return;

	nrNodes = numberOfNodes(*tree); //Mängden noder

	sortedArray = writeSortedToArray(*tree); //Hjälpfunktion

	freeTree(tree);//Tömmer träd

	buildTreeSortedFromArray(tree, sortedArray, nrNodes); //Hjälpfunktion
	free(sortedArray);//Frigör array

	assert(depth(*tree) == minDepth(*tree));//Kollar så att trädet är balanserat
	assert(nrNodes == numberOfNodes(*tree));//Kollar så att ingen nod försvan
}

void freeTree(BSTree* tree)
{
	if (isEmpty(tree) == 1)return 0;
	
	BSTree tmp = *tree;

	if (tmp == NULL) return;

	freeTree(&tmp->right);//Rekursivt går igenom alla noder
	freeTree(&tmp->left);

	free(*tree);//Frigör minne
	*tree = NULL;

	assert(isEmpty(*tree) == 1); //Kollar så det är tommt
}


