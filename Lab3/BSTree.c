#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "BSTree.h"
#define _CRT_SECURE_NO_WARNINGS

/*Det Šr helt tillŒtet att lŠgga till egna hjŠlpfunktioner men de befintliga funktionerna fŒr inte Šndras*/

/* Statiska hjalpfunktioner anvands av andra funktioner i tradet och ska inte ligga i interfacet (anvandaren behover inte kanna till dessa) */
static struct treeNode* findNode(const BSTree tree, int data) {

	int last, currentLast;
	BSTree tmp = tree;

	while (1) {
		if(tmp == NULL) return NULL;
		else if (data == tmp->data) {
			return tmp;
		}
		else if (data > tmp->data) tmp = tmp->right;
		else if (data < tmp->data) tmp = tmp->left;
	}
}
static struct treeNode* prevNode(const BSTree tree, int data) {
	BSTree tmp = tree;
	BSTree prev = tree;
	while (1) {
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
static void single(BSTree* node, BSTree* previous) { 
	BSTree tmp = *node;
	BSTree prev = *previous;
	if (prev == tmp) {
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
static void leaf(BSTree* node, BSTree* previous) { //No branches, remove just free()
	//Just unlink and remove
	BSTree tmp = *node;
	BSTree prev = *previous;
	if (prev->left == tmp) {
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
static void doubleBranch(BSTree* node, const BSTree tree) { //double branch, remove
	//Find smallest element in left/right sub branch
	BSTree tmp = *node;
	BSTree rep = *node;
	int placeholder = 0;
	if (tmp->data == rep->data) rep = rep->left;

	while (1) {
		if (rep->data > tmp->data) {
			if (rep->left != NULL) rep = rep->left;
			else {
				placeholder = rep->data;
				if (rep->left != NULL) {
					BSTree prev = prevNode(tree, rep->data);
					single(&rep, prev);
					return;
				}
				else {
					BSTree prev = prevNode(tree, rep->data);
					leaf(&rep, prev);
				}
				tmp->data = placeholder;
				return;
			}
		}
		else if (rep->data < tmp->data) {
			if (rep->right != NULL) rep = rep->right;
			else {
				placeholder = rep->data;
				if (rep->left != NULL) {
					BSTree prev = prevNode(tree, rep->data);
					single(&rep, prev);
					return;
				}
				else {
					BSTree prev = prevNode(tree, rep->data);
					leaf(&rep, &prev);
				}
				tmp->data = placeholder;
				return;
			}
		}
	}
}

/* Skapar en tradnod med det givna datat genom att allokera minne for noden. Glom inte att initiera pekarna*/
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
static int sortArray(int* array, const BSTree tree) {
	if (isEmpty(tree) == 1)return 0;
	BSTree tmp = tree;
	int amount = 1;

	if (tmp == NULL) {
		return 0;
	}
	else if (tmp != NULL) {
		
		amount += sortArray(array, tmp->right);
		amount += sortArray(array, tmp->left);
		array[amount - 1] = tmp->data;
		return amount;
	}
}
/* Returnerar en dynamiskt allokerad array som innehaller tradets data sorterat */
static int* writeSortedToArray(const BSTree tree)
{
	int* sortedArray = NULL;
	BSTree tmp = tree;
	int i = 0;

	if (isEmpty(tree) == 1)return 0;
	
	int nrNodes = numberOfNodes(tree);

	sortedArray = (int*)malloc(nrNodes * sizeof(int));
	if (sortedArray == NULL) return;

	sortArray(sortedArray, tmp);
	for (int i = 0; i < nrNodes; i++) {
		printf("%d\n", sortedArray[i]);
	}
    /* Skapa en dynamisk array men ratt storlek
     
       Skriv datat frŒn tradet sorterat till arrayen (minsta till storsta)
       - till detta kanske du behover en hjalpfunktion */
    
    return sortedArray; //Ersatt med korrekt returvarde
}

/* Bygger upp ett sorterat, balanserat trad fran en sorterad array */
static void buildTreeSortedFromArray(BSTree* tree, const int arr[], int size)
{
	if (size < 0) return;

	int mid = (0 + size) / 2;

	insertSorted(tree, arr[mid]);

	//tmp->left = createNode(0);
	buildTreeSortedFromArray(tree, arr, mid -1);
	//tmp->right = createNode(0);
	buildTreeSortedFromArray(tree, arr + mid + 1, size - mid - 1);
	
    /* Bygg rekursivt fran mitten.
       Mittenelementet i en delarray skapar rot i deltradet
       Vanster delarray bygger vanster deltrad
       Hoger delarray bygger hoger deltrad*/
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
	/*Tank pa att tradet kan vara tomt vid insattning
      Du bestammer sjalv hur dubletter ska hanteras, ska de inte accepteras eller
      ska de laggas till vanster/hoger?.
      Post-condition kan verifieras med hjalp av find(...)*/
	if (data < 0) return;
	if (isEmpty(*tree) == 1) {
		*tree = createNode(data);
		return;
	}
	do {
		if (data < tmp->data) {
			if (tmp->left == NULL) {
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
		if (data == tmp->data) return;

	} while (data != tmp->data);

}

/* Utskriftsfunktioner
   Vid anrop: anvand stdio som andra argument for att skriva ut pŒ skarmen
   Det racker att ni implementerar LR ordningarna*/
void printPreorder(const BSTree tree, FILE *textfile)
{
	if (tree != NULL) {
		fprintf(textfile, "%d ", tree->data);
		printPreorder(tree->left, textfile);
		printPreorder(tree->right, textfile);
	}
}

void printInorder(const BSTree tree, FILE *textfile)
{
	if (tree != NULL) {
		printInorder(tree->left, textfile);
		fprintf(textfile, "%d ", tree->data);
		printInorder(tree->right, textfile);
	}
}

void printPostorder(const BSTree tree, FILE *textfile)
{
	if (tree != NULL) {
		printPostorder(tree->left, textfile);
		printPostorder(tree->right, textfile);
		fprintf(textfile, "%d ", tree->data);;
	}
}

/* Returnerar 1 om 'data' finns i tree, 0 annars */
int find(const BSTree tree, int data)
{
	BSTree tmp = tree;

	if (isEmpty(tree) == 1)return 0;
	tmp = findNode(tmp, data);
	if (tmp == NULL) return 0;
	else if (data == tmp->data) return 1;

	return 1; //Ersatt med korrekt returvarde
}

/* Tar bort 'data' fran tradet om det finns */
void removeElement(BSTree* tree, int data)
{
	if (isEmpty(*tree) == 1)return;

	BSTree tmp = *tree;
	BSTree prev = prevNode(tmp, data);
	/* Inget data ska/kan tas bort fran ett tomt trad
     Tre fall: Ett lov (inga barn), ett barn (vanster eller hoger), tva barn
     Glom inte att frigora noden nar den lankats ur tradet*/
	tmp = findNode(tmp, data);

	if (tmp->right == NULL && tmp->left == NULL) {
		if (prev == tmp) {
			free(*tree);
			*tree = NULL;
			return;
		}
		leaf(&tmp, &prev);
	}
	else if (tmp->right != NULL && tmp->left != NULL) doubleBranch(&tmp, *tree);
	else if (tmp->right != NULL || tmp->left != NULL) single(&tmp, &prev);
}

/* Returnerar hur manga noder som totalt finns i tradet */
int numberOfNodes(const BSTree tree)
{
	if (isEmpty(tree) == 1)return 0;
	BSTree tmp = tree;
	int amount = 1;
	
	if (tmp == NULL){
		return 0;
	}
	else if(tmp != NULL) {
		amount += numberOfNodes(tmp->right);
		amount += numberOfNodes(tmp->left);
		//printf("Amount: %d Node: %d\n", amount, tmp->data);
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
		rightDepth = depth(tmp->right);
		leftDepth = depth(tmp->left);

		if (leftDepth > rightDepth) return (leftDepth + 1);
		else return(rightDepth + 1);
	}
}

/* Returnerar minimidjupet for tradet
   Se math.h for anvandbara funktioner*/
int minDepth(const BSTree tree)
{
	BSTree tmp = tree;
	int rightDepth = 1, leftDepth = 1;

	if (tmp == NULL) return 0;

	if (tmp->left == NULL && tmp->right == NULL) return 1;

	if (tmp->left != NULL) {
		leftDepth = minDepth(tmp->left);
	}
	if (tmp->right != NULL) {
		rightDepth = minDepth(tmp->right);
	}
	
	//printf("%d ----%d---- %d \n", leftDepth, min(leftDepth, rightDepth) + 1, rightDepth);
	return min(leftDepth, rightDepth) + 1;
}

/* Balansera tradet sa att depth(tree) == minDepth(tree) */
void balanceTree(BSTree* tree)
{
	int nrNodes = 0; //orignial nr nodes
	int* sortedArray;

	if (isEmpty(*tree) == 1)return;

	nrNodes = numberOfNodes(*tree);
	/* Forslag pa algoritm:
	   - overfor tradet till en dynamiskt allokerad array med writeSortedToArray()
	   - tom tradet med freeTree()
	   - bygg upp tradet rekursivt fran arrayen med buildTreeSortedFromArray()
	   - frigor minne for den dynamiskt allokerade arrayen*/

	sortedArray = writeSortedToArray(*tree);
	freeTree(tree);
	buildTreeSortedFromArray(tree, sortedArray, nrNodes);
	printf("%d---------%d\n", depth(*tree), minDepth(*tree));
	free(sortedArray);

	 /*  Post - conditions:
	   - tree har lika manga noder som tidigare
	   - djupet for tradet ar samma som minimumdjupet for tradet */
	//assert(nrNodes == numberOfNodes(*tree));
}

/* Tom tradet och frigor minnet for de olika noderna */
void freeTree(BSTree* tree)
{
	if (isEmpty(tree) == 1)return 0;
	
	BSTree tmp = *tree;

	if (tmp == NULL) return;

	freeTree(&tmp->right);
	freeTree(&tmp->left);

	free(*tree);
	*tree = NULL;

	assert(isEmpty(*tree) == 1);
	// Post-condition: tradet ar tomt
}


