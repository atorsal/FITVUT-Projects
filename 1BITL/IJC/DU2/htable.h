// htable.h
// Riesenie IJC-DU2, priklad b), 14.4.2016
// Autor: Dávid Bolvanský, FIT
// Prelozene: gcc 4.9
// Popis: Deklaracia struktur a prototypy funkcii

#ifndef HTABLE_H
#define HTABLE_H

typedef struct htab_listitem {
	char *key;
	unsigned data;
	struct htab_listitem *next;
} htab_listitem;


typedef struct {
	unsigned size;
	unsigned int (*hash_fun_ptr) (const char *, unsigned);
	unsigned n;
	htab_listitem *list[];
} htab_t;

unsigned int hash_function(const char *str, unsigned htab_size);
void htab_clear(htab_t * t);
void htab_foreach(htab_t * t,
		  void (*function) (char *key, unsigned value));
void htab_free(htab_t * t);
htab_t *htab_init(unsigned size);
htab_t *htab_init2(unsigned size,
		   unsigned int (*hash_function_ptr) (const char *str,
						      unsigned htab_size));
struct htab_listitem *htab_lookup_add(htab_t * t, const char *key);
void htab_remove(htab_t * t, const char *key);
void htab_statistics(htab_t * t);

#endif				//HTABLE_H