# Makefile
# Řešení IJC-DU1, příklad b), 28.2.2016
# Autor: Dávid Bolvanský, FIT
# Přeloženo: gcc 4.9
# Zostavenie IJC-DU1

CC=gcc

# Parametre prekladu
CFLAGS=-O2 -std=c99 -Wall -pedantic

# Vytvori vsetky potrebne subory
all: primes primes-inline steg-decode

# Zavislosti pre program primes
primes: primes.o eratosthenes.o error.o
	$(CC) $(CFLAGS) primes.o eratosthenes.o error.o -lm -o $@
primes.o: primes.c bit_array.h eratosthenes.h
	$(CC) $(CFLAGS) -c primes.c -o $@
eratosthenes.o: eratosthenes.c bit_array.h eratosthenes.h
	$(CC) $(CFLAGS) -c eratosthenes.c -o $@

# Zavislosti pre program primes-inline 
primes-inline: primes-inline.o eratosthenes-inline.o error.o
	$(CC) $(CFLAGS) primes-inline.o eratosthenes-inline.o error.o -lm -o $@
primes-inline.o: primes.c bit_array.h eratosthenes.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c primes.c -o $@
eratosthenes-inline.o: eratosthenes.c bit_array.h eratosthenes.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c eratosthenes.c -o $@

# Zavislosti pre program steg-decode
steg-decode: steg-decode.o eratosthenes.o error.o ppm.o
	$(CC) $(CFLAGS) steg-decode.o eratosthenes.o error.o ppm.o -lm -o $@
steg-decode.o: steg-decode.c error.h ppm.h eratosthenes.h
	$(CC) $(CFLAGS) -c steg-decode.c -o $@

# Ostatne zavislosti
bit_array.h: error.o
error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c -o $@
ppm.o: ppm.c ppm.h error.h
	$(CC) $(CFLAGS) -c ppm.c -o $@

# Odstrani vytvorene objekty
clean:
	rm -f *.o

# Odstrani vytvorene objekty a subory
clean-all:
	rm -f *.o primes primes-inline steg-decode
  
# Zabali zdrojove subory do archivu  
zip:
	zip xbolva00.zip *.c *.h Makefile
