// primes.c
// Řešení IJC-DU1, příklad b), 28.2.2016
// Autor: Dávid Bolvanský, FIT
// Přeloženo: gcc 4.9
// Popis: Vypis 10 poslednych prvocisel

#include <stdio.h>
#include <stdlib.h>
#include "eratosthenes.h"


#define N 202000000
#define PRIMES_COUNT 10
    
int main(void) {
    ba_create(array, N + 1);
    Eratosthenes(array);
    
    unsigned long primes[10] = {0};
    unsigned long i = ba_size(array) - 1, j = 0;
    while (j < PRIMES_COUNT){
        if(ba_get_bit(array,i) == 0)
		{
            primes[j] = i;
            j++;
		}
        
        if (i == 0) {
            break;
        }
        i--;
	}
    
    for(int p = PRIMES_COUNT - 1; p >= 0; p--) {
		printf("%lu\n", primes[p]);
    }
    
    
    return EXIT_SUCCESS;
}


