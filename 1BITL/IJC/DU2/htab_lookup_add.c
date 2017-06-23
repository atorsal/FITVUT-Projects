// htab_lookup_add.c
// Riesenie IJC-DU2, priklad b), 14.4.2016
// Autor: Dávid Bolvanský, FIT
// Prelozene: gcc 4.9
// Popis: Vyhladavanie polozky v zozname, ak neexistujem vytvori ju

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "htable.h"

struct htab_listitem *htab_lookup_add(htab_t * t, const char *key)
{
	unsigned int hash = t->hash_fun_ptr(key, t->size);

	// Pointer on first item
	htab_listitem *item = t->list[hash];

	// Searching item by key
	while (item != NULL) {
		//If found, return it
		if (strcmp(key, item->key) == 0)
			return item;

		if (item->next != NULL) {
			item = item->next;
		} else {
			break;
		}
	}

	// Not found, append item
	htab_listitem *new_item = malloc(sizeof(htab_listitem));
	if (new_item == NULL)
		return NULL;

	// Allocate memory for key
	new_item->key = malloc(sizeof(char) * (strlen(key) + 1));
	if (new_item->key == NULL) {
		free(new_item);
		return NULL;
	}
	// Fill data
	strcpy(new_item->key, key);
	new_item->data = 0;
	new_item->next = NULL;

	// Append to list
	if (item == NULL) {
		t->list[hash] = new_item;
	} else {
		item->next = new_item;
	}

	// Increment counter
	t->n++;

	return new_item;
}
