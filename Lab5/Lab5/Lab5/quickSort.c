	if (size <= 1) return;

	int tmp = arrayToSort[size - 1];

	int swapIndex = 0;

	for (int currentIndex = 0; lessThan(currentIndex, size - 1, statistics); currentIndex++)
	{
		if (lessThan(arrayToSort[currentIndex], tmp, statistics))
		{
			swapElements(&arrayToSort[currentIndex], &arrayToSort[swapIndex], statistics);

			swapIndex++;
		}
	}

	swapElements(&arrayToSort[swapIndex], &arrayToSort[size -1], statistics);

	quickSort(arrayToSort, swapIndex, statistics);
	quickSort(arrayToSort + swapIndex + 1, size - swapIndex - 1, statistics);