#include <stdio.h>

int length_of_string(char chars[]){
    int counter = 0;
    while(chars[counter] != 0){
        ++counter;
    }
    return counter;
}

int main(int argc, char* argv[]){
    if (argc != 3){
        printf("This program expects exactly 2 arguments. You entered %i. Please try again.\n", argc-1);
        return -1;
    }
    for(int i = 1; i < 3; ++i){
        if (length_of_string(argv[i]) != 1){
            printf("This program expects the arguments to be a single digit. However, argument %i entered is of length %i. Please try again.\n", i, length_of_string(argv[i]));
            return -2;
        }
    }    
    for(int i = 1; i < 3; ++i){
        int arg_ascii_value = (int) argv[i][0];
        if (arg_ascii_value < 48 || arg_ascii_value > 57){
            printf("This program expects the arguments to be a digit between 0 and 9. However, argument %i entered is %s. Please try again.\n", i, argv[i]);
            return -3;
        } 
    }

    char digit1 = (int) argv[1][0];
    char digit2 = (int) argv[2][0];
    int digit1_val = digit1 - '0';
    int digit2_val = digit2 - '0';
    printf("%i*%i=%i\n",  digit1_val, digit2_val , digit1_val * digit2_val);

	return 0;
}