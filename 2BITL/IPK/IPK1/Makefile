all:
	g++ -std=c++11 -c network_io.cc
	g++ -std=c++11 -c file_io.cc
	g++ -std=c++11 -c ftrestd.cc
	g++ -std=c++11 -c ftrest.cc
	g++ -std=c++11 -pthread ftrestd.o network_io.o file_io.o -o ftrestd
	g++ -std=c++11 ftrest.o network_io.o file_io.o -o ftrest

clean:
	rm -f *.o ftrest ftrestd
