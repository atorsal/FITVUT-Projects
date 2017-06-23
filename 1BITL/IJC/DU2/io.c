// io.c
// Riesenie IJC-DU2, priklad b), 14.4.2016
// Autor: Dávid Bolvanský, FIT
// Prelozene: gcc 4.9
// Popis: Nacitavanie slov zo streamu

#include <stdio.h>
#include <ctype.h>


int get_word(char *s, int max, FILE * f)
{

	if (!s || !f) {
		fprintf(stderr, "Null pointer.\n");
		return EOF;
	}

	if (max < 0) {
		fprintf(stderr, "Wrong max value.\n");
		return EOF;
	}

	char c;
	static int is_warning = 0;
	int i = 0;

	while ((c = fgetc(f)) != EOF) {
		if (!isspace(c)) {
			break;
		}
	}
	if (c == EOF) {
		return EOF;
	}

	ungetc(c, f);

	// Read words
	while ((c = fgetc(f)) != EOF) {
		if (isspace(c) || i == max) {
			if (i == max) {
				if (!is_warning) {
					fprintf(stderr,
						"Limit of word length (127) was reached. Cutting line...\n");
					is_warning = 1;
				}
				while (!isspace(c) && c != EOF) {
					c = fgetc(f);
				}
				if (c == EOF)
					return EOF;
			}
			break;
		}
		s[i++] = c;
	}

	// Oh, we can have an incorrectly ended line. Comment this.
	/*if (c == EOF) {
	   return EOF;
	   } */

	s[i] = '\0';
	return i;
}
