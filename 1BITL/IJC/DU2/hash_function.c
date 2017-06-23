// hash_function.c
// Riesenie IJC-DU2, priklad b), 14.4.2016
// Autor: Dávid Bolvanský, FIT
// Prelozene: gcc 4.9
// Popis: Hashovacia funkcia pre tabulku

#include "htable.h"


unsigned int hash_function(const char *str, unsigned htab_size)
{
	unsigned int h = 0;
	const unsigned char *p;
	for (p = (const unsigned char *) str; *p != '\0'; p++)
		h = 65599 * h + *p;
	return h % htab_size;
}
