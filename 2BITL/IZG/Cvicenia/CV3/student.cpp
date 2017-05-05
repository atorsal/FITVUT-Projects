/**
 * @file        student.cpp
 * @author      Petr Kleparnik, VUT FIT Brno, ikleparnik@fit.vutbr.cz
 * @author      Kamil Behun, VUT FIT Brno, ibehun@fit.vutbr.cz
 * @date        13.03.2017
 *
 * @brief       Deklarace funkci studentu. DOPLNUJI STUDENTI
 *
 */

#include <iostream>
#include "base.h"
#include "student.h"
#include "globals.h"

/**
 * @brief Vraci barvu pixelu z pozice [x, y]
 * @param[in] x X souradnice pixelu
 * @param[in] y Y souradnice pixelu
 * @return Barva pixelu na pozici [x, y] ve formatu RGBA
 *
 * Pozor! Nehlida velikost framebufferu, pokud je dana souradnice mimo hranice.
 */
RGBA getPixel(int x, int y)
{
    return framebuffer[y * width + x];
}

/**
 * @brief Nastavi barvu pixelu na pozici [x, y]
 * @param[in] x X souradnice pixelu
 * @param[in] y Y souradnice pixelu
 * @param[in] color Barva pixelu ve formatu RGBA
 *
 * Pozor! Nehlida velikost framebufferu, pokud je dana souradnice mimo hranice.
 */
void putPixel(int x, int y, RGBA color)
{
    framebuffer[y * width + x] = color;
}

/**
 * @brief Vykresli usecku se souradnicemi [x1, y1] a [x2, y2]
 * @param[in] x1 X souradnice 1. bodu usecky
 * @param[in] y1 Y souradnice 1. bodu usecky
 * @param[in] x2 X souradnice 2. bodu usecky
 * @param[in] y2 Y souradnice 2. bodu usecky
 * @param[in] color Barva pixelu usecky ve formatu RGBA
 */
void drawLine(int x1, int y1, int x2, int y2, RGBA color)
{
    bool steep = abs(y2 - y1) > abs(x2 - x1);

    if (steep) {
        SWAP(x1, y1);
        SWAP(x2, y2);
    }

    if (x1 > x2) {
        SWAP(x1, x2);
        SWAP(y1, y2);
    }

    const int dx = x2 - x1, dy = abs(y2 - y1);
    const int P1 = 2 * dy, P2 = P1 - 2 * dx;
    int P = 2 * dy - dx;
    int y = y1;
    int ystep = 1;
    if (y1 > y2) ystep = -1;

    for (int x = x1; x <= x2; x++) {
        if (steep) {
            if (y >= 0 && y < width && x >= 0 && x < height) {
                putPixel(y, x, color);
            }
        } else {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                putPixel(x, y, color);
            }
        }

        if (P >= 0) {
            P += P2;
            y += ystep;
        } else {
            P += P1;
        }
    }
}

/**
 * @brief Vyplni a vykresli trojuhelnik
 * @param[in] v1 Prvni bod trojuhelniku
 * @param[in] v2 Druhy bod trojuhelniku
 * @param[in] v3 Treti bod trojuhelniku
 * @param[in] color1 Barva vyplne trojuhelniku
 * @param[in] color2 Barva hranice trojuhelniku
 *
 * SPOLECNY UKOL. Doplnuji studenti se cvicicim.
 */
void pinedaTriangle(const Point &v1, const Point &v2, const Point &v3, const RGBA &color1, const RGBA &color2)
{
	// Nalezeni obalky (minX, maxX), (minY, maxY) trojuhleniku.

	Point min(MIN(v1.x, MIN(v2.x, v3.x)), MIN(v1.y, MIN(v2.y, v3.y)));
	Point max(MAX(v1.x, MAX(v2.x, v3.x)), MAX(v1.y, MAX(v2.y, v3.y)));

	// Oriznuti obalky (minX, maxX, minY, maxY) trojuhleniku podle rozmeru okna.

	min.x = MIN(width - 1, MAX(0, min.x));
	max.x = MIN(width - 1, MAX(0, max.x));
	min.y = MIN(height - 1, MAX(0, min.y));
	max.y = MIN(height - 1, MAX(0, max.y));

    // Spocitani parametru hranove funkce (A, B, C) pro kazdou hranu.
    // Hranova funkce je obecna rovnice primky Ax + By + C = 0.
    // Primku prochazejici body (x1, y1) a (x2, y2) urcime jako
    // (y1 - y2) * x + (x2 - x1) * y + x1 * y2 - x2 * y1 = 0.

	int a12 = v1.y - v2.y;
	int a23 = v2.y - v3.y;
	int a31 = v3.y - v1.y;

	int b12 = v2.x - v1.x;
	int b23 = v3.x - v2.x;
	int b31 = v1.x - v3.x;

	int c12 = v1.x * v2.y - v2.x * v1.y;
	int c23 = v2.x * v3.y - v3.x * v2.y;
	int c31 = v3.x * v1.y - v1.x * v3.y;

	int e12 = a12 * min.x + b12 * min.y + c12;
	int e23 = a23 * min.x + b23 * min.y + c23;
	int e31 = a31 * min.x + b31 * min.y + c31;

    // Vyplnovani: Cyklus pres vsechny body (x, y) v obdelniku (minX, minY), (maxX, maxY).
    // Pro aktualizaci hodnot hranove funkce v bode P (x +/- 1, y) nebo P (x, y +/- 1)
    // vyuzijte hodnoty hranove funkce E (x, y) z bodu P (x, y).

	for (int y = min.y; y < max.y; y++) {
		int e12L = e12;
		int e23L = e23;
		int e31L = e31;

		for (int x = min.x; x < max.x; x++) {
			if (e12L >= 0 && e23L >= 0 && e31L >= 0)
				putPixel(x, y, color1);

			e12L += a12;
			e23L += a23;
			e31L += a31;
		}
		e12 += b12;
		e23 += b23;
		e31 += b31;
	}

    // Prekresleni hranic trojuhelniku barvou color2.
    drawLine(v1.x, v1.y, v2.x, v2.y, color2);
    drawLine(v2.x, v2.y, v3.x, v3.y, color2);
    drawLine(v3.x, v3.y, v1.x, v1.y, color2);
}

