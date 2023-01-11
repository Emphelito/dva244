#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "BSTree.h"
#define _CRT_SECURE_NO_WARNINGS


static struct treeNode* findNode(const BSTree tree, int data) { //Tidig funktion som inte �r rekursiv

	//int last, currentLast;
	BSTree tmp = tree;

	while (tmp != NULL) { //while som forts�tter tills den hittar v�rdet eller till det inte finns n�gon nod kvar
		if (data == tmp->data) {
			return tmp;
		}
		else if (data > tmp->data) tmp = tmp->right;
		else if (data < tmp->data) tmp = tmp->left;
	}
}
static struct treeNode* prevNode(const BSTree tree, int data) { //Icke rekursiv som hittar v�rdet p� f�r�ldern till noden wdd
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
static void singleDelete(BSTree* node, BSTree* prev, BSTree* tree) { //Single branch
	if (*node == NULL) 	return;

	if (*node == *prev) { //When node to remove is root
		if ((*node)->left == NULL) {
			*tree = (*prev)->right;
			free(*node);
			*node = NULL;
			return;
		}
		else {
			*tree = (*prev)->left;
			free(*node);
			*node = NULL;
			return;
		}
	}

	if ((*node)->left == NULL) { // the node has only a right child
		BSTree rightChild = (*node)->right;
		free(*node);
		if (rightChild->data > (*prev)->data) {
			(*prev)->right = rightChild;
		}
		else (*prev)->left = rightChild;
	}
	else if ((*node)->right == NULL) { // the node has only a left child
		BSTree leftChild = (*node)->left;
		free(*node);
		if (leftChild->data > (*prev)->data) {
			(*prev)->right = leftChild;
		}
		else (*prev)->left = leftChild;
	}
	
}
static void leafDelete(BSTree* node, BSTree* prev) { //Delete single node with no branches/children
	//Just unlink and remove
	if (*prev != NULL) {
		if (*node == (*prev)->left) {
			(*prev)->left = NULL;
		}
		else {
			(*prev)->right = NULL;
		}
	}
	free(*node);
	*node = NULL;
}
static void doubleBranchDelete(BSTree* tree, int data) { 
	
	BSTree repl; //Used for replacement node
	BSTree node = *tree; 

	while (node->data != data && node != NULL) { //Moves to node to delete
		if (data < node->data) { //Compares size of data
			node = node->left;
		}
		else
			node = node->right;
	}
	BSTree tmp = node->right; //Moves to right sub tree
	repl = tmp;
	while (tmp != NULL) { //smallest node in right sub tree
		repl = tmp;
		tmp = tmp->left;
	}
	int newData = repl->data;
	BSTree prevnodeFunc = prevNode(*tree, repl->data); //Used in del functions

	if (repl->left == NULL && repl->right == NULL) {
		leafDelete(&repl, &prevnodeFunc); //Called if replacment node has no children
	}
	else {
		singleDelete(&repl, &prevnodeFunc, tree); //Called if replacment has one child
	}
	node->data = newData; //Assigns replacement data to node to delete. Frees replacment node in previous if() 
}

static struct treeNode* createNode(int data)
{
	BSTree tmp = NULL;

	tmp = malloc(sizeof(struct treeNode));
	assert(tmp != NULL);

	tmp->data = data;
	tmp->left = NULL;
	tmp->right = NULL;

	return tmp; 
}
static void sortArray(int* array, const BSTree tree, int* pos) { //Hj�lpfunktion till writeSortedToArray

	if (tree == NULL) {
		return;
	}
	
	sortArray(array, tree->left, pos); //G�r igenom det v�nstra tr�det/gren

	array[*pos] = tree->data; 
	(*pos)++; //Anv�nder en pointer f�r att kunna �ndra v�rdet i alla instanser



	sortArray(array, tree->right, pos);//G�r igenom det h�gra tr�det/gren
}
/* Returnerar en dynamiskt allokerad array som innehaller tradets data sorterat */
static int* writeSortedToArray(const BSTree tree)
{
	int* sortedArray = NULL;
	BSTree tmp = tree;
	int i = 0; 

	if (isEmpty(tree) == 1)return 0;
	
	int nrNodes = numberOfNodes(tree); //Tar reda p� m�ngden noder som finns

	sortedArray = (int*)malloc(nrNodes * sizeof(int)); //Allokerar minne f�r sortedArray
	if (sortedArray == NULL) return;

	sortArray(sortedArray, tmp, &i);//Anv�nder hj�lpfunktion
    
    return sortedArray; 
}

/* Bygger upp ett sorterat, balanserat trad fran en sorterad array */
static void buildTreeSortedFromArray(BSTree* tree, const int arr[], int size)
{
	if (size < 0) return;

	int mid = (size) / 2; //Delar med 2 f�r att komma till mitten

	insertSorted(tree, arr[mid]);//Anv�nder insertSorted() f�r att l�gga till noder

	buildTreeSortedFromArray(tree, arr, mid -1); //flyttar mid med ett steg �t v�nster

	buildTreeSortedFromArray(tree, arr + mid + 1, size - mid - 1); //Flyttar arr �t h�ger, �nda s�ttet som jag kunda komma �t allt �t h�ger om orginela mitten
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
	if (isEmpty(*tree) == 1) {
		*tree = createNode(data);
		assert(find(*tree, data) == 1);
		return;
	}
	if (data <= (*tree)->data) {
		insertSorted(&(*tree)->left, data);
	}
	else {
		insertSorted(&(*tree)->right, data);
	}
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
		fprintf(textfile, "%d ", tree->data);//Skriver data efter den har g�tt igenom v�nster deltr�d
		printInorder(tree->right, textfile);
	}
}

