#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main() {
	int input = 0;

	assert(sum(1) == 1);
	assert(sum(2) == 3);
	assert(sum(3) == 6);
	assert(sum(4) == 10);
	assert(sum(5) == 15);
	assert(sum(20) == 210);
	assert(sum(0) == 0);
	assert(sum(-1) == 0);

	return 0;
}
int sum(int input) {

	int total = 0;

	if (input < 0)return 0;

	printf("Sum (%d) anropas\n", input);

	if (input != 1 && input > 0)total = sum(input - 1);
	total += input;

	printf("Sum (%d) retuneras \n", total);

	return total;
}
