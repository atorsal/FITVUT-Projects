// error.h
// Řešení IJC-DU1, příklad b), 28.2.2016
// Autor: Dávid Bolvanský, FIT
// Přeloženo: gcc 4.9
// Popis: Rozhranie k súboru error.c

#ifndef ERROR_H
#define ERROR_H

/* Vypise varovanie na stderr a nasledne vypise argumenty vo formáte funkcie printf */
void warning_msg(const char *fmt, ...);

/* Vypise chybovu hlasku a ukonci program s navratovym kodom 1 */
void fatal_error(const char *fmt, ...);

#endif //ERROR_H
