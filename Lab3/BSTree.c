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
static struct treeNode* prevNode(const BSTree tree, int data) { //Icke rekursiv som hittar värdet på föräldern till noden wdd
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

	int mid = (size) / 2; //Delar med 2 för att komma till mitten

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
	else if (tmp->right != NULL && tmp->left != NULL) doubleBranchDelete(tree, data); //Ta bort gren(två barn)
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

	buildTreeSortedFromArray(tree, sortedArray, nrNodes-1); //Hjälpfunktion
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


