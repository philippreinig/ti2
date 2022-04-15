#include <stdio.h>

int fibonacci(int n){
    if (n < 0){
        printf("ERROR: Can't calculate fibonacci of a negative number.");
        return -1;
    }

    if (n == 0 || n == 1 ) return n;
    else return fibonacci(n-1) + fibonacci(n-2);
}

int main(){
    int x = 10;
    printf("fib(%u)=%u\n",x, fibonacci(x));
    return 0;
}
