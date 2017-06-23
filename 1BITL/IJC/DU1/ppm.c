// ppm.c
// Øešení IJC-DU1, pøíklad b), 4.3.2016
// Autor: Dávid Bolvanský, FIT
// Pøeloženo: gcc 4.9
// Popis: Nacitanie a zapis ppm suborov

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "error.h"
#include "ppm.h"

struct ppm * ppm_read(const char * filename)
{
	FILE *ppm_file;
	struct ppm* image = NULL;
	unsigned xsize, ysize;
    
    ppm_file = fopen(filename, "r");
    if (ppm_file == NULL)
    {
		warning_msg("Nepodarilo sa otvorit subor %s.\n",filename);
        return NULL;
    }


    if(fscanf(ppm_file, "P6 %u %u 255 ", &xsize, &ysize) != 2)
    {
		fclose(ppm_file);
		warning_msg("Subor %s nie je platnym PPM suborom.\n",filename);
        return NULL;
	}

	unsigned long img_size =  3 * xsize * ysize * sizeof(char);
    

	image = malloc(img_size + sizeof(struct ppm));
	if(image == NULL)
	{
		fclose(ppm_file);
		warning_msg("Nepodarila sa alokacia pamate pre obrazok.\n");
        return NULL;
	}

	if( fread(&image->data, sizeof(char), img_size, ppm_file) != img_size || ferror(ppm_file) != 0 || fgetc(ppm_file) != EOF)
	{
		fclose(ppm_file);
		free(image);
		warning_msg("Chyba pri citani obrazku.\n");
        return NULL;
	}

	fclose(ppm_file);

	image->xsize = xsize;
	image->ysize = ysize;

    return image;
}

int ppm_write(struct ppm *p, const char * filename) {
    FILE *ppm_file = fopen(filename, "wb");

    if (ppm_file == NULL) {
        warning_msg("Nepodarilo sa otvorit subor %s.\n", filename);
        return -1;
    }
    if (fprintf(ppm_file, "P6\n%u %u\n255\n", p->xsize, p->ysize) < 0) {
        warning_msg("Nepodarilo sa zapisat hlavicku do suboru %s.\n", filename);
        fclose(ppm_file);
        return -2;
    }
    unsigned long img_size = MAX_IMAGE_SIZE * sizeof(char);
    if(fwrite(p->data, sizeof(char), img_size, ppm_file) != img_size) {
        warning_msg("Nepodarilo sa zapisat data do suboru %s.\n", filename);
        fclose(ppm_file);
        return -3;
    }

    fclose(ppm_file);
    return 0;
}