/**
 * @brief Vyplni a vykresli polygon
 * @param[in] points Pole bodu polygonu
 * @param[in] size Pocet bodu polygonu (velikost pole "points")
 * @param[in] color1 Barva vyplne polygonu
 * @param[in] color2 Barva hranice polygonu
 *
 * SAMOSTATNY BODOVANY UKOL. Doplnuji pouze studenti.
 */
void pinedaPolygon(const Point *points, const int size, const RGBA &color1, const RGBA &color2)
{
    // Pri praci muzete vyuzit pro vas predpripravene datove typy z base.h., napriklad:
    //
    //      Pro ukladani parametru hranovych funkci muzete vyuzit prichystany vektor parametru hranovych funkci "EdgeParams":
    //
    //          EdgeParams edgeParams(size)                         // Vytvorite vektor (pole) "edgeParams" parametru hranovych funkci o velikosti "size".
    //          edgeParams[i].a, edgeParams[i].b, edgeParams[i].c   // Pristup k parametrum (a, b, c) hranove funkce v poli "edgeParams" na indexu "i".
    //
    //      Pro ukladani hodnot hranovych funkci muzete vyuzit prichystany vektor hodnot hranovych funkci "EdgeFncValues":
    //
    //          EdgeFncValues edgeFncValues(size)                   // Vytvorite vektor (pole) "edgeFncValues" hodnot hranovych funkci o velikosti "size".
    //          edgeFncValues[i]                                    // Pristup k hodnote hranove funkce v poli "edgeFncValues" na indexu "i".
    //

    // Nalezeni obalky (minX, maxX), (minY, maxY) polygonu.

	Point max(points[0].x, points[0].y);
	Point min(points[0].x, points[0].y);

	for (int i = 0; i < size; i++) {
		min.x = MIN(min.x, points[i].x);
		max.x = MAX(max.x, points[i].x);
		min.y = MIN(min.y, points[i].y);
		max.y = MAX(max.y, points[i].y);
	}

    // Oriznuti obalky (minX, maxX), (minY, maxY) polygonu podle rozmeru okna

	min.x = MIN(width - 1, MAX(0, min.x));
	max.x = MIN(width - 1, MAX(0, max.x));
	min.y = MIN(height - 1, MAX(0, min.y));
	max.y = MIN(height - 1, MAX(0, max.y));

    // Spocitani parametru hranove funkce pro kazdou hranu.
    // Hranova funkce je obecna rovnice primky Ax + By + C = 0.
    // Primku prochazejici body (x1, y1) a (x2, y2) urcime jako
    // (y1 - y2) * x + (x2 - x1) * y + x1 * y2 - x2 * y1 = 0.

	EdgeParams edges(size);

	for (int i = 0; i < size; i++) {
		int next = (i + 1) % size;
		edges[i].a = points[i].y - points[next].y;
		edges[i].b = points[next].x - points[i].x;
		edges[i].c = points[i].x * points[next].y - points[next].x * points[i].y;
	}

    // Test konvexnosti polygonu

	for (int i = 0; i < size; i++) {
		int n = (i + 2) % size;
		int value = edges[i].a * points[n].x + edges[i].b * points[n].y + edges[i].c;
		if (value < 0) {
			std::cout << "Nie je konvexne" << std::endl;
			return;
		}
	}

    // Vyplnovani: Cyklus pres vsechny body (x, y) v obdelniku (minX, minY), (maxX, maxY).
    // Pro aktualizaci hodnot hranove funkce v bode P (x +/- 1, y) nebo P (x, y +/- 1)
    // vyuzijte hodnoty hranove funkce E (x, y) z bodu P (x, y) */

	EdgeFncValues edge_fn_values(size);
	for (int i = 0; i < size; i++) {
		edge_fn_values[i] = edges[i].a * min.x + edges[i].b  * min.y + edges[i].c;
	}

	for (int y = min.y; y <= max.y; y++) {
		EdgeFncValues values = edge_fn_values;

		for (int x = min.x; x <= max.x; x++) {
			bool in = true;

			for (int i = 0; i < size; i++) {
				if (values[i] < 0)
				{
					in = false;
					break;
				}
			}

			if (in) putPixel(x, y, color1);

			for (int i = 0; i < size; i++)
				values[i] += edges[i].a;

		}

		for (int i = 0; i < size; i++) {
			edge_fn_values[i] += edges[i].b;
		}

	}

    // Prekresleni hranic polygonu barvou color2.
    for (int i = 0; i < size; i++) {
        if (i == size - 1) {
            drawLine(points[i].x, points[i].y, points[0].x, points[0].y, color2);
        } else {
            drawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, color2);
        }
    }
}
