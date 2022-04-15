/* gcc -Wall -O2 linked_list.c */
/* valgrind ./a.out */

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Element of a linked list
 * 
 */
struct Element {
	/**
	 * @brief Position of this element in the linked list
	 * 
	 */
	int pos;

	/**
	 * @brief Pointer to the next element in the linked list
	 * 
	 */
	struct Element *next;
};

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
	if (size == 0) {
		panic("Size is 0!\n");
	}
	ptr = malloc(size);
	if (!ptr) {
		panic("No mem left!\n");
	}
	return ptr;
}

/**
 * @brief Frees allocated memory space at an address returned by xmalloc
 * 
 * @param ptr Pointer to allocated memory space
 */
static void xfree(void *ptr)
{
	if (ptr == NULL){
		panic("Got Null-Pointer!\n");
	}
	free(ptr);
}

/**
 * @brief Initialises a linked list with a fixed length
 * 
 * @param len Lenght of the linked list
 * @return struct elem* Head of the linked list
 */
static struct Element* init_list(size_t len)
{
	struct Element* head = malloc(sizeof(struct Element));
	struct Element* curr = head;
	head->pos = 0;
	for(size_t i = 1; i < len; ++i){
		struct Element* new = malloc(sizeof(struct Element));
		new->pos = i;
		curr->next = new;
		curr=new;
	}
	curr->next =head;
	return head;
}

/**
 * @brief Frees all allocated memory of a linked list
 * 
 * @param head Head element of the linked list
 * @param len Length of the linked list
 */
static void clean_list(struct Element *head, size_t len)
{	
	struct Element* next = head;
	for(size_t i; i < len; ++i){
		struct Element* curr = next;
		next = curr -> next;
		free(curr);
	}
}

/**
 * @brief Traverses a linked list multiple times and outputs the iteration and list position for each iteration
 * 
 * @param head Head element of the linked list
 * @param times Amount of time the list should be traversed
 */
static void traverse_list(struct Element *head, int times)
{
	for(int i = 1; i <= times; ++i){
		struct Element* curr = head;
		do{
			printf("iter %i, elem %i\n", i, curr ->pos);
			curr = curr->next;
		}while(curr != head);
	}
}

int main(void)
{
	struct Element *head = NULL;
	size_t len = 10;

	head = init_list(len);
	traverse_list(head, 2);
	clean_list(head, len);

	return 0;
}

