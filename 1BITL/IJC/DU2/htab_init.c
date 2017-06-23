// htab_init.c
// Riesenie IJC-DU2, priklad b), 14.4.2016
// Autor: Dávid Bolvanský, FIT
// Prelozene: gcc 4.9
// Popis: Vytvorenie a inicializacia tabulky

#include <stdlib.h>
#include <stdio.h>
#include "htable.h"

htab_t *htab_init(unsigned size)
{

	// Allocate memory for tab and items
	htab_t *t = malloc(sizeof(htab_t) + size * sizeof(htab_listitem));
	if (t == NULL) {
		fprintf(stderr,
			"Unable to allocate memory for hash table.\n");
		return NULL;
	}

	if (t->list == NULL) {
		free(t);
		return NULL;
	}
	// Initialize items with NULL
	for (unsigned i = 0; i < size; i++) {
		t->list[i] = NULL;
	}
	t->size = size;
	t->n = 0;
	t->hash_fun_ptr = hash_function;

	return t;
}
