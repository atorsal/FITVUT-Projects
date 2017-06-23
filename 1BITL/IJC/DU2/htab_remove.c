// htab_remove.c
// Riesenie IJC-DU2, priklad b), 14.4.2016
// Autor: Dávid Bolvanský, FIT
// Prelozene: gcc 4.9
// Popis: Vyhladanie a zrusenie poloziek

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "htable.h"

void htab_remove(htab_t * t, const char *key)
{
	unsigned i = t->hash_fun_ptr(key, t->size);

	htab_listitem *prev_item = t->list[i];
	htab_listitem *item = t->list[i];

	// Search item, if found, remove it and free memory
	while (item != NULL) {
		if (strcmp(item->key, key) == 0) {
			if (prev_item == item) {
				t->list[i] = item->next;
			}
			prev_item->next = item->next;

			free(item);
			t->n--;
			break;
		} else {
			prev_item = item;
			item = item->next;
		}
	}
}
