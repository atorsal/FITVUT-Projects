// htab_foreach.c
// Riesenie IJC-DU2, priklad b), 14.4.2016
// Autor: Dávid Bolvanský, FIT
// Prelozene: gcc 4.9
// Popis: Volanie funkcie pre kazdy prvok

#include <stdio.h>
#include <stdlib.h>
#include "htable.h"

void htab_foreach(htab_t * t, void (*function) (char *key, unsigned value))
{
	struct htab_listitem *h_item;

	// Call function for every item
	for (unsigned i = 0; i < t->size; i++) {
		if (t->list[i] != NULL) {
			for (h_item = t->list[i]; h_item != NULL;
			     h_item = h_item->next) {
				function(h_item->key, h_item->data);
			}
		}
	}
}
