#define _CRT_SECURE_NO_WARNINGS
#include "SortingAlgorithms.h"
#include "Statistics.h"
#include "Array.h"
#include <assert.h>
#include <string.h>

int isImplemented(SortingAlgorithm algorithm)
{
	switch (algorithm)
	{
      case BUBBLE_SORT:
      case INSERTION_SORT:
      case SELECTION_SORT:
      //case QUICK_SORT:
      case MERGE_SORT:
            return 1;
        default:
            return 0;
	}
}

/*Antalet byten i en och samma algoritm kan vara olika beroende på implementationen. Ibland ligger datat redan på rätt plats och då kan man välja att testa det och inte göra ett byte (vilket ger extra jämförelse) eller så kan man ändå göra ett byte (med sig själv). Följer man de algoritmer som vi gått igenom på föreläsningarna exakt så gör man en swap även på ett element som ligger på rätt plats
 
    När du analyserar det data som genereras (result.txt) så behöver du ha detta i åtanke */

/******************************************************************************************/
/* Era algoritmer har: */

static void bubbleSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics)
{
	for (int i = 0; lessThan(i, size - 1, statistics); i++) { //Go through all elements
		for (int j = 0; lessThan(j, size - i - 1, statistics); j++) { //Compare all elements to each other
			if (greaterThan(arrayToSort[j], arrayToSort[j + 1], statistics)) {
				swapElements(&arrayToSort[j], &arrayToSort[j+1], statistics);
			}
		}
	}
}

static void insertionSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics)
{
	for (int i = 1; lessThan(i, size, statistics); i++) { //Start at 1 since first element is 'sorted'
		int tmp = arrayToSort[i];
		int j = i - 1;

		while (greaterThanOrEqualTo(j, 0, statistics) && greaterThan(arrayToSort[j], tmp, statistics)) { //While loop used as a combo while/if
			arrayToSort[j + 1] = arrayToSort[j]; //Shift elemnt to the right in array
			j = j - 1; //Next element
		}
		swapElements(&arrayToSort[j + 1], &tmp, statistics); //Best way to use swap element i guess
	}
}

static void selectionSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics)
{
	for (int i = 0; lessThan(i, size - 1, statistics); i++) {
		int min = i;
		for (int j = i + 1; lessThan(j, size, statistics); j++) { //Find smallest element

			if (lessThan(arrayToSort[j], arrayToSort[min], statistics)) {
				min = j;
			}
		}
		swapElements(&arrayToSort[min], &arrayToSort[i], statistics);
	}
}


//No clue if you should and in that case how you would implement swapElements()
static void mergeSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics) //Similar structure to Lab4 writeFromArray
{
	if (size <= 1) return; //If elements in the array only has 1 element its already "sorted"
	

	unsigned int mid = size / 2; //Middle

	ElementType* leftArray = arrayToSort; //Left array, just array
	ElementType* rightArray = arrayToSort + mid;//Right sub array moved (mid) to the righ

	mergeSort(leftArray, mid, statistics); //Splitting/creating sub arrays further
	mergeSort(rightArray, size - mid, statistics);

	ElementType* tmpArray = malloc(size * sizeof(ElementType)); //Temporary array 
	if (tmpArray == NULL) return;

	unsigned int i = 0; //Used for left array
	unsigned int j = 0; //Used for right array
	unsigned int k = 0; //Used for tmp array
	//while (i < mid && j < size - mid) { //Checking so that it does not go outside of respective array
	while (lessThan(i, mid, statistics) && lessThan(j, size - mid, statistics)) { //Checking so that it does not go outside of respective array
		if (leftArray[i] <= rightArray[j]) { //Checking if left is smaller or equal to righ array
			tmpArray[k] = leftArray[i]; //Adding leftArray[i] to tmpArray
			i++; //Next index
		}
		else {
			tmpArray[k] = rightArray[j]; //Adding rightArray[j] to tmpArray
			j++; //Next
		}
		k++;
	}

	while (lessThan(i, mid, statistics)) { //If right array index is outside allow range
		tmpArray[k] = leftArray[i];
		i++;
		k++;
	}

	while (lessThan(j, size - mid, statistics)) { //If left array index is outside allow range
		tmpArray[k] = rightArray[j];
		j++;
		k++;
	}

	for (unsigned int i = 0; lessThan(i, size, statistics); i++) { //Adding elements to original array, dont know if lessThan() should be used here
		arrayToSort[i] = tmpArray[i];
	}

	free(tmpArray); //Freeing mem of tmpArray
}

