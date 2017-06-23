# Makefile
# Riesenie IJC-DU2, priklad b), 28.4.2016
# Autor: Dávid Bolvanský, FIT
# Popis: Zostavenie IJC-DU2

# format source code with: indent -kr -i8 `ls | egrep '\.(c|cc|h)$'`

CFLAGS=-std=c99 -pedantic -Wall -Wextra
CXXFLAGS=-std=c++11 -pedantic -Wall
CC=gcc
CXX=g++

LIBFILES=hash_function.o htab_init.o htab_init2.o htab_lookup_add.o htab_foreach.o htab_remove.o htab_clear.o htab_free.o
all: tail tail2 wordcount wordcount-dynamic

tail: tail.o
	$(CC) $(CFLAGS) tail.o -o tail

tail2: tail2.o
	$(CXX) $(CXXFLAGS) tail2.o -o tail2
    

tail.o: tail.c
	$(CC) $(CFLAGS) -c tail.c

tail2.o: tail2.cc
	$(CXX) $(CXXFLAGS) -c tail2.cc

wordcount: wordcount.o io.o libhtable.a
	$(CC) $(CFLAGS) -o wordcount -static io.o wordcount.o libhtable.a

wordcount-dynamic: wordcount.o io.o libhtable.so
	$(CC) $(CFLAGS) -o wordcount-dynamic wordcount.o io.o -L.  libhtable.so
	

libhtable.a: $(LIBFILES)
	ar crs libhtable.a $(LIBFILES)

libhtable.so: $(LIBFILES)
	$(CC) $(CFLAGS) -shared -fPIC -o libhtable.so $(LIBFILES)

hash_function.o: hash_function.c
	$(CC) $(CFLAGS) -c -fPIC hash_function.c

htab_init.o: htab_init.c
	$(CC) $(CFLAGS) -c -fPIC htab_init.c
    
htab_init2.o: htab_init2.c
	$(CC) $(CFLAGS) -c -fPIC htab_init2.c

htab_lookup_add.o: htab_lookup_add.c
	$(CC) $(CFLAGS) -c -fPIC htab_lookup_add.c

htab_foreach.o: htab_foreach.c
	$(CC) $(CFLAGS) -c -fPIC htab_foreach.c

htab_remove.o: htab_remove.c
	$(CC) $(CFLAGS) -c -fPIC htab_remove.c

htab_clear.o: htab_clear.c
	$(CC) $(CFLAGS) -c -fPIC htab_clear.c

htab_free.o: htab_free.c
	$(CC) $(CFLAGS) -c -fPIC htab_free.c
    
clean:    
	rm -f *.o *.a *.so tail tail2 wordcount wordcount-dynamic
    
zip:
	zip xbolva00.zip *.c *.cc *.h Makefile 
    