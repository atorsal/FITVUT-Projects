// htab_free.c
// Riesenie IJC-DU2, priklad b), 14.4.2016
// Autor: Dávid Bolvanský, FIT
// Prelozene: gcc 4.9
// Popis: Vyhladanie a zrusenie poloziek

#include <stdlib.h>
#include "htable.h"


void htab_free(htab_t * t)
{
	// Free memory of table
	htab_clear(t);
	free(t);
	t = NULL;
}
