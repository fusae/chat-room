server: chat_room.o chat_session.o chat_server.o server.o
	g++ -Wall -std=c++11 $^ -o $@ -lboost_system

server_debug: chat_room.o chat_session.o chat_server.o server.o
	g++ -g -Wall -std=c++11 $^ -o $@ -lboost_system

client: chat_client.o
	g++ -Wall -std=c++11 $^ -o $@ -lboost_system -lpthread

CC=g++
CFLAGS= -std=c++11 -c
chat_room.o: chat_room.cpp
	$(CC) $(CFLAGS) $^

chat_session.o: chat_session.cpp
	$(CC) $(CFLAGS) $^

chat_server.o: chat_server.cpp
	$(CC) $(CFLAGS) $^

server.o: server.cpp
	$(CC) $(CFLAGS) $^

chat_client.o: chat_client.cpp
	$(CC) $(CFLAGS) $^

clean:
	rm -f *.o