static void quickSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics)
{

}


/******************************************************************************************/


char* getAlgorithmName(SortingAlgorithm algorithm)
{
	/* Ar inte strangen vi allokerar lokal for funktionen?
	   Nej, inte i detta fall. Strangkonstanter ar ett speciallfall i C */
	switch (algorithm)
	{
        case BUBBLE_SORT:	 return "  Bubble sort ";
        case SELECTION_SORT: return "Selection sort";
        case INSERTION_SORT: return "Insertion sort";
        case MERGE_SORT:	 return "  Merge sort  ";
        case QUICK_SORT:	 return "  Quick sort  ";
        default: assert(0 && "Ogiltig algoritm!"); return "";
	}
}

// Sorterar 'arrayToSort' med 'algorithmToUse'. Statistik for antal byten och jamforelser hamnar i *statistics
static void sortArray(ElementType* arrayToSort, unsigned int size, SortingAlgorithm algorithmToUse, Statistics* statistics)
{
	if(statistics != NULL)
		resetStatistics(statistics);

	switch (algorithmToUse)
	{
	case BUBBLE_SORT:	 bubbleSort(arrayToSort, size, statistics); break;
	case SELECTION_SORT: selectionSort(arrayToSort, size, statistics); break;
	case INSERTION_SORT: insertionSort(arrayToSort, size, statistics); break;
	case MERGE_SORT:	 mergeSort(arrayToSort, size, statistics); break;
	case QUICK_SORT:	 quickSort(arrayToSort, size, statistics); break;
	default:
		assert(0 && "Ogiltig algoritm!");
	}
}

// Forbereder arrayer for sortering genom att allokera minne for dem, samt intialisera dem
static void prepareArrays(SortingArray sortingArray[], SortingAlgorithm algorithm, const ElementType* arrays[], const unsigned int sizes[])
{
	assert(isImplemented(algorithm));

	int i;
	int totalArraySize;

	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		totalArraySize = sizeof(ElementType)*sizes[i];
		sortingArray[i].arrayToSort = malloc(totalArraySize);
		memcpy(sortingArray[i].arrayToSort, arrays[i], totalArraySize);

		sortingArray[i].algorithm = algorithm;
		sortingArray[i].arraySize = sizes[i];
		resetStatistics(&sortingArray[i].statistics);
	}
}

// Sorterar arrayerna
static void sortArrays(SortingArray toBeSorted[])
{
	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		SortingArray* current = &toBeSorted[i];
		sortArray(current->arrayToSort, current->arraySize, current->algorithm, &current->statistics);
	
		if (!isSorted(current->arrayToSort, current->arraySize))
		{
			printf("Fel! Algoritmen %s har inte sorterat korrekt!\n", getAlgorithmName(current->algorithm));
			printf("Resultatet ‰r: \n");
			printArray(current->arrayToSort, current->arraySize, stdout);
			assert(0); // Aktiveras alltid
		}
	}
}

void printResult(SortingArray sortedArrays[], FILE* file)
{
	assert(file != NULL);

	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		fprintf(file, "%4d element: ", sortedArrays[i].arraySize);
		printStatistics(&sortedArrays[i].statistics, file);
		fprintf(file, "\n");
	}
	fprintf(file, "\n");
}

void sortAndPrint(SortingArray sortingArray[], SortingAlgorithm algorithm, const ElementType* arrays[], unsigned int sizes[], FILE* file)
{
	assert(file != NULL);

	prepareArrays(sortingArray, algorithm, arrays, sizes);
	sortArrays(sortingArray);
	printResult(sortingArray, file);
}

void freeArray(SortingArray sortingArray[])
{
	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		if (sortingArray[i].arrayToSort != NULL)
			free(sortingArray[i].arrayToSort);
		sortingArray[i].arrayToSort = NULL;
		sortingArray[i].arraySize = 0;
		resetStatistics(&sortingArray[i].statistics);
	}
}
