// bit_array.h
// Řešení IJC-DU1, příklad a), 28.2.2016
// Autor: Dávid Bolvanský, FIT
// Prelozeno: gcc 4.9
// Popis: Implementacia bitoveho pola

#include <limits.h>
#include "error.h"


#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

typedef unsigned long bit_array_t[];

// Pocet bitov v unsigned longu
#define UL_BITS (CHAR_BIT * sizeof(unsigned long))
#define BIT_POS(index) ((index / UL_BITS) + 1)
#define T_BITS(jmeno_pole) (CHAR_BIT * sizeof(*jmeno_pole))
#define T_BIT_POS(jmeno_pole, index) (index/ (sizeof(*jmeno_pole) * CHAR_BIT))
#define BIT_ARRAY_SIZE(velikost) ((velikost) % UL_BITS) == 0 ? (((velikost)/UL_BITS) + 1) : (((velikost)/UL_BITS) + 2)


#define ba_create(jmeno_pole, velikost)   unsigned long jmeno_pole[BIT_ARRAY_SIZE(velikost)] = {velikost, 0}

#define DU1_SET_BIT_(p,i,b) p[T_BIT_POS(p,i)] = (b) ? (p[T_BIT_POS(p,i)] | (1ULL << (i % T_BITS(p)))) :                                           \
     (p[T_BIT_POS(p,i)] & ~(1ULL << (i % T_BITS(p))))

#define DU1_GET_BIT_(p,i) (p[T_BIT_POS(p,i)] & (1ULL << (i % T_BITS(p)))) ? 1 : 0


#ifdef USE_INLINE
// inline funkcie
inline unsigned long ba_size(bit_array_t jmeno_pole) {
    return jmeno_pole[0];
}

inline void ba_set_bit(bit_array_t jmeno_pole, unsigned long index, int vyraz) {
    if(index >= ba_size(jmeno_pole)) {
        fatal_error("Index %ld mimo rozsah 0..%ld", (long)index, (long)ba_size(jmeno_pole));
    }
    jmeno_pole[BIT_POS(index)] = (vyraz) ? (jmeno_pole[BIT_POS(index)] | (1ULL << (index % UL_BITS))) :  (jmeno_pole[BIT_POS(index)] & ~(1ULL << (index % UL_BITS))); 
}

inline int ba_get_bit(bit_array_t jmeno_pole, unsigned long index) {
    if(index >= ba_size(jmeno_pole)) {
        fatal_error("Index %ld mimo rozsah 0..%ld", (long)index, (long)ba_size(jmeno_pole));
    }
    return (jmeno_pole[BIT_POS(index)] & (1ULL <<(index % UL_BITS)) ? 1 : 0);
}

#else
// Makra
#define ba_size(jmeno_pole) jmeno_pole[0]
#define ba_set_bit(jmeno_pole, index, vyraz)                                                                                                   \
    (index >= ba_size(jmeno_pole)) ? (fatal_error("Index %ld mimo rozsah 0..%ld", (long)index, (long)ba_size(jmeno_pole)), 0) :             \
    (jmeno_pole[BIT_POS(index)] = (vyraz) ? (jmeno_pole[BIT_POS(index)] | (1ULL << (index % UL_BITS))) :                                \
            (jmeno_pole[BIT_POS(index)] & ~(1ULL << (index % UL_BITS))))

#define ba_get_bit(jmeno_pole,index)  \
    (index >= ba_size(jmeno_pole)) ? (fatal_error("Index %ld mimo rozsah 0..%ld", (long)index, (long)ba_size(jmeno_pole)),0) :             \
    (jmeno_pole[BIT_POS(index)] & (1ULL <<(index % UL_BITS)) ? 1 : 0)


#endif // USE_INLINE

#endif // BIT_ARRAY_H
