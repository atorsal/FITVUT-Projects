// eratostenes.c
// Řešení IJC-DU1, příklad a), 28.2.2016
// Autor: Dávid Bolvanský, FIT
// Přeloženo: gcc 4.9
// Popis: Implementacia eratostenovho sita

#include <stdio.h>
#include <math.h>
#include "eratosthenes.h"

// Vypocet prvocisel pomocou eratostenovho sita
// Prvocisla su v bitovom poli ako nuly
void Eratosthenes(bit_array_t pole) {
	unsigned long N = ba_size(pole);
	unsigned long limit = sqrt(N);
	
    // 0 a 1 nie su prvocisla
    ba_set_bit(pole, 0, 1);
    ba_set_bit(pole, 1, 1);
    unsigned long i = 2;
    unsigned long j = 2;
    
    while (i <= limit) {
        if (ba_get_bit(pole,i) == 0) {
            j = 2 * i;
			while (j < N) {
                ba_set_bit(pole, j, 1);
                j += i;
            }
		}
        i++;
    }
}
