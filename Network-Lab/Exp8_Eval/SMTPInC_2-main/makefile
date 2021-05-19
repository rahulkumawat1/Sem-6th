CC = gcc

all: server.o client.o popserver.o

server.o: server.c
	$(CC) server.c -o server.o -lpthread

client.o: client.c
	$(CC) client.c -o client.o

popserver.o: popserver.c
	$(CC) popserver.c -o popserver.o -lpthread

clean:
	rm -f *.o *.out