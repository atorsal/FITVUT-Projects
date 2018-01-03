// htab_clear.c
// Riesenie IJC-DU2, priklad b), 14.4.2016
// Autor: Dávid Bolvanský, FIT
// Prelozene: gcc 4.9
// Popis: Zrusenie vsetkych poloziek v tabulke

#include <stdlib.h>
#include <stdio.h>
#include "htable.h"

void htab_clear(htab_t * t)
{
	htab_listitem *item;
	htab_listitem *next_item;

	// Free memory of all items in table
	for (unsigned i = 0; i < t->size; i++) {
		if (t->list[i] != NULL) {
			item = t->list[i];
			while (item != NULL) {
				next_item = item->next;
				free(item->key);
				free(item);
				item = next_item;
			}

			t->list[i] = NULL; // if not, free causes abort (invalid pointer)
                               // in unit tests after double clear
		}
	}

	t->n = 0;
}
