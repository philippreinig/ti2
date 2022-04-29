/*
Aufgabe 1: Threads erstellen [2 Pkt.]
Schreiben Sie ein kleines C-Programm, welches POSIX-Threads erzeugt (pthread create(3)). Dabei soll die Anzahl der zu erzeugenden Threads per Kommandozeile ubergeben ¨
werden. Jeder Thread soll dann in einer Endlosschleife den Text Thread <x>: Hello World!
ausgeben, wobei <x> die Nummer des erzeugten Threads ist. Nach jeder Textausgabe soll
sich der Thread fur eine Sekunde schlafen legen ( ¨ sleep(1);). (Das Programm beendet sich
erst durch Drucken von CTRL+C.) Beispielausgabe: ¨
$ ./threads 3
Thread 1: Hello World!
Thread 2: Hello World!
Thread 1: Hello World!
Thread 3: Hello World!
Thread 1: Hello World!
Thread 2: Hello World!
...
Hinweis: Binden Sie die Header der POSIX threads Bibliothek und unistd.h fur die ¨ sleepFunktion ein.
# include < pthread .h >
# include < unistd .h >
Kompilieren Sie mit der Option -pthread, um die Bibliothek zu linken; z.B.
gcc -o threads -lpthreads threads.c
*/

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void* print_infintely(void* indx_p){
    int* indx = (int*) indx_p;
    printf("print_infintely called with argument: %i (PID:%u, %u)\n", (*indx), getpid(), getppid());
    int i;
    for(int i = 0; 1; ++i){
        printf("Thread %i (%i): Hello World!\n", *indx, i);
        sleep(5);
    }
    return (void*) NULL;
}

int main(int argc, char* argv[]){
    
    if (argc != 2){
        printf("ERROR: Invalid amount of arguments - exactly 2 required!\n");
        return -1;
    }

    int n = atoi(argv[1]);

    pthread_t pids[n];
    int ids[n];
    int thread_return_vals[n];

    printf("n=%i\n", n);

    for(int i = 0; i < n; ++i){
        ids[i] = i;
        thread_return_vals[i] = pthread_create(&pids[i], NULL, print_infintely, (void*) &ids[i]);
    }

    for(int i = 0; i < n; ++i){
        pthread_join(pids[i], NULL);
    }

    for(int i = 0; i < n; ++i){
        printf("thread_return_vals[%i]=%i\n", i, thread_return_vals[i]);
    }
    
    return 0;
}