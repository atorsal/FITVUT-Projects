// steg_decode.c
// Řešení IJC-DU1, příklad a), 4.3.2016
// Autor: Dávid Bolvanský, FIT
// Přeloženo: gcc 4.9
// Popis: Dekodovanie PPM spravy

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "eratosthenes.h"
#include "error.h"
#include "ppm.h"



void decode_ppm(char data[])
{
	ba_create(array, MAX_IMAGE_SIZE + 1);
	Eratosthenes(array);
    
    unsigned long size = ba_size(array) - 1;
    
    int c = 0;
    int bit_pos = 0;
    int bit = 0;
    
    for (unsigned long i = 2; i <= size; i++ )
	{
        if (ba_get_bit(array, i) == 0) {
			bit = DU1_GET_BIT_((&data[i]), 0);
			DU1_SET_BIT_((&c), bit_pos, bit);
            
            bit_pos++;
            if(bit_pos == CHAR_BIT)
			{
				if(c == '\0')
				{
                    putchar('\n');
					break;
				}
				if(!isprint(c))
				{
					fatal_error("Netlacitelny znak.\n");
				}
				printf("%c", c);
				bit_pos = 0;
			}
			
		}
	}
	
}


int main(int argc, char *argv[]) {
	if (argc != 2) {
		fatal_error("Nespravne argumenty programu.\n");
	}
	else {
		struct ppm* ppm_image = ppm_read(argv[1]);

		if(ppm_image == NULL) {
			fatal_error("Chybny PPM subor.\n");
		}
        
        decode_ppm(ppm_image->data);
        free(ppm_image);
	}

	return EXIT_SUCCESS;
}