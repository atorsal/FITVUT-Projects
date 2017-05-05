all:
	g++ -std=c++11 -c chat_handler.cc
	g++ -std=c++11 -c chat_client.cc
	g++ -std=c++11 -pthread chat_client.o chat_handler.o -o chat_client

clean:
	rm -f *.o chat_client