void printPostorder(const BSTree tree, FILE *textfile)
{
	if (tree != NULL) {
		printPostorder(tree->left, textfile);
		printPostorder(tree->right, textfile);
		fprintf(textfile, "%d ", tree->data);;//Skriver data efter den har g�tt igenom alla deltr�d
	}
}

int find(const BSTree tree, int data)
{
	BSTree tmp = tree;

	if (isEmpty(tree) == 1)return 0;
	tmp = findNode(tmp, data); //Hj�lpfunktion som anv�nds av andra funktioner ocks� eftersom den retunerar noden
	if (tmp == NULL) return 0;
	else if (data == tmp->data) return 1; //Kollar om den retunerade noden har samma v�rde som de vi letar efter

	return 0; 
}

void removeElement(BSTree* tree, int data)
{
	if (isEmpty(*tree) == 1)return;

	BSTree tmp = *tree;
	BSTree prev = prevNode(tmp, data); //Hittar noden som kom innan noden som ska tas bort

	tmp = findNode(tmp, data); //F�r noden fr�n hj�lpfunktion

	if (tmp->right == NULL && tmp->left == NULL) { //Om det �r ett l�v
		if (prev == tmp) { //om det �r rooten
			free(*tree);
			*tree = NULL;
			return;
		}
		leafDelete(&tmp, &prev); //Ta bort vanligt l�v
	}
	else if (tmp->right != NULL && tmp->left != NULL) doubleBranchDelete(tree, data); //Ta bort gren(tv� barn)
	else if (tmp->right != NULL || tmp->left != NULL) singleDelete(&tmp, &prev, tree); //Ta bort gren(ett barn)
}

/* Returnerar hur manga noder som totalt finns i tradet */
int numberOfNodes(const BSTree tree)
{
	if (isEmpty(tree) == 1)return 0;
	return numberOfNodes(tree->left) + numberOfNodes(tree->right) + 1;
	
}

/* Returnerar hur djupt tradet ar */
int depth(const BSTree tree)
{
	if (isEmpty(tree) == 1)return 0;

	BSTree tmp = tree;
	int rightDepth, leftDepth;

	if (tmp == NULL) return 0;
	else {
		rightDepth = depth(tmp->right); //Djupet p� h�ger deltr�d
		leftDepth = depth(tmp->left); //Djupet p� v�nster deltr�d

		if (leftDepth > rightDepth) return (leftDepth + 1);
		else return(rightDepth + 1); //Kunde ha anv�nt max() men t�nkte inte p� det. kollar vilken som  �r st�rst och l�gger till 1(rotten)
	}
}

int minDepth(const BSTree tree)
{
	BSTree tmp = tree;

	if (tmp == NULL) return 0;

	double min = log2(numberOfNodes(tree) + 1); //Log2 av m�ngde noder + 1

	min = ceil(min); //Rundar upp�t ex. 2.2 = 3

	return min;
}

void balanceTree(BSTree* tree)
{
	int nrNodes = 0; 
	int* sortedArray;

	if (isEmpty(*tree) == 1)return;

	nrNodes = numberOfNodes(*tree); //M�ngden noder

	sortedArray = writeSortedToArray(*tree); //Hj�lpfunktion

	freeTree(tree);//T�mmer tr�d

	buildTreeSortedFromArray(tree, sortedArray, nrNodes-1); //Hj�lpfunktion
	free(sortedArray);//Frig�r array

	assert(depth(*tree) == minDepth(*tree));//Kollar s� att tr�det �r balanserat
	assert(nrNodes == numberOfNodes(*tree));//Kollar s� att ingen nod f�rsvan
}

void freeTree(BSTree* tree)
{
	if (isEmpty(tree) == 1)return 0;
	
	BSTree tmp = *tree;

	if (tmp == NULL) return;

	freeTree(&tmp->right);//Rekursivt g�r igenom alla noder
	freeTree(&tmp->left);

	free(*tree);//Frig�r minne
	*tree = NULL;

	assert(isEmpty(*tree) == 1); //Kollar s� det �r tommt
}


