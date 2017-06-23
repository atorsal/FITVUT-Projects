// ppm.h
// Øešení IJC-DU1, pøíklad b), 4.3.2016
// Autor: Dávid Bolvanský, FIT
// Pøeloženo: gcc 4.9
// Popis: Rozhranie k ppm.c

#ifndef PPM_H
#define PPM_H

// ppm structure
 struct ppm { 
    unsigned xsize; 
    unsigned ysize; 
    char data[]; // RGB bajty, celkem 3*xsize*ysize 
};

#define MAX_IMAGE_SIZE 5000*5000*3

struct ppm * ppm_read(const char * filename);
int ppm_write(struct ppm *p, const char * filename);

#endif // PPM_H
