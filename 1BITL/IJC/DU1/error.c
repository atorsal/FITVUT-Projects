// error.c
// Řešení IJC-DU1, příklad b), 28.2.2016
// Autor: Dávid Bolvanský, FIT
// Přeloženo: gcc 4.9
// Popis: Implementacia funkcii na vypis chybovych hlaseni

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "error.h"

/* Vypise varovanie na stderr a nasledne vypise argumenty vo formáte funkcie printf */
void warning_msg(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "CHYBA: ");
    vfprintf(stderr,  fmt, args);
    va_end(args);
}

/* Vypise chybovu hlasenie a ukonci program s navratovym kodom 1 */
void fatal_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "CHYBA: ");
    vfprintf (stderr, fmt, args);
    va_end(args);
    exit(1);
}
