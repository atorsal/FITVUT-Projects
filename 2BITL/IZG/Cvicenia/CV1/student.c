/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * $Id: $
 * 
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * -
 */

#include "student.h"
#include "globals.h"

#include <time.h>

/******************************************************************************
 ******************************************************************************
 Funkce vraci pixel z pozice x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce vraci barvu (0, 0, 0).
 Ukol za 0.5 bodu */
S_RGBA getPixel(int x, int y)
{
	if (x >= 0 && y >= 0 && x < width && y < height) {
		return frame_buffer[y*width +x];
	}
	
	return makeBlackColor(); //vraci barvu (0, 0, 0)
}

/******************************************************************************
 ******************************************************************************
 Funkce vlozi pixel na pozici x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce neprovadi zadnou zmenu.
 Ukol za 0.5 bodu */
void putPixel(int x, int y, S_RGBA color)
{
	if (x >= 0 && y >= 0 && x < width && y < height) {
		frame_buffer[y*width + x] = color;
	}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na odstiny sedi. Vyuziva funkce GetPixel a PutPixel.
 Ukol za 1 bod. */
void grayScale()
{
	S_RGBA color;
	int intensity = 0;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < width; y++) {
			color = getPixel(x, y);
			intensity = ROUND(0.299 * color.red + 0.587 * color.green + 0.114 * color.blue);
			color.red = color.green = color.blue = intensity;
			putPixel(x, y, color);
		}
	}
}

void distributeError(int x, int y, float error) {
	S_RGBA color; 
	color = getPixel(x, y);
	int c = ROUND(color.green + error);
	if (c > 255) c = 255;
	if (c < 0) c = 0;
	color.red = color.green = color.blue = c;
	putPixel(x, y, color);
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu distribuce chyby.
 Ukol za 1 bod */
void errorDistribution()
{
	S_RGBA color;
	int error = 0;
	int c = 0;
	grayScale();
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			color = getPixel(x, y);
			c = color.green;
			if (c <= 128) {
				color = COLOR_BLACK;
				error = c;
			}
			else {
				color = COLOR_WHITE;
				error = c - 255;
			}
			
			putPixel(x, y, color);

			distributeError(x + 1, y, (3.0 / 8.0) * error);
			distributeError(x, y + 1, (3.0 / 8.0) * error);
			distributeError(x + 1, y + 1, (1.0 / 4.0) * error);
		}
	}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci metody prahovani.
 Demonstracni funkce */
void thresholding(int Threshold)
{

	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);

			/* Porovname hodnotu cervene barevne slozky s prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > Threshold)
				putPixel(x, y, COLOR_WHITE);
			else
				putPixel(x, y, COLOR_BLACK);
		}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci nahodneho rozptyleni. 
 Vyuziva funkce GetPixel, PutPixel a GrayScale.
 Demonstracni funkce. */
void randomDithering()
{

	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Inicializace generatoru pseudonahodnych cisel */
	srand((unsigned int)time(NULL));

	/* Projdeme vsechny pixely obrazku */
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);
			
			/* Porovname hodnotu cervene barevne slozky s nahodnym prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > rand()%255)
			{
				putPixel(x, y, COLOR_WHITE);
			}
			else
				putPixel(x, y, COLOR_BLACK);
		}
}
/*****************************************************************************/
/*****************************************************************************/