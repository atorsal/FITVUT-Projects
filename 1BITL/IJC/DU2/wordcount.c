// wordcount.c
// Riesenie IJC-DU2, priklad b), 14.4.2016
// Autor: Dávid Bolvanský, FIT
// Prelozene: gcc 4.9
// Popis: Pocitanie vyskytu slov

#include <stdio.h>
#include <stdlib.h>
#include "htable.h"
#include "io.h"

// Word length limit
#define MAX_WORD_LEN 127

// Cislo bolo vybrane na zaklade experimentalneho dosazdovania a podmienkou bolo
// aby dane cislo bolo prvocislo pre efektivnu pracu s tabulkou
// Vybrane z tabulky: http://planetmath.org/goodhashtableprimes
// Informacie: https://en.wikipedia.org/wiki/Universal_hashing
#define HASH_TABLE_SIZE 12289

//Function prototype
void print_item(char *key, unsigned int value);

void print_item(char *key, unsigned int value)
{
	printf("%s\t%d\n", key, value);
}

int main(void)
{
	char buffer[MAX_WORD_LEN + 1];
	struct htab_listitem *item;

	// Inicialize hash table
	htab_t *htable = htab_init(HASH_TABLE_SIZE);

	// Test for htab_init2
	// htab_t *htable = htab_init2(HASH_TABLE_SIZE, &hash_function);

	if (htable == NULL) {
		fprintf(stderr, "Unable to allocate memory.\n");
		return -1;
	}
	// Read words
	while (get_word(buffer, MAX_WORD_LEN, stdin) != EOF) {
		if ((item = htab_lookup_add(htable, buffer)) == NULL) {
			fprintf(stderr,
				"Unable to allocate memory for new item.\n");
			htab_free(htable);
			return -1;
		}
		// Test for htab_remove
		// htab_remove(htable, item->key);

		item->data++;
	}

	// Print counts for words
	htab_foreach(htable, print_item);

	// Free hash table from memory
	htab_free(htable);

    return EXIT_SUCCESS;
}
