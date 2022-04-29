/* gcc -Wall net.c */

#include <stdio.h>
#include <stdint.h>

int main(void)
{
	uint8_t add[] = {129, 169, 25, 100, 54};
	uint8_t sub[] = {255, 255, 240, 0};
	uint8_t net[4];

	/* ... */

	printf("sizeof(add)=%lu\n", sizeof(add));
	printf("sizeof(sub)=%lu\n", sizeof(sub));
	printf("sizeof(net)=%lu\n", sizeof(net));
	printf("sizeof(add[0])=%lu\n", sizeof(add[0]));
	printf("sizeof(sub[0])=%lu\n", sizeof(sub[0]));
	printf("sizeof(net[0]=%lu\n", sizeof(net[0]));

	for (int i = 0; i < sizeof(add) / sizeof(add[0]); ++i)
	{
		net[i] = add[i] & sub[i];
		printf("%u: %u\t&\t%u\t=\t%u\n", i, add[i], sub[i], net[i]);
	}

	printf("net address: %u.%u.%u.%u\n", net[0], net[1], net[2], net[3]);

	return 0;
}

// unsigned int int_to_bin (unsigned int x) {
//     return (x == 0 || x == 1 ? x : (x % 2 + 10 * int_to_bin(x / 2)));
// }
