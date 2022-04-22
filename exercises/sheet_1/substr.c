/* gcc -Wall -O2 substr.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Prints a messages and exists with failure code
 *
 * @param serror Error message
 */
static void panic(const char *serror)
{
	fprintf(stderr, "%s", serror);
	exit(EXIT_FAILURE);
}

/**
 * @brief Allocates memory and panics on allocation error
 *
 * @param size Size in bytes to allocate
 * @return void* Pointer to allocated memory
 */
static void *xmalloc(size_t size)
{
	void *ptr;
	if (size == 0)
	{
		panic("Size is 0!\n");
	}
	ptr = malloc(size);
	if (!ptr)
	{
		panic("No mem left!\n");
	}
	return ptr;
}

/**
 * @brief Retrieves a substring of a string
 *
 * @param str The string to retrieve a substring from.
 * @param pos The zero-based starting character position of a substring in str.
 * @param len The number of characters in the substring.
 * @return char* A string that is equivalent to the substring of length len that begins at pos in str.
 */

size_t length_of_string(char chars[])
{
	size_t counter = 0;
	while (chars[counter] != 0)
	{
		++counter;
	}
	return counter;
}

static char *substring(const char *str, size_t pos, size_t len)
{
	size_t str_length = length_of_string(str);
	if (pos + len > str_length)
	{
		printf("ERROR: Size of string is %u. Can't create substring until index %u\n", str_length, pos + len);
		return NULL;
	}
	char *sub_str = malloc(sizeof(char) * (len + 1));
	for (size_t i = 0; i < len; ++i)
	{
		sub_str[i] = str[pos + i];
	}
	sub_str[pos + len] = '\0';

	return sub_str;
}

int main(int argc, char **argv)
{
	char *foo = "Nicht\n";
	char *bar = substring(foo, 2, 3);
	if (bar != NULL)
		printf("%s\n", bar);

	printf("length_of_string(foo)=%i", length_of_string(foo));
	printf("length_of_string(bar)=%i", length_of_string(bar));

	free(bar);
	return 0;
}
