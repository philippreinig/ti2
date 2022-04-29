/* gcc -w -o error error.c */

#include <stdio.h>
#include <stdlib.h>

/* This function returns the address of the local variable b, which is invalid after exiting the function because it
	is stored on the stack and thus it's memory is cleared. */

int *f(int a)
{
	int *b = malloc(sizeof(int));
	*b = 2 * a;
	return b;
}

int main(void)
{
	int *p4, *p8;
	p4 = f(4);
	p8 = f(8);
	printf("p4: %i / p8: %i\n", *p4, *p8);
	free(p4);
	free(p8);
}
