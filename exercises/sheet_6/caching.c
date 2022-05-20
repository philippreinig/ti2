/* gcc -Wall -O2 caching.c -o caching */
/* sudo perf stat ./caching 0 */
/* sudo perf stat ./caching 1 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static void f(char **matrix, size_t x, size_t y) {
  /* Traverse data column major and increase every field in the matrix by one */
  for(size_t i = 0; i < x; ++i){
    for(size_t j = 0; j < y; ++j){
      matrix[i][j] += 1;
    }
  }
}

static void g(char **matrix, size_t x, size_t y) {
  /* Traverse data column major and increase every field in the matrix by one */
  for(size_t i = 0; i < y; ++i){
    for(size_t j = 0; j < x; ++j){
      matrix[j][i] += 1;
    }
  }
}

int main(int argc, char **argv) {
  char **matrix;
  size_t x, y;
  size_t i;

  if (argc != 2) {
    printf("Usage: ./caching <test-nr:0|1>\n");
    return -EIO;
  }

  x = y = sysconf(_SC_PAGE_SIZE) * 5;
  matrix = valloc(x * sizeof(char *));
  if (matrix == NULL) {
    return -ENOMEM;
  }

  for (i = 0; i < x; i++) {
    matrix[i] = malloc(y * sizeof(char));
    if (matrix[i] == NULL) {
      return -ENOMEM;
    }
  }

  switch (atoi(argv[1])) {
    default:
    case 0:
      f(matrix, x, y);
      break;
    case 1:
      g(matrix, x, y);
      break;
  }

  for (i = 0; i < x; i++) {
    free(matrix[i]);
  }
  free(matrix);
  return 0;
}
